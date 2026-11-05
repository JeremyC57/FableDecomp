// CAIStateGroup_HoverHornetQueen::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) -- remember the queen's start position, run the enemy-creature base initialiser, reset the phase state and put
// Leaf: ?Initialise@CAIStateGroup_HoverHornetQueen@@QAEXAAVCAIBrain@@AAVCThingAICreature@@JJABVCCharString@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// the boss component (interface 0xad) back to phase 0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A890, length 163 bytes.
// Callees: CThing::GetPosition @ 0x006A4D00 (manifest label CVertexBufferWin32::DoSizeof is propagated),
// CAIStateGroup_EnemyCreatureBase::Initialise @ 0x0091D820 (five arguments forwarded), CVectorMap::LowerBound
// @ 0x0040F020 (inlined GetInterface, 0xad = bit 13 of +0x34), CTCBoss::SetNextBossPhase(long, bool) @ 0x007AEDA0.
// Shape: StartPos (+0x1c, PDB) copied through registers from the returned reference; the three resets from one
// zeroed edi (bOneTime +0x34 = true first); the boss lookup used unconditionally (miss path reads the never-written
// key slot, which VC places in the dead `creature` parameter slot); the call is duplicated on both paths.
// Layout: engine/CAIStateGroup_HoverHornetQueen.h names at the retail STLport 4.6 offsets (vector 0xc): the two dword
// clears are CurrentCirclePoint +0x4c and RouteDirection +0x64 (the Ego_r donor put Things over +0x4c and
// EnemyKnockDownFrame at +0x64, which is why this model once named them m_phaseCounter / EnemyKnockDownFrame).
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

#include "engine/C3DVector.h"
class CCharString;
typedef long EScriptAIPriority;

class CThingPhysical { public:
    const C3DVector& PeekPos() const;             // 0x006A4D00
};
class CThingAICreature : public CThingPhysical { public:
};

class CTCBoss { public:                                // interface 0xad
    void SetNextBossPhase(long phase, bool force);   // 0x007AEDA0
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    unsigned char PTargetThing[8];              // +0x14 CIntelligentPointer<CThingCreatureBase>
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority,
                    const CCharString& name);   // 0x0091D820
};

struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_EnemyCreatureBase {
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool bOneTime;                              // +0x34
    unsigned char _pad35[3];
    CThing* PEmitter;                           // +0x38
    CThing* PEmitter2;                          // +0x3c
    unsigned char Things[0xc];                  // +0x40 vector<CIntelligentPointer<CThing> >
    long CurrentCirclePoint;                    // +0x4c
    unsigned char CircleMarkers[0xc];           // +0x50 vector<CIntelligentPointer<CThing> >
    long EnemyKnockDownFrame;                   // +0x5c
    bool TimingCritical;                        // +0x60
    unsigned char _pad61[3];
    long RouteDirection;                        // +0x64 eWaspDirection
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority,
                    const CCharString& name);
};

void CAIStateGroup_HoverHornetQueen::Initialise(CAIBrain& brain, CThingAICreature& creature, long id,
                                                EScriptAIPriority priority, const CCharString& name)
{
    StartPos = creature.PeekPos();
    CAIStateGroup_EnemyCreatureBase::Initialise(brain, creature, id, priority, name);
    bOneTime = true;
    RouteDirection = 0;
    CurrentCirclePoint = 0;
    CTCBoss* boss;
    PBrain->PCreature->GetTC(TCI_BOSS, &boss);
    boss->SetNextBossPhase(0, false);
}