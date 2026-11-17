// IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> >(const CIntelligentPointer<CThingCreatureBase>& thing) -- GetPItem @ 0x00A01B50 twice:
// null, then the dead bit (+0x91 & 1)
// Leaf: ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingCreatureBase@@@@@@YI_NABV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006961F0, 36 bytes, __fastcall (the reference in ecx).
// Retail bytes:  push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// NF72 (round 7, q10887): FableWin.pdb 0x01DBD600 names this instantiation (Ego_r.pdb lists other IS_THING_ALIVE instances, not this one; the
//   queue evidence said Ego_r); the ported CAIStateGroup_CreatureTakeCover::Update 0x0097A630 (NF68) binds it through engine/ThingAlive.h on
//   PTargetThing. Retail has no ICF, so 0x006961F0 is this one instance. Was the stand-in IsThingAlive_006961f0(CIntelligentPointer<CThing>&);
//   now the explicit instantiation of engine/ThingAlive.h's IS_THING_ALIVE with the 0x0097A630 macro block.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TYPED_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TAKECOVER
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_ISRUNNABLELEAF
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_UPDATE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_SETASDIRECTLYAWARE
#define FABLE_RETAIL_CAISTATEGROUPBASE_TARGETFACE
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CCREATUREACTION_PROJECTILECROUCH_CLASS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "rebuild_abi.h"
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/NCreatureMode.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CAIStateGroup_CreatureTakeCover.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/GFRandom.h"
#include "fable_string.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CCreatureAction_ProjectileCrouch.h"

template bool FABLE_FASTCALL IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> >(const CIntelligentPointer<CThingCreatureBase>& thing);
