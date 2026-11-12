// CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage(CDefPointer<const CMessageEventDef>, C3DVector, long) -- find the nearest
// guard (an AI creature with property 3) to this state group's creature; none -> AI_ACTION_FAILED. Otherwise flee to within 2.0 of
// him showing fear while no other yell for help is heard; a flee result other than AI_ACTION_FINISHED is returned as is. Then hand
// him the message event (CCreatureAction_SendMessage, performed and waited on): AI_ACTION_FAILED unless it finished, else FINISHED.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009810B0, 521 bytes (ret 14h).
// Leaf: ?AIAction_FindAGuardAndGiveMessage@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@V?$CDefPointer@$$CBVCMessageEventDef@@@@VC3DVector@@J@Z
// Callees: CThingPhysical::PeekPos 0x006A4D00, CAIStateGroupBase::PeekThingSearchTools 0x008FD000 / PeekWorld 0x008FCFA0,
//   CThingSearchTools::GetPBestLeafThing<CThingFilter_AICreatureHasProperty, CThingCompare_Nearest, CThingAICreature> 0x009823D0,
//   CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer
//   vtables 0x0123E7E0 <CThingAICreature>, 0x0125AAEC <const CThingCreatureBase>), CCreatureAction_ShowFear ctor 0x00842270,
//   CAIFilter_NoOtherYellForHelp (vtable 0x01299350), CAIStateGroupBase::AIAction_FleeToThingProximity 0x009804A0,
//   ~CCreatureActionBase 0x00693EF0, CCreatureAction_SendMessage ctor 0x00982210 (body below), AIAction_PerformActionAndWait 0x0097DB70;
//   the SendMessage ctor body calls CCreatureActionBase ctor 0x00693B30 and GetRandomAnimFromName 0x006924B0 (and CCharString 0x0099EBF0/0x0099EAE0).
// Shape: the same-TU CCreatureAction_SendMessage constructor (retail 0x00982210, its REAL body, `__declspec(noinline)`) must be
// visible: c2 then knows the by-value message handle's address does not escape through it, and releases the parameter in place
// (`mov ecx,[esp+0F4h] ; dec [ecx+4] ; jne`) while the SendMessage temporary's PDef keeps the load/dec/store form. The guard pointer
// is default-constructed then Set (the guard is read after the base ctor); the creature reference is named before the guard local
// and its position named before the search (store scheduling); the CIntelligentPointer<const CThingCreatureBase> argument of the
// flee is the inline converting constructor; ~CCreatureAction_SendMessage is the implicit inline one.
// Headers: overlay opt-ins FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD, FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF_DECL,
//   FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE, FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR, FABLE_RETAIL_CCREATUREACTION_SHOWFEAR_DERIVED.
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
__declspec(noinline) CCreatureAction_SendMessage::CCreatureAction_SendMessage(CThingCreatureBase& creature, const CDefPointer<const CMessageEventDef>& def,
        CThingCreatureBase* target, const C3DVector& pos, long extraData)
    : CCreatureActionBase(creature, -1.0f, 0.0f, GetRandomAnimFromName(creature, "STANDARD_IDLE"), CSoundPair(), 0, 0)
    , PDef(def), Pos(pos), PTarget(target), ExtraData(extraData)
{
}
// Ego_r.pdb: CAIFilter_NoOtherYellForHelp : CAIStateGroupBase::CAIFilterBase, 8 bytes, World +4; ??RCAIFilter_NoOtherYellForHelp@@UBE_NXZ.
class CAIFilter_NoOtherYellForHelp : public CAIStateGroupBase::CAIFilterBase {   // vtable 0x01299350
public:
    CAIFilter_NoOtherYellForHelp(const CWorld& world) : World(&world) { }
    virtual bool operator()() const;                        // 0x009825F0
private:
    const CWorld* World;                                    // +0x04
};
EAIActionReturn CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage(CDefPointer<const CMessageEventDef> message, C3DVector position,
                                                                    long parameter)
{
    const CThingAICreature& creature = GetCreature();
    CThingAICreature* guard = 0;
    const C3DVector& pos = creature.PeekPos();
    if (!PeekThingSearchTools().GetPBestLeafThing(CThingFilter_AICreatureHasProperty(3), CThingCompare_Nearest(pos), &guard))
        return AI_ACTION_FAILED;
    CIntelligentPointer<CThingAICreature> guardPointer;
    guardPointer.Set(guard);
    EAIActionReturn result = AIAction_FleeToThingProximity(guardPointer, 2.0f,
        CCreatureAction_ShowFear(GetCreature()), CAIFilter_NoOtherYellForHelp(PeekWorld()));
    if (result != AI_ACTION_FINISHED)
        return result;
    bool failed = AIAction_PerformActionAndWait(CCreatureAction_SendMessage(GetCreature(), message, guardPointer.Get(), position,
                                                                             parameter), false) != AI_ACTION_FINISHED;
    if (failed)
        return AI_ACTION_FAILED;
    return AI_ACTION_FINISHED;
}