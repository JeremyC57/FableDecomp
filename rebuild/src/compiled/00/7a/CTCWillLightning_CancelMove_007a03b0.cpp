// CTCWillLightning::CancelMove() -- cancel the lightning power: deactivate it unless the caster's special-abilities component is busy with a different ability
// Leaf: ?CancelMove@CTCWillLightning@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A03B0, 75 bytes (+ int3 padding to 80), __fastcall, plain `ret`.
// Identity: the code sits in CTCWillLightning's block (vtable 0x0126C724 slots at 0x007A0D70..0x007A4090) and calls the
//   state-5 transition 0x007A0310 (Deactivate: m_fTimeBase = m_fTime, state 5, walk the beam list); its one caller
//   0x0072B651 fetches interface 0x94 (CTCWillLightning) from the hero first. Ego_r.pdb `?CancelMove@CTCWillLightning@@QAEXXZ`
//   is the only public void() member besides the sound helpers. The catalog's "LightningAbility_Deactivate_IfActive" is
//   descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline GetInterface: 0xA7 = CTCSpecialAbilities,
//   bit 7 of the bitset dword +0x34, which VC tests as the sign of the low byte), CTCWillLightning::Deactivate 0x007A0310.
// Layout: CTCSpecialAbilities CurrentAbility +0x10 (EHeroAbility; 0xB = this lightning ability, as the landed
//   CCreatureAction_BattleChargeImpact::FinishAction compares 12 for the battle charge).
// Shape: `if (!has-0xA7 || abilities->CurrentAbility == 0xB) Deactivate();` -- the not-present branch jumps straight to
//   the call; the present branch falls into it when the ability matches.
// Label correction: catalogued as `LightningAbility_Deactivate_IfActive`; the body is CTCWillLightning::CancelMove.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                   // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                          // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCSpecialAbilities { public:                             // interface 0xA7
    unsigned char _pad00[0x0c];
    bool          AbilityMutexLock;                      // +0x0c
    long          CurrentAbility;                        // +0x10 EHeroAbility
};

class CTCWillLightning {
public:
    virtual ~CTCWillLightning();
    CThing* ParentThing;                                 // +0x04 (CTCBase)

    void Deactivate();                                   // 0x007A0310
    void CancelMove();
};

void CTCWillLightning::CancelMove()
{
    CTCSpecialAbilities* abilities;
    if (!ParentThing->GetInterface(&abilities, 0xa7) || abilities->CurrentAbility == 0xb)
        Deactivate();
}