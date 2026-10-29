// CCreatureAction_UseObject::PerformAction() -- re-aim the move-to position at the creature, queue a CEventItemUsed(creature) on the used thing's entity events, then hand the use to the thing's scripted hook (CTCActionUseScriptedHook::OnActionUse) when it has one, else to its CTCActionUse component's virtual slot 28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069FE60, length 366 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8, PUsedThing +0xa8), CThing::GetPos @ 0x004C73D0,
// CCreatureActionBase::SetMoveToPos(const C3DVector&) @ 0x006929E0, CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x42
// CTCEntityEvents, bit 2 of +0x28, tested; 0xC2 CTCActionUseScriptedHook, bit 2 of +0x38, tested; 0xD CTCActionUse, bit 13 of +0x20,
// untested), operator new @ 0x00BFEA1A (0x10-byte event), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (the
// event's CIntelligentPointer<CThing>, vtable 0x01238C6C), CTCEntityEvents::AddEvent @ 0x008113C0, CTCActionUseScriptedHook::
// OnActionUse(CThing*) @ 0x00729B20, the CTCActionUse virtual slot 28 (+0x70, one CThing*). NEntityEvents::CEventItemUsed vtable
// 0x0125C820 (RTTI).
// Layout: retail CCreatureActionBase is 0xa8 bytes: PCreature +0x08, PUsedThing +0xa8.
// Shape: the event is {vptr, long 0, CIntelligentPointer<CThing>} with an inline ctor (the OpenChest 0x008448E0 model); &PCreature is
// spilled across the new/ctor block; the `new` null test selects 0 for AddEvent; the absent-events arm still evaluates a
// PUsedThing.GetPItem() whose result is dropped (a compiled-out assertion on the used thing); the scripted-hook branch returns early.
class C3DVector;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentThingPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    __forceinline CIntelligentThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct __declspec(novtable) CEventBase {
    virtual void v00();
    long m_data;                                // +0x04
    CEventBase() : m_data(0) {}
};
namespace NEntityEvents {
class CEventItemUsed : public CEventBase { public:            // 0x10 bytes, vtable 0x0125C820
    CIntelligentThingPointer User;              // +0x08
    CEventItemUsed(CThing* user) : User(user) {}
};
}
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);           // 0x008113C0
};
class CTCActionUseScriptedHook { public:               // interface type 0xC2
    void OnActionUse(CThing* user);             // 0x00729B20
};
struct CTCActionUse {                           // interface type 0xD
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    virtual void OnUsed(CThing* user);          // slot 28 (+0x70)
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    void SetMoveToPos(const C3DVector& pos);    // 0x006929E0
};
class CCreatureAction_UseObject : public CCreatureActionBase { public:
    CBaseIntelligentPointer PUsedThing;         // +0xa8
    void PerformAction();
};

void CCreatureAction_UseObject::PerformAction()
{
    SetMoveToPos(PCreature.GetPItem()->GetPos());
    CThing* used = PUsedThing.GetPItem();
    if (used->HasTC(0x42)) {
        CTCEntityEvents* events = used->GetTC<CTCEntityEvents>(0x42);
        events->AddEvent(new NEntityEvents::CEventItemUsed(PCreature.GetPItem()));
    }
    else
        PUsedThing.GetPItem();
    CThing* usedThing = PUsedThing.GetPItem();
    if (usedThing->HasTC(0xc2)) {
        usedThing->GetTC<CTCActionUseScriptedHook>(0xc2)->OnActionUse(PCreature.GetPItem());
        return;
    }
    CTCActionUse* actionUse;
    PUsedThing.GetPItem()->GetInterface(&actionUse, 0xd);
    actionUse->OnUsed(PCreature.GetPItem());
}