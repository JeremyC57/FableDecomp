// CPlayerGui::SetSpecialAbilityButtonsAsDisplaying(bool show, bool show_abilities) -- start the normal <-> special button transition when the state changes (not while paused)
// Leaf: ?SetSpecialAbilityButtonsAsDisplaying@CPlayerGui@@QAEX_N0@Z
// Retail: Fable.exe @ 0x00437580, 81 bytes, `ret 8`; callers 0x0062608E, 0x006284F1, 0x0068FF6B, 0x0068FF98, 0x0069000E;
// Ego_r.pdb ?SetSpecialAbilityButtonsAsDisplaying@CPlayerGui@@QAEX_N0@Z.
// Body: nothing while World->Paused or when both flags already match; else store the abilities flag, the show flag,
// NormalToSpecial = show, SpecialToNormal = !show, AbilityButtonScale = 0.1f.
// Layout: retail CPlayerGui -- World +0x04, ShowSpecialAbilityButtons +0x250, ShowSpecialAbilityButtonsAbilities +0x251,
// AbilityButtonScale +0x278, NormalToSpecial +0x27c, SpecialToNormal +0x27d (Ego_r.pdb +0x258.. with 16-byte vectors; the
// three vectors before AbilityButtonScale are 12 bytes in retail); CWorld from engine/CWorld.h (Paused +0xa4).
// Shape: /O2 (16-aligned entry).
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