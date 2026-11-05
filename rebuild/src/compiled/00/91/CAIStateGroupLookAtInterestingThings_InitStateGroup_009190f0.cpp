// CAIStateGroup_LookAtInterestingThings::InitStateGroup(const CWorld&, CThingAICreature&) -- schedule the first
// Leaf: ?InitStateGroup@CAIStateGroup_LookAtInterestingThings@@UAEXABVCWorld@@AAVCThingAICreature@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// look 0..239 + 0..119 seconds from now and register the group with the creature's idle scheduler (type 9).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009190F0, length 184 bytes (+ int3 padding). `ret 8`.
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0 (twice), GFROR13 @ 0x00497890, GFGetCurrentFrame
// @ 0x0049D870, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the creature argument, 0xd3 = bit 19
// of +0x38), CTCIdleScheduler::RegisterIdleStateGroup(CAIStateGroupBase*, EIdleStateGroup) @ 0x00801920; the
// frame-rate global @ 0x01375550.
// Shape: two inlined world LCG draws (`% 240` in ebx, `% 120` in esi); FrameToLook (+0x14, engine header) =
// frame + (a + b) * frames-per-second with the global read into a NAMED local first (retail holds it in ebp across
// the frame call; the bare global is a memory imul operand after the call); the scheduler lookup's key lands in the dead `creature` parameter slot.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase {
protected:
    unsigned long* GetPWorldSeed() const;   // 0x008FCFD0 (Ego_r.pdb ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ)
private: public:
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

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290 / 0x006AF2B0 twins, out of line, `ret 4`
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

public:
    static long __fastcall GetFrame();   // 0x0049D870
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
};

long __fastcall GFROR13(unsigned long);      // 0x00497890
            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

class CThingAICreature;
class CTCIdleScheduler { public:                       // interface 0xd3
    void RegisterIdleStateGroup(CAIStateGroupBase* group, long idleType);   // 0x00801920
};

struct CAIStateGroup_LookAtInterestingThings : CAIStateGroupBaseW {
    long FrameToLook;                           // +0x14 (engine header)
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
    bool InitCreaturePosition();
    virtual bool IsRunnable();
};

void CAIStateGroup_LookAtInterestingThings::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    unsigned long a = WorldRandomRange(*GetPWorldSeed(), 240);
    unsigned long b = WorldRandomRange(*GetPWorldSeed(), 120);
    long framesPerSecond = CMainGameComponent::GetConstantFPS();
    FrameToLook = CWorld::GetFrame() + (a + b) * framesPerSecond;
    CTCIdleScheduler* scheduler;
    if (((CThing&)creature).GetTC(TCI_IDLE_SCHEDULER, &scheduler))
        scheduler->RegisterIdleStateGroup(this, 9);
}