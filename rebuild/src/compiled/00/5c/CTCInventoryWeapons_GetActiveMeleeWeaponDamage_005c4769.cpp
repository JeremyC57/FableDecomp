// CTCInventoryWeapons::GetActiveMeleeWeaponDamage() const -- the active melee weapon's damage while that weapon thing is alive, else 0
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
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

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;            // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || wanted < it->first)
                it = map->last;
            *out = (T*)((void*)it->second);
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
class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
struct CIntelligentPointer {
    unsigned char _body[8];
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
        damage = ((CThing*)((CBaseIntelligentPointer*)&PActiveMeleeWeapon)->GetPItem())->GetTC<CTCWeapon>(0x48)->GetDamage();
    return damage;
}