// CTCInventoryWeapons::GetActiveMeleeWeaponDamage() const -- the active melee weapon's damage while that weapon thing is alive, else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C4769, length 108 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on)
// (`fldz ; fstp` zero). Ego_r.pdb: ?GetActiveMeleeWeaponDamage@CTCInventoryWeapons@@QBEMXZ.
// Callees: IS_THING_ALIVE<CIntelligentPointer> @ 0x004CC340 (`__fastcall`), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound @ 0x004365B0 (the inline CThing::GetInterface for interface 0x48,
// the weapon component), the weapon's damage getter @ 0x005DAAA0 (manifest "CTCWeapon::GetAnimationSpeedValue"; name descriptive).
// Shape: the zero result, the GetInterface key and the accessor's unset component (read back on the miss path) all share the
// one frame word [ebp-4]; the hit path assigns the result, which stays in st(0) for the single `return damage`.
// Layout: PActiveMeleeWeapon at retail +0x134 (as the landed RefreshActiveWeaponData 0x005C30D3).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type) const;            // 0x004365B0
};
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component;
        GetInterface(&component, wanted);
        return component;
    }
};
struct CIntelligentPointer {
    unsigned char _body[8];
    CThing* GetPItem() const;                 // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer* pointer);   // 0x004CC340
class CTCWeapon { public:                            // interface type 0x48
    float GetDamage() const;                  // 0x005DAAA0
};

class CTCInventoryWeapons { public:
    unsigned char _pad00[0x134];
    CIntelligentPointer PActiveMeleeWeapon;   // +0x134
    float GetActiveMeleeWeaponDamage() const;
};

float CTCInventoryWeapons::GetActiveMeleeWeaponDamage() const
{
    float damage = 0.0f;
    if (IS_THING_ALIVE(&PActiveMeleeWeapon))
        damage = PActiveMeleeWeapon.GetPItem()->GetTC<CTCWeapon>(0x48)->GetDamage();
    return damage;
}