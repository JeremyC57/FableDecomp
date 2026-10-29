// CTCPerceiveAlert::FindAlertEvent() -- find the most recent alert message (type 0xC) the owner can see or hear;
// when its sender is alive, an ally of the owner, and the script thing the event names is our tracked thing (+0x0c),
// become directly aware of the perceived thing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00744D00, length 162 bytes (+ int3 padding).
// Label note: catalog "CAIStateGroup_MinionReactToAlert::FindAlertEvent"; the body sits in the CTC region and uses
// +0x04 as the owner thing and +0x0c as an intelligent pointer, and calls SetAsDirectlyAwareOfPerceivedThing
// @ 0x007449B0 on `this` -- treat the class attribution as unverified.
// Callees: the world getter @ 0x00686A80 (`[0x013B8A1C]+24h`, +0x60 = CMessageEventManager),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x00746260,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, AreThingsAllies @ 0x0076D4F0 (__fastcall),
// CGameScriptThing::GetPThing @ 0x006AF520, SetAsDirectlyAwareOfPerceivedThing @ 0x007449B0.
// Shape: the filter chain `IsOfType(0xc) && CanBeSeenOrHeard(owner)` as temporaries (two one-word filters and the Bind2
//   {&first, &second} pair in the 0x10 frame) with the same-TU FindMostRecentMessage stand-in, the manager through an inline
//   accessor (the filter address is pushed after the world call); the sender goes through the landed 0x008D04E0 typed-item
//   test (`GetPItem() != 0 && IsThingOfType<CThing>(GetPItem())`: the rvalue bound to the const reference is the dead frame
//   store, the zero mask the constant `xor al,al ; jne`), then a named alive sender for AreThingsAllies and a named
//   GetPThing() result compared with PTracked (GetPThing is called before PTracked's GetPItem).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    enum { ThingTypeMask = 0 };                 // the base class matches every thing (landed 0x008D04E0 typed-item test)
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x6c - 0x4c];
    unsigned long ThingTypeFlags;               // +0x6c
    unsigned char _pad2[0x91 - 0x70];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class CWorld;
class CMessageEventFilter_IsOfType { public: long Type; CMessageEventFilter_IsOfType(long type) : Type(type) { } };
class CMessageEventFilter_CanBeSeenOrHeard { public: CThing* PPerceiver; CMessageEventFilter_CanBeSeenOrHeard(CThing* perceiver) : PPerceiver(perceiver) { } };
template<class A, class B> struct CFilter_Bind2T {
    const A* PFirst;
    const B* PSecond;
    CFilter_Bind2T(const A& a, const B& b) : PFirst(&a), PSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2T<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2T<A, B>(a, b); }
typedef CFilter_Bind2T<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CFilter_Bind2;

class CGameScriptThing { public:
    CThing* GetPThing();                        // 0x006AF520
};
class CMessageEvent : public CGameScriptThing { public:
    unsigned char _pad[0x48];
    CBaseIntelligentPointerCtor PSender;        // +0x48
};
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind2& filter);   // 0x00746260
};
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindMostRecentMessage does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindMostRecentMessage instantiation (template bodies are compiled in the caller's TU): reads the
// const-reference argument, lets no argument escape.
__declspec(noinline) CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CFilter_Bind2& filter)
{
    long r = TmplSink_s7_1(this, 0 + *(const long*)&filter);
    return (CMessageEvent*)r;
}
#pragma auto_inline(on)
struct CWorldEvents {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
    CMessageEventManager* GetPMessageEventManager() { return PMessageEventManager; }
};

bool __fastcall AreThingsAllies(CThing* a, CThing* b);   // 0x0076D4F0
// the landed 0x008D04E0 / 0x008941A0 typed-item test: the rvalue GetPItem() binds to the const reference through a frame
// temporary (the dead store) and the zero mask folds the test to a constant (`xor al,al ; jne`)
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

struct CTCPerceiveAlert {
    void* __vftable;                            // +0x00
    CThing* POwner;                             // +0x04
    unsigned char _pad_0x08[0x4];
    CBaseIntelligentPointerCtor PTracked;       // +0x0c
    CWorldEvents* GetPWorld();                  // 0x00686A80
    void SetAsDirectlyAwareOfPerceivedThing();  // 0x007449B0
    void FindAlertEvent();
};

void CTCPerceiveAlert::FindAlertEvent()
{
    CThing* owner = POwner;
    CMessageEvent* ev = GetPWorld()->GetPMessageEventManager()->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0xc) && CMessageEventFilter_CanBeSeenOrHeard(owner));
    if (ev != 0) {
        CBaseIntelligentPointerCtor& sender = ev->PSender;
        if (sender.GetPItem() != 0 && IsThingOfType<CThing>(sender.GetPItem())) {
            CThing* thing = sender.GetPItem();
            if (thing != 0 && !(thing->m_flags91 & 1) && AreThingsAllies(thing, owner)) {
                CThing* perceived = ev->GetPThing();
                if (PTracked.GetPItem() == perceived)
                    SetAsDirectlyAwareOfPerceivedThing();
            }
        }
    }
}