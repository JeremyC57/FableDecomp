// CAIStateGroup_MazeTutorialBattleRespondToLightning::Update() -- while the target is alive and the group is not
// Leaf: ?Update@CAIStateGroup_MazeTutorialBattleRespondToLightning@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
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
// NF134 (2026-11-20, q5021): AIAction_PerformActionAndWait 0x0097DB70 respelled in place onto the row-local CAIStateGroupBase as the PDB
//   ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z (Ego_r.pdb 0x00848940 + FableWin.pdb
//   0x02892570: protected, const CCreatureActionBase&); was the derived stand-in
//   ?AIAction_PerformActionAndWait@CAIStateGroup_MazeTutorialBattleRespondToLightning@@QAE?AW4EAIActionReturn@@AAVCCreatureActionBase@@_N@Z.
//   The row's existing local EAIActionReturn line moved above the base (unchanged).
// NF175 (2026-11-20, q4968): AIAction_LookAtThing 0x0097D6F0 respelled from the derived stand-in (CIntelligentPointerConstThing, local
//   enums) to the PDB ?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z
//   (Ego_r.pdb 0x008484A0, protected), declared on the row-local CAIStateGroupBase; the target member and the by-value argument are the shared
//   CIntelligentPointer<CThing> / <const CThing> (converting constructor inline), the enums engine/EAIAction.h (repeat 1 = ACTION_REPEAT_UNTIL_FINISHED).
// NF196 (2026-11-20, q4411 / q6405): the local CCreatureActionBase / CCreatureAction_CastWillLightningSpell models are gone; the action
//   comes from the shared engine/CCreatureAction_CastWillLightningSpell.h (opt-in FABLE_RETAIL_CCREATUREACTION_CASTWILLLIGHTNINGSPELL_MAZE, on
//   engine/CCombatActionBase.h, retail CCreatureActionBase layout FABLE_RETAIL_CCREATUREACTIONBASE; RTTI: CCombatActionBase@0). The temporary's
//   implicit destructor now calls ??1CCombatActionBase@@UAE@XZ at 0x0062E900 (was the stand-in ??1CCreatureAction_CastWillLightningSpell@@QAE@XZ),
//   and the constructor at 0x008BCA00 binds the PDB ??0CCreatureAction_CastWillLightningSpell@@QAE@AAVCThingCreatureBase@@JMAAVCThing@@@Z
//   (Ego_r.pdb 0x008F5020; creature and target by reference; was the pointer-form stand-in).
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#include "engine/CIntelligentPointer.h"

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

#include "engine/EAIAction.h"
class CCreatureActionBase;
class CAIStateGroupBase { public:
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool interruptible);   // 0x0097DB70
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> target, long frames, float speed);   // 0x0097D6F0
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
protected:
    bool IsStopping() const;   // 0x008FD010 (Ego_r.pdb protected const)
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

class CThingCreatureBase;
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLLIGHTNINGSPELL_MAZE
#include "engine/CCreatureAction_CastWillLightningSpell.h"

struct CAIStateGroup_MazeTutorialBattleRespondToLightning : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;        // +0x14
    unsigned char _pad1c[4];
    long FrameOfLastHit;                        // +0x20 (engine header)
    virtual void Update();
};

void CAIStateGroup_MazeTutorialBattleRespondToLightning::Update()
{
    if (IS_THING_ALIVE(PTarget) && !IsStopping()) {
        AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PTarget, 0x21, 0.083333336f);
        if (IS_THING_ALIVE(PTarget) && !IsStopping()) {
            CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
            AIAction_PerformActionAndWait(CCreatureAction_CastWillLightningSpell(*creature, 2, 3.0f, *((CThing*)PTarget.GetPItem())), false);
        }
    }
    Terminate();
}