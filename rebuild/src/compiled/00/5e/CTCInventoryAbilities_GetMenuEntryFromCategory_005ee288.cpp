// CTCInventoryAbilities::GetMenuEntryFromCategory(EUISpellCategory, CComponent*) const -- finds the attack / surround / physical spells menu entry (PC or console name) under a list
// Leaf: GetMenuEntryFromCategory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE288, 149 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 8`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (built in the argument area), the list's
//          NUISystem::CComponent vtable slot 3 (+0x0C, find child by name, CCharString by value).
// Globals: the player GUI @ 0x013B8790 (+0x2BE: LiveGuiMode, PDB +0x2CE); the six entry names.
// Shape: `switch (category)` over 0..2 in source order (`xor ecx,ecx ; sub eax,ecx ; je ; dec ; je ; dec ; je`,
// default `return 0`); each case is `if (pc) return list->FindChild(CCharString("PC_...", -1)); return
// list->FindChild(CCharString("UI_...", -1));`: VC hoists the identical lead (list and vtable loads, `push ecx ; mov
// ecx,esp ; push -1`) above the branch and merges all six ctor/call tails (a ternary on the literal, or on two
// CCharString temporaries, compiles differently). The zero register also serves the flag compare.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();
};

struct CComponent {                                  // NUISystem::CComponent
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name); // slot 3 (+0x0C)
};

class CPlayerGui { public:
    unsigned char _pad_0x000[0x2BE];
    bool          LiveGuiMode;                 // +0x2BE
};

extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

enum EUISpellCategory {
    UI_SPELL_CATEGORY_ATTACK   = 0,
    UI_SPELL_CATEGORY_SURROUND = 1,
    UI_SPELL_CATEGORY_PHYSICAL = 2
};

class CTCInventoryAbilities { public:
    void* __vftable;                                 // +0x00
    void* ParentThing;                               // +0x04 CTCBase

    CComponent* GetMenuEntryFromCategory(EUISpellCategory category, CComponent* list) const;
};

CComponent* CTCInventoryAbilities::GetMenuEntryFromCategory(EUISpellCategory category, CComponent* list) const
{
    switch (category) {
    case UI_SPELL_CATEGORY_ATTACK:
        if (g_pPlayerGui->LiveGuiMode)
            return list->FindChild(CCharString("PC_MENU_ENTRY_ATTACK_SPELLS", -1));
        return list->FindChild(CCharString("UI_MENU_ENTRY_ATTACK_SPELLS", -1));
    case UI_SPELL_CATEGORY_SURROUND:
        if (g_pPlayerGui->LiveGuiMode)
            return list->FindChild(CCharString("PC_MENU_ENTRY_SURROUND_SPELLS", -1));
        return list->FindChild(CCharString("UI_MENU_ENTRY_SURROUND_SPELLS", -1));
    case UI_SPELL_CATEGORY_PHYSICAL:
        if (g_pPlayerGui->LiveGuiMode)
            return list->FindChild(CCharString("PC_MENU_ENTRY_PHYSICAL_SPELLS", -1));
        return list->FindChild(CCharString("UI_MENU_ENTRY_PHYSICAL_SPELLS", -1));
    }
    return 0;
}