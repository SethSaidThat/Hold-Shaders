# Licenses and credits

Community Shaders is GPL-3.0 (see COPYING.txt) WITH the Modding Exception and GPL-3.0 Linking Exception (see EXCEPTIONS.md), except for the third-party code below. A LICENSE file or header applies only to the code it names, not to the whole directory or to later modifications.

## Third-party code

| Code                                                                                                                                  | Author          | License    | Notice                                                                             |
| ------------------------------------------------------------------------------------------------------------------------------------- | --------------- | ---------- | ---------------------------------------------------------------------------------- |
| `Shaders/BloodSplatter.hlsl` (formatting changes only)                                                                                | Ilya Perapechka | MIT        | [LICENSE-Perapechka-MIT.txt](LICENSE-Perapechka-MIT.txt)                           |
| Original code in `Shaders/{DistantTree,Effect,Lighting,Particle,RunGrass,Sky,Water}.hlsl` (modifications are GPL-3.0 with exceptions) | Ilya Perapechka | MIT        | [LICENSE-Perapechka-MIT.txt](LICENSE-Perapechka-MIT.txt), SPDX header in each file |
| `Shaders/Common/Spherical Harmonics/SphericalHarmonics.hlsli`                                                                         | SebH            | MIT        | [LICENSE-SebH-MIT.txt](LICENSE-SebH-MIT.txt)                                       |
| `Shaders/Common/FastMath.hlsli`                                                                                                       | Michal Drobot   | MIT        | Header in file                                                                     |
| `Shaders/Common/Color.hlsli` (color conversion section)                                                                               | Google LLC      | Apache-2.0 | Header in file                                                                     |
| `Shaders/Common/Color.hlsli` (Luma Framework adaptation)                                                                              | Filippo Tarpini | MIT        | Comment in file                                                                    |
| Night Eye (`src/Features/NightEye.*`, `Shaders/NightEye/*` including the eyeshine masks), ported from Truly Night Eye 1.0              | Aswordmaster    | GPL-3.0-or-later with the Modding and Linking exceptions | SPDX header in each file                                       |

## Third-party binaries and fonts shipped in Hold Shaders builds

| What                                                                                                                  | Author                     | License                                          | Notice                                                             |
| --------------------------------------------------------------------------------------------------------------------- | -------------------------- | ------------------------------------------------ | ------------------------------------------------------------------ |
| `amd_fidelityfx_framegeneration_dx12.dll`, `amd_fidelityfx_loader_dx12.dll`, `amd_fidelityfx_upscaler_dx12.dll` (AMD signed binaries, FidelityFX SDK commit 60f4ea8) | Advanced Micro Devices, Inc. | AMD FidelityFX SDK licence (binary redistribution) | [LICENSE-AMD-FidelityFX-SDK.txt](LICENSE-AMD-FidelityFX-SDK.txt) |
| Font Awesome Free icon font (`fa-solid-900.ttf`, built into the DLL)                                                  | Font Awesome (fontawesome.com) | SIL OFL 1.1 (font), CC BY 4.0 (icons), MIT (code) | [Font-Awesome-LICENSE.txt](Font-Awesome-LICENSE.txt)               |
| Game-Icons icon font (`game-icons.ttf`, built into the DLL)                                                           | game-icons.net contributors | CC BY 3.0                                        | [Game-Icons-LICENSE.txt](Game-Icons-LICENSE.txt)                   |
| Tabler Icons icon font (`tabler-icons.ttf`, built into the DLL)                                                       | Paweł Kuna                 | MIT                                              | [Tabler-Icons-LICENSE.txt](Tabler-Icons-LICENSE.txt)               |

## Inspired by

| Community Shaders code                           | Mod                                                                                         | Author |
| ------------------------------------------------ | ------------------------------------------------------------------------------------------- | ------ |
| `src/EngineFixes/SunOcclusionPollFix.cpp`        | [Hitch Hunter and Fixes - SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/194231) | Zzyxzz |
| `src/EngineFixes/ShadowmapCascadeCullingFix.cpp` | [Shadow Fixes - SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/195345)           | Zzyxzz |
