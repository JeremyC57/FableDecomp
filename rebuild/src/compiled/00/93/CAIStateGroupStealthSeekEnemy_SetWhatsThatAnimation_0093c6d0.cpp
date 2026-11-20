// CAIStateGroup_StealthSeekEnemy::SetWhatsThatAnimation(CTCPerceiveThing*) -- play a random WHATS_THAT animation; the
// Leaf: SetWhatsThatAnimation
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetMagnitudeSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// update finishes unless the animation completed, the perceived thing is still worth seeking and it has not moved
// from the last known position (squared 3D distance within 1e-4 squared).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093C6D0, length 206 bytes (+ int3 padding), `ret 4`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("WHATS_THAT" @ 0x01273694), CThingCreatureBase::
// GetRandomAnimFromName @ 0x00662D80, CCreatureAction_PlayAnimation(creature, CAnimationEntry*, sound, ...) @
// 0x00842570, CAIStateGroupBase::AIAction_PerformAction @ 0x0097D8B0, ~CCreatureActionBase @ 0x00693EF0, ~CCharString
// @ 0x0099EAE0, IsStillRunnable(CTCPerceiveThing*) @ 0x0093C4E0, the finish call @ 0x008FCF40; epsilon float @ 0x0129BA3C.
// Shape: the creature through an inline const GetCreature() accessor (PBrain loaded early, as retail); an IMPLICIT CCharString conversion temporary for the name, the action result as a named long compared after
// the action dtor, `diff.GetMagnitudeSquared() > EPS * EPS` with the epsilon read from its global twice.
// No engine header for this class; LastKnownEnemyPos (+0x1c) named descriptively.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model and a
// derived-class AIAction_PerformAction taking a pointer): the callee 0x0097D8B0 is the protected
// ?AIAction_PerformAction@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@@Z (Ego_r.pdb 0x00848680; header _MEMBERS +
// _PERFORMACTION_NOWAIT), the action passed by const reference; Terminate 0x008FCF40 is the base's.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

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
    FABLE_FORCEINLINE CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    FABLE_FORCEINLINE CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
#include "engine/C3DVector.h"

class CAnimationEntry;
// NF196 (2026-11-20, q6456): GetRandomAnimFromName is the shared header's const member returning the entry by reference (flat-view
//   opt-in), binding Ego_r.pdb / FableWin.pdb ?GetRandomAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z at
//   0x00662D80 (was the stand-in ?GetRandomAnimFromName@CThingCreatureAnim@@QAEPAVCAnimationEntry@@ABVCCharString@@J@Z, struct removed);
//   the row's existing CCreatureAction_PlayAnimation model is respelled in place to the PDB constructor
//   ??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@ABVCSoundPair@@_N33JJ@Z (Ego_r.pdb 0x0051CA80; landed
//   0x00842E40 binds it; was the pointer form ..@PAVCThing@@PAVCAnimationEntry@@..).
#define FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_GETRANDOMANIMFROMNAME
#include "engine/CThingCreatureBase.h"

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase& creature, const CAnimationEntry& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x00842570
};

class CTCPerceiveThing { public:
    unsigned char _pad[0x20];
    float PosX, PosY, PosZ;                     // +0x20
};

extern const float DELTA_NOTIONAL_ZERO; // 0x0129BA3C (1e-4)

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    unsigned char _pad_0x14[0x8];               // +0x14 PEnemy
    float LastKnownEnemyPosX, LastKnownEnemyPosY, LastKnownEnemyPosZ;   // +0x1c
    CThing* GetCreature() const { return PBrain->PCreature; }
    bool IsStillRunnable(CTCPerceiveThing* perceive);   // 0x0093C4E0
    void SetWhatsThatAnimation(CTCPerceiveThing* perceive);
};

void CAIStateGroup_StealthSeekEnemy::SetWhatsThatAnimation(CTCPerceiveThing* perceive)
{
    const CAnimationEntry& animation = ((CThingCreatureBase*)GetCreature())->GetRandomAnimFromName("WHATS_THAT", -1);
    long result = AIAction_PerformAction(CCreatureAction_PlayAnimation(*(CThingCreatureBase*)GetCreature(), animation, CSoundPair(),
        false, true, false, 0, 0));
    if (result == 2 && IsStillRunnable(perceive)) {
        C3DVector diff(perceive->PosX - LastKnownEnemyPosX, perceive->PosY - LastKnownEnemyPosY,
            perceive->PosZ - LastKnownEnemyPosZ);
        if (!(diff.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
            return;
    }
    Terminate();
}