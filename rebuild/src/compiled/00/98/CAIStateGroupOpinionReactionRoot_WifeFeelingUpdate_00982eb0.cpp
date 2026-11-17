// CAIStateGroup_OpinionReactionRoot::WifeFeelingUpdate(EOpinionAttitudeType attitude) -- say "TEXT_AI_WIFE_OPINION_ACT_FEELING_" + the attitude's name to the thing being reacted to; true once the speech action has finished (EAIActionReturn 2)
// Leaf: ?WifeFeelingUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982EB0, length 84 bytes.
// Identity: the catalog label "CPlayer::SayWifeOpinionFeeling" is a guess; Ego_r.pdb has
// `?WifeFeelingUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z`,
// WifeGeneralUpdate and WifeToHerselfUpdate 0x60 bytes apart, exactly like 0x00982EB0 / F10 / F70,
// and the body calls the class's SayToThingReactingTo on `this`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the state group's getter
// @ 0x008FCFC0 (returns this->[+4]->[+0x1C]->[+0x24]; called for nothing: the result is dropped;
// manifest label "CPlayer::GetCurrentMode"), CCharString::operator+=(const CCharString&) @
// 0x0099F0A0, CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo(const CCharString&) @
// 0x0095EE70, CCharString::~CCharString @ 0x0099EAE0; the attitude-name table @ 0x013BABD0
// (CCharString per EOpinionAttitudeType); the literal @ 0x0129991C.
// Shape: a named CCharString local in the `push ecx` slot (explicit (literal, -1) constructor),
// the bool `== 2` kept in bl across the destructor (`sete bl ; ... mov al,bl`), `ret 4`.
// NF52 (round 7): the attitude names are the private static COpinionReactionManager::AttitudeStrings
// (?AttitudeStrings@COpinionReactionManager@@0QBVCCharString@@B, 0x013BABD0; both PDBs' dynamic initializer / atexit destructor), read
// through FableWin.pdb's public inline ?GetAttitudeString@COpinionReactionManager@@QBEABVCCharString@@W4EOpinionAttitudeType@@@Z on the
// world's manager (FableWin ?GetOpinionReactionManager@CWorld@@QAEAAVCOpinionReactionManager@@XZ, inline): that is why the GetWorld()
// call survives with its result unused. CCharString / CWorld / COpinionReactionManager / EOpinionAttitudeType come from their shared
// headers (fable_string.h, engine/CWorld.h, engine/COpinionReactionManager.h FABLE_RETAIL_GREETING_PASS23). Were local CCharString /
// CWorld / EOpinionAttitudeType models and the made-up extern g_OpinionAttitudeNames. The local CAIStateGroupBase model is pre-existing.
// NF56 (round 7, queue q10448): the class comes from engine/CAIStateGroup_OpinionReactionRoot.h (overlay opt-in
//   FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE: the retail class on its CAIStateGroupBase base), so the row defines the
//   PDB-private ?WifeFeelingUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z (Ego_r.pdb 0x009A3420 / FableWin.pdb
//   0x02C7E8F0) and calls the private ?SayToThingReactingTo@..@@AAE?AW4EAIActionReturn@@ABVCCharString@@@Z and the protected base
//   ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ through engine/CAIStateGroupBase.h; EAIActionReturn is engine/EAIAction.h. Were local
//   CAIStateGroupBase / CAIStateGroup_OpinionReactionRoot models (public members) and a local EAIActionReturn.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/EOpinion.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/CWorld.h"
class CTCOpinionSource;
#include "engine/COpinionReactionManager.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"

bool CAIStateGroup_OpinionReactionRoot::WifeFeelingUpdate(EOpinionAttitudeType attitude)
{
    CCharString text("TEXT_AI_WIFE_OPINION_ACT_FEELING_", -1);
    text += GetWorld().GetOpinionReactionManager().GetAttitudeString(attitude);
    return SayToThingReactingTo(text) == AI_ACTION_FINISHED;
}
