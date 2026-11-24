# Changes in Hold Shaders

Hold Shaders is a modified version of Bottled Shaders (https://github.com/InTheBottle/Bottled-Shaders), branch
`Bottle-Compendium` at commit `a2ebc86` (23 Nov 2026), which is a modified version of Community Shaders
(https://github.com/community-shaders/skyrim-community-shaders). This file is the notice required by section 5(a) of the
GNU General Public License v3: it says what was changed, by whom, and when.

## 23 Nov 2026, SethSaidThat (Modhold)

- The product name shown to players is "Hold Shaders" (menu title, Skyrim's own settings menu entry, messages,
  translations). Runtime identifiers are unchanged: the DLL name, the `CommunityShaders` folders and settings files, and
  the exported API (`BottledShaders_*` wetness functions included).
- The log file is `HoldShaders.log`.
- The Bottled Shaders bottle logo is no longer drawn; a Hold Shaders diamond mark (`Util::Icons::DrawHoldMark`) is drawn
  in its place.
- The credits for Bottled Shaders and Community Shaders are shown when hovering the menu title and are kept in the DLL
  (`Plugin::CREDITS`).
- The release archive is named `HoldShaders_AIO`.
- The upstream Nexus upload workflow and the Discord notification workflow are removed (and the Nexus dry-run job in release-build.yaml), so nothing posts to Nexus or an upstream Discord from this fork.
- Licences for the AMD FidelityFX runtime DLLs and the three icon fonts built into the DLL now ship in `SKSE/Plugins/CommunityShaders/Licenses` (THIRD_PARTY.md lists them).
- The credits line ends with the no-warranty notice and the source link.
- GitHub release and pull-request build names say Hold Shaders.
- README: the upstream badges (they link the upstream repository, issues and Discord) are removed so Hold Shaders users are not sent to the upstream teams; the licence link points at COPYING.txt.
- The menu works with a controller: D-pad or left stick moves, A selects, B backs out of a field, popup or page and then
  closes (it acts as Escape), LB / RB step to the previous / next feature page, the right stick scrolls, the triggers
  adjust values slower / faster, and pressing both sticks in opens and closes the menu (that combo is kept from the game).
  While the menu is open the controller no longer reaches the game. The status bar shows the controller buttons once the
  controller is used.
- The menu header has a Modhold button that asks first, then opens the Hold Shaders Modhold page or SethSaidThat's
  Modhold page in the browser.
- Added the "Hold Shaders" Baseline preset pack: its colour grading (Post Processing, Color Grading and Tone Mapping)
  is a little darker than stock, with softer highlights and slightly richer colour.
- Lighter first launch (23 Nov 2026, after a player's desktop froze at the main menu on the first launch): the startup
  shader compile uses half the logical CPU threads minus one (was all but two), background compiling a quarter of the
  performance cores (was half), and the one-time Unified Water cache build a quarter of the threads, at most 4, at
  below-normal priority (was all but four at normal priority). A player's own "Compiler Threads" settings still win.
- The Performance Overlay (FPS graph, draw calls, frame time) starts hidden on a fresh install (`ShowInOverlay` defaults
  to false); players open it with the overlay key (F10) or from the menu. A saved choice still wins.
