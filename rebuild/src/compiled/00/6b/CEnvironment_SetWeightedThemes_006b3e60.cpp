// CEnvironment::SetWeightedThemes(long channel, long priority, long* themes, float* weights, long count) -- unless the theme is forced, sum the weights per non-zero global theme index into a sorted static list, set each listed theme's weight on the channel's priority entry (the last one rebalancing), then empty the list
// Leaf: ?SetWeightedThemes@CEnvironment@@QAEXJJPAJPAMJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B3E60, 390 bytes, thiscall `ret 14h`. Caller: CWorld::UpdateEnvironmentCameraPos (0x0049E57F).
// Identity: Ego_r.pdb / FableWin.pdb ?SetWeightedThemes@CEnvironment@@QAEXJJPAJPAMJ@Z (Ego_r 0x006EFD20, FableWin 0x02451D40); both PDBs
//   also name the function-static `ThemeWeights` (`CEnvironment::SetWeightedThemes'::`17'::`dynamic atexit destructor for 'ThemeWeights'',
//   retail 0x01229370, landed; the vector at 0x013BA8EC, guard bit 1 of 0x013BA8F8). The catalog's "CEnvironmentThemePriorityStackEntry_SetWeight"
//   (forge autoname) and the landed callers' "SetCameraThemes" are descriptive placeholders.
// Callees: std::lower_bound<pair<long, float>*, pair<long, float> > 0x006B5A50 (__fastcall), vector<pair<long, float> >::insert 0x006B5ED0
//   and ::clear 0x006B5B50, CDefinitionManager::GetDefClassIndexFromGlobalIndex 0x009ACCA0 (through the inline-expanded
//   CEnvironment::GetLocalEnvironmentThemeIndexFromGlobal, out of line at 0x006B2C50), NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::
//   SetThemeWeight(long, float, bool) 0x00886D70, atexit 0x004012BC.
// Layout: CEnvironment (engine/CEnvironment.h): DefinitionManager +0x04, ForcedEnvironmentTheme +0x3E, PriorityStackChannels +0x48 (CArray of
//   12-byte stacks of 48-byte entries), ThemeSet +0x1954 (12-byte day sets: GetCount = (end - begin) / 12).
// Shape: one pair<long, float> key homed at [esp+10h] for the whole first loop (its weight only written on insert); the cached `end` of the
//   second loop is ebx (0x006B3F6F), the visible GetLocalEnvironmentThemeIndexFromGlobal body puts the local index in ecx (0x006B3F92).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CENVIRONMENT_THEMESTACKS
#define FABLE_RETAIL_CENVIRONMENT_THEMEINDEX
#define FABLE_RETAIL_CENVIRONMENT_SETWEIGHTEDTHEMES
#define FABLE_RETAIL_CENVIRONMENTTHEMERESOURCEPALLETTE_GETCOUNT
#define FABLE_RETAIL_STLP_LOWER_BOUND
#define FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CEnvironment.h"

void CEnvironment::SetWeightedThemes(long channel, long priority, long* themes, float* weights, long count)
{
    if (ForcedEnvironmentTheme)
        return;
    if (channel < 0 || channel >= 6)
        return;
    if (priority < 0 || priority >= 5)
        return;
    if (count < 1)
        return;
    static std::vector<std::pair<long, float> > ThemeWeights;
    // Accumulate: one sorted entry per global theme index, weights of repeated themes summed.
    std::pair<long, float> theme_weight(0, 0.0f);
    for (long i = 0; i < count; ++i) {
        long theme = themes[i];
        if (theme != 0) {
            theme_weight.first = theme;
            std::vector<std::pair<long, float> >::iterator it =
                std::lower_bound(ThemeWeights.begin(), ThemeWeights.end(), theme_weight);
            if (it != ThemeWeights.end() && it->first == theme) {
                it->second += weights[i];
            } else {
                theme_weight.second = weights[i];
                ThemeWeights.insert(it, theme_weight);
            }
        }
    }
    // Apply: stop at the first theme this level's theme set does not hold.
    NEnvironmentInternal::CEnvironmentThemePriorityStackEntry& entry = PriorityStackChannels[channel].GetEntry(priority);
    for (std::vector<std::pair<long, float> >::iterator it = ThemeWeights.begin(), end = ThemeWeights.end(); it != end; ++it) {
        long local_theme = GetLocalEnvironmentThemeIndexFromGlobal(it->first) - 1;
        if (local_theme < 0 || local_theme >= ThemeSet.GetCount())
            break;
        entry.SetThemeWeight(local_theme, it->second, it + 1 == end);
    }
    ThemeWeights.clear();
}