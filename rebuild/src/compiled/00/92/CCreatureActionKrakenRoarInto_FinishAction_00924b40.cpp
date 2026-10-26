// CCreatureAction_KrakenRoarInto::FinishAction() -- the creature-action base FinishAction (skipping CCombatActionBase's), then make the roar loop (loop 0) the owner's current action
// Leaf: ?FinishAction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924B40, length 72 bytes (+ int3 padding), vtable slot 4 of 0x0129133C
// (RTTI `?A0x29922355::CCreatureAction_KrakenRoarInto`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenRoarInto@?A0xe2035276@@UAEXXZ).
// Callees: CCreatureActionBase::FinishAction @ 0x00694430 (first; a qualified call to the grand-base), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PCreature at +8, twice), CCreatureAction_KrakenRoarLoop::CCreatureAction_KrakenRoarLoop(CThingCreatureBase&, long)
// @ 0x00925920 (FableWin.pdb ??0...@QAE@AAVCThingCreatureBase@@J@Z), CThingCreatureBase::SetCurrentAction @ 0x006644F0,
// ~CCombatActionBase @ 0x0062E900.
// Shape: the 0x118-byte roar loop temporary sits at the frame base; the owner for the call's `this` is fetched first (edi).
class CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase;
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CCreatureActionBase { public:                    // retail sizeof 0xa8 (Ego_r.pdb 0xb4)
    virtual void v00();
    virtual void FinishAction();                // 0x00694430 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    CIntelligentPointer<CThing> PTarget;            // +0xa8 CIntelligentPointer<CThing>
    unsigned char           _padb0[0x114 - 0xb0];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_KrakenRoarLoop : CCombatActionBase {   // vtable 0x01291494
    long LoopNum;                               // +0x114 (PDB +0x12c)
    CCreatureAction_KrakenRoarLoop(CThingCreatureBase& creature, long loopNum);   // 0x00925920
};
struct CCreatureAction_KrakenRoarInto : CCombatActionBase {   // vtable 0x0129133C
    virtual void FinishAction();
};

void CCreatureAction_KrakenRoarInto::FinishAction()
{
    CCreatureActionBase::FinishAction();
    Creature().SetCurrentAction(CCreatureAction_KrakenRoarLoop(Creature(), 0));
}