// CAIStateGroup_Drunk::IsRunnable() -- runnable when the creature has not just sent a "drunk" message (type 0x14),
// has no scripted override, and its drunk component (interface 0xc3) reports it is drunk.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00922600, length 166 bytes (+ int3 padding). Vtable slot 11.
// Callees: CMessageEventFilter_IsTargettedAtThing ctor @ 0x006AF290, the second world accessor @ 0x008FCFA0
// (message manager at +0x60), CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType,
// IsOriginatedByThing>> @ 0x008D46D0, the brain accessor @ 0x0088B7D0 (`[+0x38] ? [+0x38]+0xc : 0`; the manifest's
// CEditWorld::DrawGetWorldMap label is propagated), CVectorMap::LowerBound @ 0x004365B0 (interface 0xc3).
// Shape: the filter chain `IsOfType(0x14) && IsOriginatedByThing(creature)` is built in the frame (Bind2 pair of
// pointers at +8/+0xc, the type at +4, the UID filter at +0x10); a hit returns `xor al,al`; the rest is one
// `&&` chain returned through the full register (`mov eax,1` / `xor eax,eax`), the creature reloaded from PBrain.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
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

class CMessageEventFilter_IsTargettedAtThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsTargettedAtThing(const CThing& thing);     // 0x006AF290 ??0CMessageEventFilter_IsTargettedAtThing@@QAE@ABVCThing@@@Z (retail calls 0x6AF290 at both sites)
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
    CMessageEventManager* GetPMessageManager() const { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
};

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsTargettedAtThing> CDrunkFilter;

class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CDrunkFilter& filter);   // 0x008D46D0
};
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindMostRecentMessage does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindMostRecentMessage instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CDrunkFilter& filter)
{
    long r = TmplSink_s7_1(this, 0 + *(const long*)&filter);
    return (CMessageEvent*)r;
}
#pragma auto_inline(on)

struct CTCDrunk {                               // interface 0xc3
    unsigned char _pad[0xc];
    unsigned char m_isDrunk;                    // +0x0c
};

struct CBrainOverride;
struct CAIBrainX : CAIBrain {
    CBrainOverride* GetPOverride();             // 0x0088B7D0
};

struct CAIStateGroup_Drunk : CAIStateGroupBaseW {
    virtual bool IsRunnable();
};

bool CAIStateGroup_Drunk::IsRunnable()
{
    CThing* creature = PBrain->PCreature;
    if (PeekWorld().GetPMessageManager()->FindMostRecentMessage(
            CMessageEventFilter_IsOfType(0x14) && CMessageEventFilter_IsTargettedAtThing(*creature)))
        return false;
    CTCDrunk* drunk;
    return ((CAIBrainX*)PBrain)->GetPOverride() == 0
        && PBrain->PCreature->GetInterface(&drunk, 0xc3)
        && drunk->m_isDrunk;
}