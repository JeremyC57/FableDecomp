// CAIStateGroup_MeleeCombatNearby::AttackEnemy() -- flag a registered combatant ready to attack, then pump combat
// Leaf: ?AttackEnemy@CAIStateGroup_MeleeCombatNearby@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Grade: RELOCATION_MATCH -- the loop-exit manager call goes through the inline GetPCombatManager() accessor (mov eax,[eax+0x4c] ; push edi ; mov ecx,eax).
// actions (terminating when the enemy dies or a pump neither succeeds nor waits) until the group stops, stops
// being runnable, leaves combat, drifts from its combat position, or the manager says it is ready to attack.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096EA50, length 187 bytes.
// Callees: CThingCreatureBase::IsInMeleeAsAttacker @ 0x00662410 (twice), the brain world accessor @ 0x008FCFC0
// (combat manager at +0x4c), CCombatManager::SetAttackerAsReadyToAttack @ 0x006EA450 /
// IsAttackerReadyToAttack @ 0x006E93C0, AIAction_PumpCombatActions @ 0x0097D980, CBaseIntelligentPointer::GetPItem
// (inlined IS_THING_ALIVE on PEnemy +0x14), Terminate @ 0x008FCF40, IsStopping @ 0x008FD010, this->IsStillRunnable
// (slot 14, +0x38), IsEnemyCreatureNearCombatPosition @ 0x0096E830.
// Shape: the creature is a named local read before the world call both times; a do/while whose PEnemy address is
// hoisted into ebp above the aligned loop head; every bail-out is a plain `return` (shared epilogue).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// Name fix (agentNF26, 2026-11-12): CThingCreatureBase::IsInMeleeAsAttacker 0x00662410 is ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ
//   (Ego_r.pdb + FableWin.pdb; body GetCombatManager().IsAttacker(*this)); was IsRegisteredCombatant (in neither PDB).
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
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h and the AI enum from
// engine/fable_ai_action_enums.h (were a local CAIStateGroupBase model, a derived-class AIAction_PumpCombatActions and a local
// EAIActionReturn with made-up enumerators): the callee 0x0097D980 is the protected
// ?AIAction_PumpCombatActions@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ (Ego_r.pdb 0x00848740; header _MEMBERS + _PUMPCOMBATACTIONS);
// GetWorld 0x008FCFC0 is the header's protected ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (_GOTOBED_WORLD), IsStopping / Terminate
// the base's; the slot-14 IsStillRunnable(bool) call (+0x38) is declared on the derived class, first after the base's slots 0..13.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

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
class CCombatManager;
class CWorld { public:
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
    CCombatManager* GetPCombatManager() const { return CombatManager; }
    unsigned char _pad[0x10];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

class CThingCreatureBase { public:
    bool IsInMeleeAsAttacker() const;         // 0x00662410 (Ego_r/FableWin ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ; manifest label C3DAnimationBlendState::AddCombinationBlend propagated)
};

class CCombatManager { public:
    void SetAttackerAsReadyToAttack(const CThingCreatureBase& attacker, bool ready);   // 0x006EA450 (NF34: Ego_r + FableWin ?SetAttackerAsReadyToAttack@CCombatManager@@QAEXABVCThingCreatureBase@@_N@Z)
    bool IsAttackerReadyToAttack(CThingCreatureBase* attacker);                  // 0x006E93C0
};

struct CAIStateGroup_MeleeCombatNearby : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool Archer;                                // +0x1c (engine header)
    virtual bool IsStillRunnable(bool checkEnemy);   // slot 14 (+0x38), first after the base's slots 0..13
    bool IsEnemyCreatureNearCombatPosition();   // 0x0096E830
    void AttackEnemy();
    virtual void Cleanup();
};

void CAIStateGroup_MeleeCombatNearby::AttackEnemy()
{
    if (((CThingCreatureBase*)PBrain->PCreature)->IsInMeleeAsAttacker()) {
        CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
        const CWorld& world = GetWorld();
        world.CombatManager->SetAttackerAsReadyToAttack(*creature, true);
    }
    do {
        EAIActionReturn result = AIAction_PumpCombatActions();
        if (!IS_THING_ALIVE(PEnemy) || (result != AI_ACTION_FINISHED && result != AI_ACTION_IN_PROGRESS))
            Terminate();
        if (IsStopping())
            return;
        if (!IsStillRunnable(true))
            return;
        if (!((CThingCreatureBase*)PBrain->PCreature)->IsInMeleeAsAttacker())
            return;
        if (!IsEnemyCreatureNearCombatPosition())
            return;
        CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
        const CWorld& world = GetWorld();
        if (world.GetPCombatManager()->IsAttackerReadyToAttack(creature))
            return;
    } while (true);
}