#include "MenuHeaderRenderer.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Fonts.h"
#include "Globals.h"
#include "I18n/I18n.h"
#include "Menu.h"
#include "Plugin.h"
#include "ShaderCache.h"
#include "State.h"
#include "ThemeManager.h"
#include "Util.h"
#include "Utils/UI.h"
#include "Utils/VectorIcons.h"

namespace
{
	// Logo height relative to the title line, and the gap between logo and title.
	constexpr float kLogoHeightRatio = 1.9f;
	constexpr float kLogoTextGapRatio = 0.45f;
	constexpr float kUnsavedDotRatio = 0.22f;
	constexpr float kCommandGroupGapRatio = 3.0f;

	struct TitleRowMetrics
	{
		float width = 0.0f;
		float height = 0.0f;
	};

	/** @brief Draws the bottle logo followed by the product title on one line. */
	TitleRowMetrics RenderTitleRow()
	{
		const auto versionStr = Util::GetFormattedVersion(Plugin::VERSION);
		const auto title = std::format("{} {}", Plugin::DISPLAY_NAME, versionStr);
		const bool showBuildTag = Plugin::BUILD_DESCRIBE != std::format("v{}", versionStr);

		const ImVec2 origin = ImGui::GetCursorScreenPos();
		TitleRowMetrics metrics{};
		{
			MenuFonts::FontRoleGuard titleFont(Menu::FontRole::Title);

			const float fontSize = ImGui::GetFontSize();
			const float logoHeight = fontSize * kLogoHeightRatio;
			const float logoWidth = logoHeight * Util::Icons::kHoldMarkAspect;
			const float gap = fontSize * kLogoTextGapRatio;
			const float rowHeight = ImMax(logoHeight, ImGui::GetTextLineHeight());

			Util::Icons::DrawHoldMark(ImGui::GetWindowDrawList(),
				ImVec2(origin.x, origin.y + (rowHeight - logoHeight) * 0.5f),
				logoHeight,
				ImGui::GetColorU32(ImGuiCol_Text));

			ImGui::SetCursorScreenPos(ImVec2(origin.x + logoWidth + gap, origin.y + (rowHeight - ImGui::GetTextLineHeight()) * 0.5f));
			ImGui::TextUnformatted(title.c_str());
			if (auto _tt = Util::HoverTooltipWrapper())
				ImGui::TextUnformatted(Plugin::CREDITS.data(), Plugin::CREDITS.data() + Plugin::CREDITS.size());

			metrics = { logoWidth + gap + ImGui::CalcTextSize(title.c_str()).x, rowHeight };
		}

		if (showBuildTag) {
			const auto buildTag = Util::GetBuildLabel();
			const float spacing = ImGui::GetStyle().ItemSpacing.x;
			ImGui::SetCursorScreenPos(ImVec2(origin.x + metrics.width + spacing, origin.y + (metrics.height - ImGui::GetTextLineHeight()) * 0.5f));
			ImGui::TextDisabled("%s", buildTag.c_str());
			metrics.width += spacing + ImGui::CalcTextSize(buildTag.c_str()).x;
		}

		ImGui::SetCursorScreenPos(ImVec2(origin.x, origin.y + metrics.height));
		ImGui::Dummy(ImVec2(0.0f, 0.0f));

		return metrics;
	}

	// Hold Shaders: the mod links its Modhold page from its menu, asking before it opens the browser.
	constexpr const char* kModholdPageUrl = "https://modholdmods.com/m/1522";
	constexpr const char* kModholdAuthorUrl = "https://modholdmods.com/u/SethSaidThat";
	bool showModholdLinks = false;

	/** @brief Draws the "Open Modhold?" popup while requested; each choice opens the browser only once picked. */
	void DrawModholdLinksPopup()
	{
		if (!showModholdLinks)
			return;
		const auto title = std::format("{}###HoldShadersModholdLinks", T("menu.modhold.title", "Open Modhold?"));
		ImGui::OpenPopup(title.c_str());
		if (auto popup = Util::CenteredPopupModal(title.c_str(), &showModholdLinks)) {
			ImGui::TextWrapped("%s", T("menu.modhold.message", "This opens Modhold in your web browser."));
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			const auto choice = [](const char* a_label, const char* a_url) {
				if (!Util::Button(a_label))
					return;
				if (a_url)
					ShellExecuteA(nullptr, "open", a_url, nullptr, nullptr, SW_SHOWNORMAL);
				showModholdLinks = false;
				ImGui::CloseCurrentPopup();
			};
			choice(T("menu.modhold.page", "Hold Shaders page"), kModholdPageUrl);
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			choice(T("menu.modhold.author", "SethSaidThat on Modhold"), kModholdAuthorUrl);
			ImGui::SameLine();
			choice(T("ui.cancel", "Cancel"), nullptr);
		}
	}

	float IconLabelButtonWidth(const char* label)
	{
		const auto& style = ImGui::GetStyle();
		return ImGui::GetFontSize() + style.ItemInnerSpacing.x + ImGui::CalcTextSize(label, nullptr, true).x + style.FramePadding.x * 2.0f;
	}

	void RenderWindowControls(Menu* menu)
	{
		const bool sidePanel = menu->IsSidePanelLayout();
		if (Util::Icons::IconButton("##MenuLayout", sidePanel ? Util::Icons::Kind::Centered : Util::Icons::Kind::SidePanel))
			menu->SetSidePanelLayout(!sidePanel);
		if (auto _tt = Util::HoverTooltipWrapper()) {
			ImGui::Text("%s", sidePanel ? T("menu.layout_centered_tooltip", "Center the menu on screen.") :
			                              T("menu.layout_side_panel_tooltip", "Dock the menu to the edge of the screen so the game stays visible while you adjust settings."));
		}

		ImGui::SameLine();
		if (Util::Icons::IconButton("##CloseMenu", Util::Icons::Kind::Close))
			menu->Close();
		if (auto _tt = Util::HoverTooltipWrapper())
			ImGui::Text("%s", T("menu.close_tooltip", "Close the menu (Esc)."));
	}
}

void MenuHeaderRenderer::RenderHeader(bool isDocked)
{
	auto* menu = globals::menu;
	if (!menu) {
		logger::error("MenuHeaderRenderer::RenderHeader: globals::menu is null, cannot render header");
		return;
	}

	const auto& style = ImGui::GetStyle();
	const float frameHeight = ImGui::GetFrameHeight();
	const float fullWidth = ImGui::GetContentRegionAvail().x;
	const ImVec2 rowOrigin = ImGui::GetCursorScreenPos();

	// When docked the tab already names the window and offers its own close button.
	TitleRowMetrics title{};
	if (!isDocked)
		title = RenderTitleRow();
	const float rowHeight = ImMax(title.height, frameHeight);
	const float controlsY = rowOrigin.y + (rowHeight - frameHeight) * 0.5f;
	float rightEdge = rowOrigin.x + fullWidth;

	if (!isDocked) {
		const float controlsWidth = frameHeight * 2.0f + style.ItemSpacing.x;
		ImGui::SetCursorScreenPos(ImVec2(rightEdge - controlsWidth, controlsY));
		RenderWindowControls(menu);
		rightEdge -= controlsWidth + style.ItemSpacing.x * kCommandGroupGapRatio;
	}

	const bool dirty = menu->HasUnsavedChanges();
	const char* unsavedLabel = T("menu.unsaved_changes", "Unsaved changes");
	const char* saveLabel = T("menu.save_settings", "Save Settings");
	const char* revertLabel = T("menu.revert_settings", "Revert");
	const char* clearCacheLabel = T("menu.clear_shader_cache", "Clear Shader Cache");
	const char* modholdLabel = T("menu.modhold.button", "Modhold");
	const float modholdWidth = ImGui::CalcTextSize(modholdLabel, nullptr, true).x + style.FramePadding.x * 2.0f;
	const float dotRadius = ImGui::GetFontSize() * kUnsavedDotRatio;
	const float unsavedWidth = dotRadius * 2.0f + style.ItemInnerSpacing.x + ImGui::CalcTextSize(unsavedLabel).x;
	const float groupGap = style.ItemSpacing.x * kCommandGroupGapRatio;

	const float commandsWidth = (dirty ? unsavedWidth + style.ItemSpacing.x : 0.0f) +
	                            IconLabelButtonWidth(saveLabel) + style.ItemSpacing.x +
	                            IconLabelButtonWidth(revertLabel) + groupGap +
	                            IconLabelButtonWidth(clearCacheLabel) + groupGap +
	                            modholdWidth;

	const float titleRight = rowOrigin.x + title.width + (isDocked ? 0.0f : groupGap);
	const bool commandsInline = rightEdge - commandsWidth >= titleRight;
	const float belowTitle = rowOrigin.y + rowHeight + style.ItemSpacing.y;
	if (commandsInline)
		ImGui::SetCursorScreenPos(ImVec2(rightEdge - commandsWidth, controlsY));
	else
		ImGui::SetCursorScreenPos(ImVec2(rowOrigin.x, isDocked ? rowOrigin.y : belowTitle));

	bool first = true;
	auto place = [&first](float width, float spacing) {
		if (!first) {
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::GetContentRegionAvail().x < width)
				ImGui::NewLine();
		}
		first = false;
	};

	if (dirty) {
		place(unsavedWidth, style.ItemSpacing.x);
		const ImVec2 dotOrigin = ImGui::GetCursorScreenPos();
		ImGui::Dummy(ImVec2(dotRadius * 2.0f, frameHeight));
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(dotOrigin.x + dotRadius, dotOrigin.y + frameHeight * 0.5f), dotRadius, ImGui::GetColorU32(Util::Colors::GetWarning()));
		ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
		ImGui::AlignTextToFramePadding();
		Util::Text::Warning("%s", unsavedLabel);
		if (auto _tt = Util::HoverTooltipWrapper()) {
			ImGui::Text("%s", T("menu.unsaved_changes_tooltip",
								  "Your changes are already active, but they are lost when the game closes.\n"
								  "Save Settings keeps them. Revert returns to your saved settings."));
		}
	}

	place(IconLabelButtonWidth(saveLabel), style.ItemSpacing.x);
	bool savePressed = false;
	if (dirty) {
		auto highlight = Util::SuccessButtonStyle();
		savePressed = Util::Icons::ButtonWithFlash(saveLabel, Util::Icons::Kind::Save);
	} else {
		savePressed = Util::Icons::ButtonWithFlash(saveLabel, Util::Icons::Kind::Save);
	}
	if (savePressed)
		menu->SaveSettings();
	if (auto _tt = Util::HoverTooltipWrapper())
		ImGui::Text("%s", T("menu.save_settings_tooltip", "Save all settings so they are kept after the game restarts.\nShortcut: Ctrl+S"));

	place(IconLabelButtonWidth(revertLabel), style.ItemSpacing.x);
	if (Util::Icons::Button(revertLabel, Util::Icons::Kind::Reload))
		Util::RequestRestoreSavedSettingsConfirmation();
	if (auto _tt = Util::HoverTooltipWrapper())
		ImGui::Text("%s", T("menu.revert_settings_tooltip", "Discard unsaved changes and reload your saved settings."));

	place(IconLabelButtonWidth(clearCacheLabel), groupGap);
	if (Util::Icons::Button(clearCacheLabel, Util::Icons::Kind::Refresh)) {
		Util::RequestClearShaderCacheConfirmation();
	}
	if (auto _tt = Util::HoverTooltipWrapper()) {
		ImGui::Text("%s", T("menu.clear_shader_cache_tooltip",
							  "Clears the shader cache and disk cache (if enabled). "
							  "The Shader Cache is the collection of compiled shaders which replace the vanilla shaders at runtime. "
							  "The Disk Cache is a collection of compiled shaders on disk. "
							  "Clearing will mean that shaders are recompiled only when the game re-encounters them."));
	}

	place(modholdWidth, groupGap);
	if (Util::Button(modholdLabel))
		showModholdLinks = true;
	if (auto _tt = Util::HoverTooltipWrapper())
		ImGui::Text("%s", T("menu.modhold.tooltip", "Open the Hold Shaders page or SethSaidThat's page on Modhold."));
	DrawModholdLinksPopup();

	const float endY = isDocked ? ImGui::GetCursorScreenPos().y : ImMax(ImGui::GetCursorScreenPos().y, belowTitle);
	ImGui::SetCursorScreenPos(ImVec2(rowOrigin.x, endY));
	ImGui::Dummy(ImVec2(0.0f, 0.0f));

	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ThemeManager::Constants::SEPARATOR_THICKNESS);
	ImGui::Spacing();
}

float MenuHeaderRenderer::GetStatusBarHeight()
{
	MenuFonts::FontRoleGuard fontGuard(Menu::FontRole::Subtext);
	return ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y;
}

void MenuHeaderRenderer::RenderStatusBar()
{
	auto* menu = globals::menu;
	auto shaderCache = globals::shaderCache;
	if (!menu || !shaderCache)
		return;

	MenuFonts::FontRoleGuard fontGuard(Menu::FontRole::Subtext);
	const auto& style = ImGui::GetStyle();
	const float rowStartX = ImGui::GetCursorScreenPos().x;
	const float fullWidth = ImGui::GetContentRegionAvail().x;
	auto* i18n = I18n::GetSingleton();

	float leftEnd = rowStartX;
	ImGui::Dummy(ImVec2(0.0f, ImGui::GetFrameHeight()));
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::AlignTextToFramePadding();
	if (!shaderCache->IsEnabled()) {
		const auto text = i18n->Format("menu.status.effects_off", { { "key", Util::Input::KeyIdToString(menu->GetSettings().EffectToggleKey) } },
			"Effects are turned off. Press {key} to turn them back on.");
		Util::Text::Warning("%s", text.c_str());
		leftEnd = ImGui::GetItemRectMax().x;
	} else if (shaderCache->IsCompiling()) {
		const auto text = i18n->Format("menu.status.compiling",
			{ { "done", std::to_string(shaderCache->GetCompletedTasks()) }, { "total", std::to_string(shaderCache->GetTotalTasks()) } },
			"Compiling shaders {done}/{total}");
		Util::Text::Info("%s", text.c_str());
		leftEnd = ImGui::GetItemRectMax().x;
	} else if (const auto completed = shaderCache->GetCompletedTasks(); completed > 0) {
		const auto text = i18n->Format("menu.status.shaders_ready", { { "count", std::to_string(completed) } }, "{count} shaders ready");
		Util::TextUnformattedDisabled(text.c_str());
		leftEnd = ImGui::GetItemRectMax().x;
	}

	if (const auto failed = shaderCache->GetFailedTasks(); failed > 0) {
		ImGui::SameLine(0.0f, style.ItemSpacing.x * kCommandGroupGapRatio);
		const auto text = i18n->Format("menu.status.shaders_failed", { { "count", std::to_string(failed) } }, "{count} failed");
		Util::Text::Error("%s", text.c_str());
		ImGui::SameLine();
		if (Util::Button(shaderCache->IsHideErrors() ? T("menu.status.show_error", "Show Error") : T("menu.status.hide_error", "Hide Error")))
			shaderCache->ToggleErrorMessages();
		if (auto _tt = Util::HoverTooltipWrapper()) {
			ImGui::Text("%s", T("menu.toggle_error_message_tooltip",
								  "Hide or show the shader failure message. "
								  "Your installation is broken and will likely see errors in game. "
								  "Please double check you have updated all features and that your load order is correct. "
								  "See the log for details."));
		}
		leftEnd = ImGui::GetItemRectMax().x;
	}

	struct Hint
	{
		const char* key;
		const char* action;
	};
	const Hint keyboardHints[] = {
		{ "Ctrl+S", T("menu.status.hint_save", "Save") },
		{ "Ctrl+F", T("menu.status.hint_search", "Search") },
		{ "Esc", T("menu.status.hint_close", "Close") },
	};
	// Hold Shaders: once the controller is used, the hints show its buttons.
	const Hint gamepadHints[] = {
		{ "D-pad/LS", T("menu.status.hint_pad_move", "Move") },
		{ "A", T("menu.status.hint_pad_select", "Select") },
		{ "B", T("menu.status.hint_pad_back", "Back") },
		{ "LB/RB", T("menu.status.hint_pad_page", "Page") },
		{ "RS", T("menu.status.hint_pad_scroll", "Scroll") },
		{ "LS+RS", T("menu.status.hint_pad_close", "Close") },
	};
	const bool gamepad = menu->IsUsingGamepad();
	const Hint* hints = gamepad ? gamepadHints : keyboardHints;
	const size_t hintCount = gamepad ? std::size(gamepadHints) : std::size(keyboardHints);
	const float hintGap = style.ItemSpacing.x * kCommandGroupGapRatio;
	float hintsWidth = 0.0f;
	for (size_t i = 0; i < hintCount; ++i)
		hintsWidth += ImGui::CalcTextSize(hints[i].key).x + style.ItemInnerSpacing.x + ImGui::CalcTextSize(hints[i].action).x + hintGap;
	hintsWidth -= hintGap;

	const float hintsX = rowStartX + fullWidth - hintsWidth;
	if (hintsX < leftEnd + hintGap)
		return;

	ImGui::SameLine();
	ImGui::SetCursorScreenPos(ImVec2(hintsX, ImGui::GetCursorScreenPos().y));
	for (size_t i = 0; i < hintCount; ++i) {
		if (i > 0)
			ImGui::SameLine(0.0f, hintGap);
		ImGui::TextUnformatted(hints[i].key);
		ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
		Util::TextUnformattedDisabled(hints[i].action);
	}
}
