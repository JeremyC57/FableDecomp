// NEnvironmentInternal::CEnvironmentThemePriorityStack::CalculateTheme(CBlendedEnvironmentTheme& theme, float time, const CEnvironmentThemeResourcePallette& pallette) -- take the bottom entry's level theme, then blend in every higher-priority entry whose base weight leaves room (1 - weight above 1/1024)
// Leaf: ?CalculateTheme@CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAEXAAVCBlendedEnvironmentTheme@2@MABVCEnvironmentThemeResourcePallette@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008889C0, 150 bytes (+ int3 padding).
// Callees: NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::GetLevelTheme(CBlendedEnvironmentTheme&, float,
//   const CEnvironmentThemeResourcePallette&) @ 0x008884D0, ...::BlendInLevel(CBlendedEnvironmentTheme&, const CBlendedEnvironmentTheme&,
//   float, const CEnvironmentThemeResourcePallette&) const @ 0x008886E0 (called with the same theme as source and destination).
//   Float pool: 0x0122DED8 = 1.0f, 0x01278080 = 0.0009765625f (1/1024).
// Names (agentNF25b, 2026-11-12): moved onto engine/CEnvironmentThemePriorityStack.h; the row's own symbol and both callees take the
//   Ego_r.pdb / FableWin.pdb spellings (NEnvironmentInternal namespace, reference parameters, BlendInLevel const). The earlier local
//   model bound ?GetLevelTheme@CEnvironmentThemePriorityStackEntry@@QAEXPAU... and ?BlendInLevel@CEnvironmentThemePriorityStackEntry@@QAEXPAU...
// Layout: Ents is the retail 12-byte CArray (STLport vector) of 48-byte entries, BaseWeight at +0 of each entry.
// Shape: the entry count `(end - begin) / 48` (0x2AAAAAAB, sar 3) is re-derived each pass, the loop starts at 1 with a
//   strength-reduced byte offset (ebp += 0x30); `1.0f - BaseWeight` is compared with `test ah,41h ; jne` (`>`).
#define FABLE_RETAIL_CENVIRONMENTTHEMEPRIORITYSTACKENTRY_STACK_FRIEND   // BaseWeight read (hdr overlay)
#include "engine/CEnvironmentThemePriorityStack.h"

void NEnvironmentInternal::CEnvironmentThemePriorityStack::CalculateTheme(CBlendedEnvironmentTheme& theme, float time, const CEnvironmentThemeResourcePallette& pallette)
{
    Ents[0].GetLevelTheme(theme, time, pallette);
    for (unsigned long i = 1; i < Ents.size(); ++i) {
        CEnvironmentThemePriorityStackEntry& entry = Ents[i];
        if (1.0f - entry.BaseWeight > 0.0009765625f)
            entry.BlendInLevel(theme, theme, time, pallette);
    }
}
