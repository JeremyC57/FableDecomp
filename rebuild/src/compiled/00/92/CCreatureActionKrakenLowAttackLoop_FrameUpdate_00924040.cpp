// CCreatureAction_KrakenLowAttackLoop::FrameUpdate() -- unless already interrupted, finish the action (virtual FinishAction) once the kraken reports its target hit; then the base FrameUpdate
// Leaf: ?FrameUpdate@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0000924040, length 117 bytes (+ int3 padding), vtable slot 56 of 0x01290C64
// (RTTI `?A0x29922355::CCreatureAction_KrakenLowAttackLoop`; FableWin.pdb ?FrameUpdate@CCreatureAction_KrakenLowAttackLoop@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0xe2 = CTCKraken, bit 2 of the bitset byte at +0x3c), CTCKraken::HasTargetBeenHitByKraken @ 0x007FB500, the
// action's virtual FinishAction (slot 4, +0x10), CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call, last).
// Shape: Interrupt is tested before the lookup and re-read after the bool call (the nested `if (!Interrupt)` guard around
// FinishAction); the UNCHECKED lookup sits in an inline accessor (absent arm reads the key word).
// Layout: Ego_r.pdb Interrupt / LoopNum +0x12c / +0x130, retail +0x114 / +0x118 (landed ctor 0x009255E0).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
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
class CTCKraken { public:                              // interface type 0xe2 component
    bool HasTargetBeenHitByKraken();            // 0x007FB500
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void FinishAction();                // slot 4 (+0x10)
    virtual void FrameUpdate();                 // 0x00859D80 (slot 56)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenLowAttackLoop : CCombatActionBase {   // vtable 0x01290C64
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    virtual void FrameUpdate();
};

void CCreatureAction_KrakenLowAttackLoop::FrameUpdate()
{
    if (!Interrupt && GetKraken()->HasTargetBeenHitByKraken()) {
        if (!Interrupt)
            FinishAction();
    }
    CCombatActionBase::FrameUpdate();
}