#include <cstdio>
#include "engine/CWorld.h"
class CPlayerGui {
public:
    void*         _vfptr;                                // +0x000
    CWorld*       World;                                 // +0x004
    unsigned char _pad008[0x250 - 0x8];
    bool          ShowSpecialAbilityButtons;             // +0x250
    bool          ShowSpecialAbilityButtonsAbilities;    // +0x251
    bool          SpecialSelected;                       // +0x252
    bool          SpecialDeselected;                     // +0x253
    unsigned char _pad254[0x278 - 0x254];                // AbilityButtons / AbilityButtonsAlpha / QuickAccessButtonsAlpha (12-byte STLport vectors)
    float         AbilityButtonScale;                    // +0x278
    bool          NormalToSpecial;                       // +0x27c
    bool          SpecialToNormal;                       // +0x27d
    void SetSpecialAbilityButtonsAsDisplaying(bool show, bool show_abilities);
};
void CPlayerGui::SetSpecialAbilityButtonsAsDisplaying(bool show, bool show_abilities)
{
    if (World->Paused)
        return;
    if (ShowSpecialAbilityButtons == show && ShowSpecialAbilityButtonsAbilities == show_abilities)
        return;
    ShowSpecialAbilityButtonsAbilities = show_abilities;
    ShowSpecialAbilityButtons = show;
    NormalToSpecial = show;
    SpecialToNormal = !show;
    AbilityButtonScale = 0.1f;
}


#include <cstring>
int main()
{
    static unsigned char wraw[sizeof(CWorld)]; std::memset(wraw, 0, sizeof wraw); CWorld* w = (CWorld*)wraw;
    static CPlayerGui g; std::memset(&g, 0, sizeof g); g.World = w;
    g.SetSpecialAbilityButtonsAsDisplaying(true, false);
    bool ok = g.ShowSpecialAbilityButtons && !g.ShowSpecialAbilityButtonsAbilities && g.NormalToSpecial && !g.SpecialToNormal && g.AbilityButtonScale == 0.1f;
    g.AbilityButtonScale = 1.0f; g.SetSpecialAbilityButtonsAsDisplaying(true, false); ok = ok && g.AbilityButtonScale == 1.0f;
    g.SetSpecialAbilityButtonsAsDisplaying(false, true);
    ok = ok && !g.ShowSpecialAbilityButtons && g.ShowSpecialAbilityButtonsAbilities && !g.NormalToSpecial && g.SpecialToNormal && g.AbilityButtonScale == 0.1f;
    w->Paused = true; g.AbilityButtonScale = 2.0f; g.SetSpecialAbilityButtonsAsDisplaying(true, true); ok = ok && g.AbilityButtonScale == 2.0f && !g.ShowSpecialAbilityButtons;
    if (ok) std::printf("00437580_TEST PASS\n");
    return 0;
}