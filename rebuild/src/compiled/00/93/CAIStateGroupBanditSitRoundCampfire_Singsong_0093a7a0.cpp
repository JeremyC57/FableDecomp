// CAIStateGroup_BanditSitRoundCampfire::Singsong() -- sheathe weapons and play the drunken sing-song (into / loop / out animations) for a random 5..10 seconds; true when the action completed (result 2)
// Leaf: ?Singsong@CAIStateGroup_BanditSitRoundCampfire@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A7A0, length 227 bytes (+ int3 padding).
// Callees: CThingCreatureBase::SheatheAllWeapons @ 0x006647F0, CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0,
// GFFloatRandom(float, ulong*) @ 0x004978A0 (__fastcall; 5.0f pushed), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCreatureAction_PlayIntoLoopOutOfAnimation ctor @ 0x008427E0
// (creature, into, loop, out, time to loop, bool, long, long: the M form), CAIStateGroupBase::AIAction_PerformActionAndWait @ 0x0097DB70,
// ~CCreatureActionBase @ 0x00693EF0. Constant 5.0f @ 0x01292458.
// Strings: "ST_SITTING_SING_SONG_DRUNK_LOOP" @ 0x01292538 (out and loop), "ST_SITTING_SING_SONG_DRUNK_INTO" @ 0x01292518.
// Shape: three named CCharString locals (out, loop, into; built before any argument push, which call temporaries do not give) in an
// inner block with the creature, a named local read after the strings; the action temporary's implicit dtor destroys its two CCharString
// members (+0xbc, +0xc0) inline before the out-of-line base dtor. Ego_d.pdb names ret (declared outside the block, as Eat 0x0093A5C0 holds
// its result) and singsong_time; the block makes the `ret == AI_ACTION_FINISHED` test follow every destructor, which gives retail's
// `xor eax,eax ; cmp esi,2 ; sete al` bool tail (a function-scope ret drops the xor).
// Identity (agentNF77, q10884): Ego_r.pdb 0x0095DBF0 + FableWin.pdb 0x02B7FA80 ?Singsong@CAIStateGroup_BanditSitRoundCampfire@@AAE_NXZ
//   (private, bool): the self leaf was the local ?Singsong@..@@QAEJXZ. The action temporary's destruction is ~CCreatureActionBase
//   (??1CCreatureActionBase@@UAE@XZ, both PDBs; RTTI CCreatureAction_PlayIntoLoopOutOfAnimation : CCreatureActionBase), formerly bound as
//   a local ??1CCreatureAction_PlayAnimationBase@@QAE@XZ.
// Layout: engine/CAIStateGroupBase.h / CAIBrain.h / CThingCreatureBase.h, engine/CCreatureActionBase.h and the new overlay header
//   engine/CCreatureAction_PlayIntoLoopOutOfAnimation.h (FABLE_RETAIL_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_IMPLICIT_DTOR: no
//   user-declared destructor, as retail). CAIStateGroup_BanditSitRoundCampfire has no shared header: local, Ego_r.pdb members
//   (struct_layouts_egor.tsv; as the landed sibling Idle 0x0093A540). Was a local CThingCreatureBase / CCharString / CCreatureActionBase /
//   CCreatureAction_PlayIntoLoopOutOfAnimation / CAIBrain / CAIStateGroupBase model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_BEDSLEEP
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_IMPLICIT_DTOR
#include "fable_string.h"
#include "engine/EAIAction.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/GFFloatRandom.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_PlayIntoLoopOutOfAnimation.h"

class CThingObject;
// CAIStateGroup_BanditSitRoundCampfire (Ego_r.pdb sizeof 48; no shared header).
class CAIStateGroup_BanditSitRoundCampfire : public CAIStateGroupBase {
private:
    bool Singsong();
    CIntelligentPointer<CThingObject> PCampfire;            // +0x14
    long          SeatPosIndex;                             // +0x1c
    bool          GotSeatPos;                               // +0x20
    bool          Sitting;                                  // +0x21
    unsigned long FrameToNextSitOnSeat;                     // +0x24
    unsigned long FrameToNextCheckIsRunnable;               // +0x28
    bool          WasCarryingWeapon;                        // +0x2c
};

bool CAIStateGroup_BanditSitRoundCampfire::Singsong()
{
    GetCreature().SheatheAllWeapons();
    float singsong_time = GFFloatRandom(5.0f, GetPWorldSeed()) + 5.0f;
    EAIActionReturn ret;
    {
        CCharString outAnim("ST_SITTING_SING_SONG_DRUNK_LOOP");
        CCharString loopAnim("ST_SITTING_SING_SONG_DRUNK_LOOP");
        CCharString intoAnim("ST_SITTING_SING_SONG_DRUNK_INTO");
        CThingCreatureBase& creature = GetCreature();
        ret = AIAction_PerformActionAndWait(
            CCreatureAction_PlayIntoLoopOutOfAnimation(creature, intoAnim, loopAnim, outAnim, singsong_time, false, 0, 0), false);
    }
    return ret == AI_ACTION_FINISHED;
}
