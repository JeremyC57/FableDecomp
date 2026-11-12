// CCreatureAction_SendMessage::CCreatureAction_SendMessage(CThingCreatureBase&, const CDefPointer<const CMessageEventDef>&, CThingCreatureBase*, const C3DVector&, long) -- base CCreatureActionBase(creature, -1.0, 0.0, the creature's STANDARD_IDLE animation, zeroed CSoundPair, 0, 0), then copy the def pointer (add-ref), the position, the target intelligent pointer and the extra data
// Leaf: ??0CCreatureAction_SendMessage@@QAE@AAVCThingCreatureBase@@ABV?$CDefPointer@$$CBVCMessageEventDef@@@@PAV1@ABVC3DVector@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982210, length 194 bytes (+ int3 padding), `ret 14h`. Vtable 0x0129947C.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the animation lookup by name @ 0x006924B0 (`__fastcall`:
// ecx owner, edx the name), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*,
// const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the literal converts implicitly to a CCharString temporary built before every other base argument (GOTCHAS:
// a literal for a `const CCharString&` parameter is built first) and destroyed after the base constructor; after the
// vtable store the init-list members in declaration order: a CDefPointer copied from a reference (null-tested add-ref),
// a 12-byte record with a memberwise user copy constructor (dword moves, no `lea`), the CIntelligentPointer<CThing>
// (inline converting ctor: base ctor, vtable 0x0123E7D8, SetPItem) and the last argument.
// Layout: no engine header; Ego_r.pdb sizeof 0xd0 with unnamed members -- names are descriptive, offsets from the bytes
// (retail CCreatureActionBase is 0xa8).
// Naming (agentNF31, 2026-11-12): respelled from the local-model stand-in (CThingCreatureBase*, const CDefPointer<CDefBase>&, CThing*,
//   const SMessageData&, long) to the Ego_r.pdb 0x0084D3A0 / FableWin.pdb 0x02893E00 constructor, the symbol the landed caller
//   0x009810B0 AIAction_FindAGuardAndGiveMessage already binds; the class and body are that row's same-TU copy (shared headers
//   engine/CCreatureActionBase.h, CDefPointer.h, C3DVector.h, CIntelligentPointer.h). The animation lookup 0x006924B0 now binds
//   ?GetRandomAnimFromName@CCreatureActionBase@@KIABVCAnimationEntry@@ABVCThingCreatureBase@@ABVCCharString@@@Z (Ego_r 0x00518E00).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CCREATUREACTION_SHOWFEAR_DERIVED
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF_DECL
#include "engine/fable_ai_action_enums.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_AICreatureHasProperty.h"
#include "engine/CThingCompare_Nearest.h"

class CMessageEventDef;
class CWorld;

#include "engine/CSoundPair.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_ShowFear.h"
class CCreatureAction_SendMessage : public CCreatureActionBase {   // vtable 0x0129947C, retail 0xC4 bytes (no engine header; members as row 0x009822E0)
public:
    CCreatureAction_SendMessage(CThingCreatureBase& creature, const CDefPointer<const CMessageEventDef>& def,
        CThingCreatureBase* target, const C3DVector& pos, long extraData);   // 0x00982210
    CDefPointer<const CMessageEventDef>     PDef;           // +0xA8
    C3DVector                               Pos;            // +0xAC
    CIntelligentPointer<CThingCreatureBase> PTarget;        // +0xB8
    long                                    ExtraData;      // +0xC0
    virtual void PerformAction();
    virtual CCreatureActionBase* Clone() const;
};
CCreatureAction_SendMessage::CCreatureAction_SendMessage(CThingCreatureBase& creature, const CDefPointer<const CMessageEventDef>& def,
        CThingCreatureBase* target, const C3DVector& pos, long extraData)
    : CCreatureActionBase(creature, -1.0f, 0.0f, GetRandomAnimFromName(creature, "STANDARD_IDLE"), CSoundPair(), 0, 0)
    , PDef(def), Pos(pos), PTarget(target), ExtraData(extraData)
{
}
