// CAIStateGroup_OpinionReactionRoot::WifeGeneralUpdate(EOpinionAttitudeType attitude) -- say "TEXT_AI_WIFE_OPINION_ACT_GENERAL_" + the attitude's name to the thing being reacted to; true once the speech action has finished (EAIActionReturn 2)
// Leaf: WifeGeneralUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982F10, length 84 bytes.
// Identity: the catalog label "CPlayer::SayWifeOpinionGeneral" is a guess; Ego_r.pdb has
// `?WifeGeneralUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z` between
// WifeFeelingUpdate and WifeToHerselfUpdate, 0x60 bytes apart, exactly like 0x00982EB0 / F10 / F70,
// and the body calls the class's SayToThingReactingTo on `this`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the state group's getter
// @ 0x008FCFC0 (returns this->[+4]->[+0x1C]->[+0x24]; called for nothing: the result is dropped;
// manifest label "CPlayer::GetCurrentMode"), CCharString::operator+=(const CCharString&) @
// 0x0099F0A0, CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo(const CCharString&) @
// 0x0095EE70, CCharString::~CCharString @ 0x0099EAE0; the attitude-name table @ 0x013BABD0
// (CCharString per EOpinionAttitudeType); the literal @ 0x01299940.
// Shape: a named CCharString local in the `push ecx` slot (explicit (literal, -1) constructor),
// the bool `== 2` kept in bl across the destructor (`sete bl ; ... mov al,bl`), `ret 4`.
// NF52 (round 7): the attitude names are the private static COpinionReactionManager::AttitudeStrings
// (?AttitudeStrings@COpinionReactionManager@@0QBVCCharString@@B, 0x013BABD0; both PDBs' dynamic initializer / atexit destructor), read
// through FableWin.pdb's public inline ?GetAttitudeString@COpinionReactionManager@@QBEABVCCharString@@W4EOpinionAttitudeType@@@Z on the
// world's manager (FableWin ?GetOpinionReactionManager@CWorld@@QAEAAVCOpinionReactionManager@@XZ, inline): that is why the GetWorld()
// call survives with its result unused. CCharString / CWorld / COpinionReactionManager / EOpinionAttitudeType come from their shared
// headers (fable_string.h, engine/CWorld.h, engine/COpinionReactionManager.h FABLE_RETAIL_GREETING_PASS23). Were local CCharString /
// CWorld / EOpinionAttitudeType models and the made-up extern g_OpinionAttitudeNames. The local CAIStateGroupBase model is pre-existing.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_GREETING_PASS23
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/EOpinion.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/CWorld.h"
class CTCOpinionSource;
#include "engine/COpinionReactionManager.h"
enum EAIActionReturn { AI_ACTION_RETURN_RUNNING = 0, AI_ACTION_RETURN_FAILED = 1, AI_ACTION_RETURN_FINISHED = 2 };

class CAIStateGroupBase { protected:   // engine/CAIStateGroupBase.h flattens the vptr: the getter only
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    EAIActionReturn SayToThingReactingTo(const CCharString& text);   // 0x0095EE70
    bool WifeGeneralUpdate(EOpinionAttitudeType attitude);
};

bool CAIStateGroup_OpinionReactionRoot::WifeGeneralUpdate(EOpinionAttitudeType attitude)
{
    CCharString text("TEXT_AI_WIFE_OPINION_ACT_GENERAL_", -1);
    text += GetWorld().GetOpinionReactionManager().GetAttitudeString(attitude);
    return SayToThingReactingTo(text) == AI_ACTION_RETURN_FINISHED;
}