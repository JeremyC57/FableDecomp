// Grade: DIFFER(1058v1045)/163 lines (unchanged) -- callee names corrected from reloc_bindings + Ego_r (0x006A1440 CTCCarrying::GetThingInPrimarySlot const, 0x005DAB10 CTCWeapon::IsUsableBy(const CThing&) const, 0x0040DCE0 IS_THING_ALIVE<CThing*>, 0x00487DD0 CPlayer::PeekPControlledCreature const -> const CThingPlayerCreature*, 0x004874D0 CPlayer::IsFirstPersonTargeting const) and the retail IS_THING_ALIVE body (0x0043DBF0) same-TU noinline instead of the invented IsThingAliveImpl stand-in; residue unchanged: retail spills the weapons out-pointer and &hero->m_present ([ebp-8]) and keeps hero in ebx (retry wave 2 sec2c)
// Was: DIFFER(1058v1045) -- retail spills the component out-pointers (weapons [ebp-0x14], spells [ebp-0x20]) and the hero presence-bits pointer [ebp-8] and keeps the hero in ebx; ours enregisters the weapons component (frame-slot / register-priority miss)
// NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities() -- set the weapon/spell ability icons for each button entry from the hero melee/ranged weapon, carried state and active/queued spell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064B1AC, length 1045 bytes (/Os frame-pointer TU).
// Tried: landed DrawAbilityList TU model (/Os, GetInterface idiom), a present-bits alias (folded, no change).
// Leaf: UpdateWeaponAbilities
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stdlib.h>
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                       // +0x91
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    template <class T> __forceinline bool GetInterface(T** ppOut, long type) const
    {
        if (m_present.test(type)) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingPlayerCreature : CThing { };
// IS_THING_ALIVE<const CThingPlayerCreature*> 0x0043DBF0: the retail body (out of line in this /Os TU), same-TU noinline
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}
struct CTCBase;
struct CPlayer {
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
    bool IsFirstPersonTargeting() const;           // 0x004874D0 (Ego_r ?IsFirstPersonTargeting@CPlayer@@QBE_NXZ)
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct CIntelligentPointer {
    void* m_a; void* m_b;
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CTCWeapons {                                // interface 0x13
    unsigned char _pad[0x134];
    CIntelligentPointer PMeleeWeapon;              // +0x134
    unsigned char _pad13c[0x141 - 0x13c];
    CIntelligentPointer PRangedWeapon;             // +0x141
    bool HasMeleeWeapon();                         // 0x005C2DED
    bool HasRangedWeapon();                        // 0x005C2DF5
};
struct CTCCombatFlag { unsigned char _pad[0xc]; bool Enabled; };   // interface 0x29
struct CTCCarrying { CThing* GetThingInPrimarySlot() const; };   // interface 0x46, 0x006A1440 (Ego_r ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ)
struct CTCWeapon { bool IsUsableBy(const CThing& thing) const; };   // interface 0x48, 0x005DAB10 (Ego_r ?IsUsableBy@CTCWeapon@@QBE_NABVCThing@@@Z)
struct CTCMode { unsigned char _pad[0x14]; bool Active; };   // interface 0x81
struct CRefObject { virtual void v00(); virtual void Destroy(); long RefCount; unsigned char _pad[0x4c - 8]; long Icon; };
struct CRefPointer {
    CRefObject* Object;
};
struct CTCSpells {                                 // interface 0x30
    bool HasActiveSpell();                         // 0x00733EE0
    void GetActiveSpell(CRefPointer* out);         // 0x00733480
    bool HasQueuedSpell();                         // 0x00733290
    void GetQueuedSpell(CRefPointer* out);         // 0x007334B0
};
struct CButtonMap {
    bool IsActionOnButton(long action, long type); // 0x00446B40
    bool IsActionOnAltButton(long action, long type);   // 0x00446B90
};
struct CAbilityEntry { unsigned char _pad[0x40]; long Action; long AltAction; unsigned char _pad48[0x5c - 0x48]; };
struct CGuiData {
    unsigned char _pad[0x764];
    long IconRangedOn;                             // +0x764
    long IconMelee;                                // +0x768
    long IconMeleeWeapon;                          // +0x76c
    long IconRangedWeapon;                         // +0x770
    unsigned char _pad774[0x7c0 - 0x774];
    long IconSpellNone;                            // +0x7c0
    long IconSpellMode;                            // +0x7c4
    long IconSpellAlt;                             // +0x7c8
    unsigned char _pad7cc[0x7fc - 0x7cc];
    CAbilityEntry* EntriesStart;                   // +0x7fc
    CAbilityEntry* EntriesFinish;                  // +0x800
    unsigned long EntryCount() const { return EntriesFinish - EntriesStart; }
};
CGuiData* GetGuiData();                            // 0x00438570
struct CPlayerGui {
    unsigned char _pad[0xc];
    CButtonMap* Buttons;                           // +0x0c
    void SetSpellIcon(long icon);                  // 0x00437650
    bool IsAltMode();                              // 0x00437640
    void SetAbilityIcon(unsigned long index, long icon);   // 0x00438A20
};

namespace NPlayerGui {
struct CDrawAbilityButtons {
    CPlayerGui*     PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    virtual void v00();
    void UpdateWeaponAbilities();
};

void CDrawAbilityButtons::UpdateWeaponAbilities()
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer()->PeekPControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    CTCSpells* spells;
    hero->GetInterface(&spells, 0x30);
    bool melee = false;
    bool ranged = false;
    bool rangedOn = false;
    CTCWeapons* weapons;
    hero->GetInterface(&weapons, 0x13);
    CTCCombatFlag* combat;
    if (hero->GetInterface(&combat, 0x29) && combat->Enabled && weapons) {
        if (weapons->HasMeleeWeapon())
            melee = IS_THING_ALIVE(weapons->PMeleeWeapon.GetPItem());
        if (weapons->HasRangedWeapon())
            ranged = IS_THING_ALIVE(weapons->PRangedWeapon.GetPItem());
    }
    CTCCarrying* carrying;
    if (hero->GetInterface(&carrying, 0x46)) {
        CThing* primary = carrying->GetThingInPrimarySlot();
        if (primary == 0)
            rangedOn = true;
        else {
            CTCWeapon* weapon;
            if (primary->GetInterface(&weapon, 0x48) && weapon->IsUsableBy(*hero))
                rangedOn = true;
        }
    }
    bool unarmed = !melee && !ranged;
    if (hero->m_present.test(0x84))
        rangedOn = false;
    CGuiData* data = GetGuiData();
    CButtonMap* buttons = PlayerGui->Buttons;
    for (unsigned long i = 0; i < data->EntryCount(); ++i) {
        if (buttons->IsActionOnButton(data->EntriesStart[i].Action, 9) || buttons->IsActionOnAltButton(data->EntriesStart[i].AltAction, 9)) {
            if (melee)
                PlayerGui->SetAbilityIcon(i, data->IconMeleeWeapon);
            else if (ranged)
                PlayerGui->SetAbilityIcon(i, data->IconRangedWeapon);
            else if (unarmed)
                PlayerGui->SetAbilityIcon(i, data->IconMelee);
        } else if (buttons->IsActionOnButton(data->EntriesStart[i].Action, 7) || buttons->IsActionOnAltButton(data->EntriesStart[i].AltAction, 7)) {
            if (rangedOn)
                PlayerGui->SetAbilityIcon(i, data->IconRangedOn);
            else
                PlayerGui->SetAbilityIcon(i, 0);
        } else if (buttons->IsActionOnButton(data->EntriesStart[i].Action, 8) || buttons->IsActionOnAltButton(data->EntriesStart[i].AltAction, 8)) {
            if (spells->HasActiveSpell()) {
                CRefPointer spell;
                spells->GetActiveSpell(&spell);
                long icon = spell.Object->Icon;
                if (--spell.Object->RefCount == 0)
                    spell.Object->Destroy();
                spell.Object = 0;
                PlayerGui->SetSpellIcon(icon);
                PlayerGui->SetAbilityIcon(i, 0);
            } else if (spells->HasQueuedSpell()) {
                CRefPointer spell;
                spells->GetQueuedSpell(&spell);
                long icon = spell.Object->Icon;
                if (--spell.Object->RefCount == 0)
                    spell.Object->Destroy();
                spell.Object = 0;
                PlayerGui->SetSpellIcon(icon);
                PlayerGui->SetAbilityIcon(i, 0);
            } else {
                CTCMode* mode;
                if (hero->GetInterface(&mode, 0x81) && mode->Active && !PlayerManager->GetMainPlayer()->IsFirstPersonTargeting())
                    PlayerGui->SetAbilityIcon(i, data->IconSpellMode);
                else if (PlayerGui->IsAltMode())
                    PlayerGui->SetAbilityIcon(i, data->IconSpellAlt);
                else
                    PlayerGui->SetAbilityIcon(i, data->IconSpellNone);
            }
        }
    }
}
}
