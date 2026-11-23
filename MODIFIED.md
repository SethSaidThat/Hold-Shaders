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
- Added the "Hold Shaders" Baseline preset pack: its colour grading (Post Processing, Color Grading and Tone Mapping)
  is a little darker than stock, with softer highlights and slightly richer colour.
