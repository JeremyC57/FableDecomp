// CCreatureAction_KrakenReturnToWater::FinishAction() -- base FinishAction, advance the owner's boss phase, then clear the kraken's ReturnHeadToWater flag
// Leaf: ?FinishAction@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923EC0, length 160 bytes, vtable slot 4 of 0x012906F4
// (RTTI `?A0x29922355::CCreatureAction_KrakenReturnToWater`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenReturnToWater@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// address kept in edi), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xad = CTCBoss, bit 13 of the bitset dword at
// +0x34, `test ch,20h`; then 0xe2 = CTCKraken, bit 2 of the byte at +0x3c), CTCBoss::IncrementBossPhase @ 0x007AED20.
// Shape: both lookups are UNCHECKED accessors (absent arms read the key word); the final store is duplicated into both arms.
// Layout: CTCKraken ReturnHeadToWater +0x5c (Ego_r.pdb +0x6c).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCBoss { public:                                // interface type 0xad component
    void IncrementBossPhase();                  // 0x007AED20
};
class CTCKraken { public:                              // interface type 0xe2 component
    unsigned char _pad00[0x5c];
    bool          ReturnHeadToWater;            // +0x5c (PDB +0x6c)
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    protected: virtual void FinishAction(); public:                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCBoss* GetBoss() const
    {
        CTCBoss* boss;
        PCreature.GetPItem()->GetInterface(&boss, 0xad);
        return boss;
    }
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenReturnToWater : CCombatActionBase {   // vtable 0x012906F4, sizeof 0x114
    virtual void FinishAction();
};

void CCreatureAction_KrakenReturnToWater::FinishAction()
{
    CCombatActionBase::FinishAction();
    GetBoss()->IncrementBossPhase();
    GetKraken()->ReturnHeadToWater = false;
}