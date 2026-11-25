// CAIStateGroup_IceTrollIcicleAttack::Update() -- with a live target: mark the target's stealth component in danger, rise up if knocked down, face the target, then post the icicle attack through a brain output for two free frames, wait out any running action and boast; terminate at the end or on any failure
// NF86 (round 7, q11115): the local CCountBlock / CCountedPointer stand-in is replaced by the shared engine/CCountedPointer.h (Reset declared
//   only: FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL), so the call at 0x00668D90 binds the explicit instantiation NF83 landed there,
//   ?Reset@?$CCountedPointer@VCCreatureActionBase@@@@QAEXPAVCCreatureActionBase@@@Z (was ?Reset@CCountedPointer@@QAEXPAX@Z); the local Clone() returns CCreatureActionBase* (its PDB type).
// Leaf: ?Update@CAIStateGroup_IceTrollIcicleAttack@@QAEXXZ
// Name fix (NF96 part 2, q11460): the local CTCStealth declared SetAsInDanger (CThing* source, long status) and bound a stand-in spelling of 0x00747B80;
//   now the Ego_r.pdb + FableWin.pdb ?SetAsInDanger@CTCStealth@@QAEXABVCThing@@W4EDangerStatus@NStealth@@@Z (engine/CTCStealth.h signature;
//   the reference and the enum push the same words).
// NF84 (round 7, q11050 binder): the local CThingCreatureBase model's IsFreeToPerformAction respelt in place to the Ego_r.pdb /
//   FableWin.pdb form ?IsFreeToPerformAction@CThingCreatureBase@@QBE_NABVCCreatureActionBase@@@Z (const, by const reference; the
//   landed self row 0x00662210 defines it), as NF83 did in 0x00662210 itself; the call passes the action by reference.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009621F0, length 733 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CVectorMap::LowerBound @ 0x0040F020, CTCStealth::SetAsInDanger @ 0x00747B80, CTCCreatureModeManager::HasMode @ 0x0073A8A0,
// CCreatureAction_RiseUp ctor @ 0x008BD880, ~CCombatActionBase @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70,
// AIAction_LookAtThing @ 0x0097D6F0, IsStopping @ 0x008FD010, CCombatActionBase ctor @ 0x00857F30 (code 0x96; vtable
// CCreatureAction_IceTrollIcicleAttack 0x012960C4), CThingCreatureBase::IsFreeToPerformAction @ 0x00662210 /
// IsPerformingAnyAction @ 0x006622A0, CBrainOutput ctor @ 0x00835620, CCountedPointer::Reset @ 0x00668D90,
// CAIBrain::SetOutput @ 0x0088B8F0 / SuspendSuspendableProcess (virtual slot 2), operator delete @ 0x00BFE9BC, AIAction_DoNothing
// @ 0x0097E200, CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0, CCreatureAction_PlayCombatAnimation ctor @ 0x008B7FF0,
// AIAction_CombatPerformActionAndWait @ 0x0097DA40, Terminate @ 0x008FCF40.
// Shape: the component tests are `bool has = HasTC()` locals (shr/and stored to a frame byte); the stealth lookup is a
// direct GetTC into a named pointer; the boast result is compared against 1 and both arms terminate (the compare folds
// away, the result copy in edi stays).
// Layout: Ego_r.pdb CAIStateGroup_IceTrollIcicleAttack PTarget +0x14.
// NF175 (2026-11-20, q4968): AIAction_LookAtThing 0x0097D6F0 respelled in place on the row-local CAIStateGroupBase from the stand-in
//   ?AIAction_LookAtThing@CAIStateGroupBase@@QAEJJUCIntelligentPointerConstThing@@JM@Z to the PDB
//   ?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z (Ego_r.pdb
//   0x008484A0, protected); PTarget and the by-value argument are the shared CIntelligentPointer<CThing> / <const CThing> (converting
//   constructor inline), the enums engine/EAIAction.h (the empty local EAIActionReturn dropped).
class CThing;
class CThingCreatureBase;
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool IsFreeToPerformAction(const CCreatureActionBase& action) const;   // 0x00662210
    bool IsPerformingAnyAction() const;               // 0x006622A0
};
namespace NStealth {   // Ego_d.pdb LF_ENUM NStealth::EDangerStatus (the enumerators of engine/CTCStealth.h FABLE_RETAIL_NSTEALTH_EDANGERSTATUS)
enum EDangerStatus { DANGER_STATUS_NULL=0, DANGER_STATUS_INVESTIGATING=1, DANGER_STATUS_CHASING=2, DANGER_STATUS_COMBAT=3, DANGER_STATUS_RANGED_COMBAT=4 };
}
class CTCStealth { public:                             // interface 0x36
    void SetAsInDanger(const CThing& source, NStealth::EDangerStatus status);   // 0x00747B80
};
class CTCCreatureModeManager { public:                 // interface 0x31
    // Naming (NF439 gen, 2026-11-25): callee 0x0073A8A0 binds its PDB spelling ?IsModeActive@CTCCreatureModeManager@@QBE_NK@Z (T1 target; was ?HasMode@CTCCreatureModeManager@@QAE_NJ@Z).
    bool IsModeActive(unsigned long mode) const;
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#include "engine/CIntelligentPointer.h"
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CCountedPointer.h"                // NF86: CCountedPointer<T> {Data, Info}; Reset(T*) out of line (0x00668D90)
class CCreatureActionBase;
class CBrainOutput { public:                           // 0x48 bytes
    unsigned long Flags;                        // +0x00 (8: perform action)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer<CCreatureActionBase> PAction; // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;            // +0x40
    CBrainOutput();                             // 0x00835620
};
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CCreatureActionBase* Clone() const;                // slot 11 (+0x2c)
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_IceTrollIcicleAttack : public CCombatActionBase { public:   // vtable 0x012960C4
    virtual void v00();
    CCreatureAction_IceTrollIcicleAttack(CThingCreatureBase* owner, CThing* target)
        : CCombatActionBase(owner, target, 0x96, CSoundPair()) { }
};
class CCreatureAction_RiseUp : public CCombatActionBase { public:
    long m_riseState;
    CCreatureAction_RiseUp(CThingCreatureBase& creature);   // 0x008BD880
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
    long m_loop;                                // retail sizeof 0x118 (frame)
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase& creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};
inline void SetPerformAction(CBrainOutput& output, CCombatActionBase* action)
{
    output.Flags |= 8;
    output.PAction.Reset(action->Clone());
}
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual bool SuspendSuspendableProcess();                       // slot 2 (+0x08) 0x00A44840 ?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ (Ego_r.pdb + FableWin.pdb, inherited; was the stand-in Yield; name-fix 2026-11-12)
    unsigned char _pad04[0x18 - 0x04];
    CThingCreatureBase* PCreature;              // +0x18
    void SetOutput(const CBrainOutput& output);   // 0x0088B8F0 ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z (Ego_r.pdb + FableWin.pdb; was the stand-in SetOutput; name-fix 2026-11-12)
};
#include "engine/EAIAction.h"
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    bool IsStopping() const;                          // 0x008FD010 (Ego_r.pdb protected const)
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
    protected: EAIActionReturn AIAction_CombatPerformActionAndWait(const CCombatActionBase& action, bool wait); public:   // 0x0097DA40 (NF315: the PDB signature, Ego_r + FableWin)
    protected: EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long flags, float turnSpeed); // 0x0097D6F0
    EAIActionReturn AIAction_DoNothing(); public:                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_IceTrollIcicleAttack : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;        // +0x14
    void Update();
};

void CAIStateGroup_IceTrollIcicleAttack::Update()
{
    if (IS_THING_ALIVE(PTarget)) {
        CThing* target = ((CThing*)PTarget.GetPItem());
        bool has = target->HasTC(TCI_STEALTH);
        if (has) {
            CTCStealth* stealth = ((CTCStealth*)&target->GetTC(TCI_STEALTH));
            stealth->SetAsInDanger(*GetCreature(), NStealth::DANGER_STATUS_COMBAT);
        }
        CTCCreatureModeManager* modes;
        if (GetCreature()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes) && modes->IsModeActive((unsigned long)0x1e)) {
            long result = AIAction_PerformActionAndWait(CCreatureAction_RiseUp(*GetCreature()), false);
            if (result == 1) {
                Terminate();
                return;
            }
        }
        if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PTarget, 1, 0.01f) == AI_ACTION_FINISHED && IS_THING_ALIVE(PTarget) && !IsStopping()) {
            long remaining = 2;
            do {
                CThingCreatureBase* creature = GetCreature();
                CCreatureAction_IceTrollIcicleAttack action(creature, ((CThing*)PTarget.GetPItem()));
                if (GetCreature()->IsFreeToPerformAction(action)) {
                    CBrainOutput output;
                    SetPerformAction(output, &action);
                    PBrain->SetOutput(output);
                    --remaining;
                }
                PBrain->SuspendSuspendableProcess();
                if (IsStopping()) {
                    Terminate();
                    return;
                }
            } while (remaining > 0);
            while (GetCreature()->IsPerformingAnyAction()) {
                if (AIAction_DoNothing() == 1) {
                    Terminate();
                    return;
                }
            }
            long result = AIAction_CombatPerformActionAndWait(
                CCreatureAction_PlayCombatAnimation(*GetCreature(), "ICICLE_BOAST", true), false);
            if (result == 1) {
                Terminate();
                return;
            }
        }
    }
    Terminate();
}