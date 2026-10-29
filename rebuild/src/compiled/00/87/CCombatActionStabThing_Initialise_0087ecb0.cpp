// CCombatAction_StabThing::Initialise() -- when the stab target is alive, post a CEventFiredShot(attacker) to its entity events, set flag 0x20 on its +0x60 object, then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087ECB0, 185 bytes (192 with padding), virtual Initialise.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type
//   0x42 = CTCEntityEvents, bit 2 of +0x28), operator new 0x00BFEA1A (0x10 bytes), CEventFiredShot::CEventFiredShot(CThing*)
//   0x0087FED0, CTCEntityEvents::AddEvent 0x008113C0, the +0x60 object's virtual slot 43 (+0xac) with 0x20,
//   CCombatActionBase::Initialise 0x00857520.
// Layout: CCombatActionBase PCreature +0x08 (landed 0x008C2A80); the stab target CIntelligentPointer at +0xa8 (retail;
//   engine/CCombatAction_StabThing.h is pads only there). Thing model as the landed 0x0095D310 plus the +0x60 pointer.
// Shape: retail makes one discarded GetPItem call before the interface test (a named, unused target local -- a
//   compiled-out check); every other use re-fetches GetPItem; the +0x60 object is read through an inline accessor so it
//   loads straight into ecx for the virtual call.
// Leaf: Initialise
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
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x14];
    struct CThingGraphic* m_pGraphic;           // +0x60
    unsigned char _pad64[0x2d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    struct CThingGraphic* GetGraphic() const { return m_pGraphic; }
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CThingGraphic {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual void SetFlag(long flag);            // slot 43 (+0xac)
};

struct CEvent { };

struct CEventFiredShot : CEvent {
    unsigned char _body[0x10];                  // sizeof = 0x10
    CEventFiredShot(CThing* shooter);           // 0x0087FED0
};

class CTCEntityEvents { public:                        // interface type 0x42
    void AddEvent(CEvent* event);               // 0x008113C0
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];          // +0x10
    void Initialise();                          // 0x00857520
};

class CCombatAction_StabThing : public CCombatActionBase { public:
    CIntelligentPointer<CThing> PTarget;            // +0xa8
    void Initialise();
};

void CCombatAction_StabThing::Initialise()
{
    if (IS_THING_ALIVE(PTarget)) {
        CThing* target = PTarget.GetPItem();
        CTCEntityEvents* events;
        if (PTarget.GetPItem()->GetInterface(&events, 0x42))
            events->AddEvent(new CEventFiredShot(PCreature.GetPItem()));
        PTarget.GetPItem()->GetGraphic()->SetFlag(0x20);
    }
    CCombatActionBase::Initialise();
}