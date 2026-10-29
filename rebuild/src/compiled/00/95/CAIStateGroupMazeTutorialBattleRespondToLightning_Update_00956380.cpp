// CAIStateGroup_MazeTutorialBattleRespondToLightning::Update() -- while the target is alive and the group is not
// stopping: look at it, then (still alive, still running) cast a will-lightning spell at it and wait; terminate.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956380, length 218 bytes (+ int3 padding). Vtable slot 12.
// Callees: CBaseIntelligentPointer::GetPItem/ctor/SetPItem @ 0x00A01B50/0x00A01B10/0x00A01B90 (target at +0x14),
// CAIStateGroupBase::IsStopping @ 0x008FD010, AIAction_LookAtThing(EAIActionRepeat, CIntelligentPointer<const
// CThing>, long, float) @ 0x0097D6F0, CCreatureAction_CastWillLightningSpell(CThingCreatureBase*, long, float,
// CThing*) @ 0x008BCA00 (a 0x11c-byte frame temporary), AIAction_PerformActionAndWait(CCreatureActionBase&, bool)
// @ 0x0097DB70, the action destructor @ 0x0062E900 (manifest label ~CTCLightningOrb is propagated), Terminate
// @ 0x008FCF40.
// Shape: nested `IS_THING_ALIVE && !IsStopping()` tests (inlined twice) sharing the Terminate tail; LookAtThing's
// by-value CIntelligentPointer<const CThing> (vtable 0x01237F1C) built in the argument area; the action temporary
// built from right-to-left arguments after the `false` push, its constructor's `this` result passed on.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
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
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
public:
public:
    void Terminate();   // 0x008FCF40
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

class CMessageEventFilter_HasExtraData { public:
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

class CMessageEvent;
class CMessageEventManager;
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(((CThing*)other.GetPItem())); }
};

enum EAIActionRepeat { AI_ACTION_REPEAT_NONE, AI_ACTION_REPEAT_ONCE };
enum EAIActionReturn { AI_ACTION_RETURN_NONE, AI_ACTION_RETURN_1, AI_ACTION_RETURN_SUCCEEDED };

class CThingCreatureBase;
class CCreatureActionBase { public:
    unsigned char _body[0x11c];
};

class CCreatureAction_CastWillLightningSpell : public CCreatureActionBase { public:
    CCreatureAction_CastWillLightningSpell(CThingCreatureBase* caster, long spell, float duration, CThing* target);  // 0x008BCA00
    ~CCreatureAction_CastWillLightningSpell();  // 0x0062E900
};

struct CAIStateGroup_MazeTutorialBattleRespondToLightning : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14 CIntelligentPointer<CThing>
    unsigned char _pad1c[4];
    long FrameOfLastHit;                        // +0x20 (engine header)
    bool IsStopping();                          // 0x008FD010
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointerConstThing target, long frames, float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_PerformActionAndWait(CCreatureActionBase& action, bool interruptible);   // 0x0097DB70
    virtual void Update();
};

void CAIStateGroup_MazeTutorialBattleRespondToLightning::Update()
{
    if (IS_THING_ALIVE(PTarget) && !IsStopping()) {
        AIAction_LookAtThing(AI_ACTION_REPEAT_ONCE, PTarget, 0x21, 0.083333336f);
        if (IS_THING_ALIVE(PTarget) && !IsStopping()) {
            CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
            AIAction_PerformActionAndWait(CCreatureAction_CastWillLightningSpell(creature, 2, 3.0f, ((CThing*)PTarget.GetPItem())), false);
        }
    }
    Terminate();
}