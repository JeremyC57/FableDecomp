// CAIStateGroup_JackDragon::IntoHover() -- with both hover markers alive and carrying a physics TC, build the hover-entry route
// for the side the dragon goes (GoLeft ? hover marker 1 : 2): an offset (0, -10.07981 - def.HoverBackOffOffset, 30.148378) and a
// second (0, -80, 30) back offset are rotated to the marker's facing angle (the second is never used again, but its RotateXY call
// stays), the dragon's YZ facing is levelled, the route runs from the dragon's position to marker position + offset, and the dragon
// starts flying it with a new CCreatureAction_DragonHoverInto; if the state group is no longer runnable it terminates and returns,
// otherwise (and when the markers are not usable) the state becomes DRAGON_STATE_HOVER (3, UpdateHover). The CJackDragonDef
// pointer is released at every exit.
// Leaf: ?IntoHover@CAIStateGroup_JackDragon@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00951D60, length 588 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CJackDragonDef> @ 0x008E3300 (same-TU COMDAT, real body below, noinline as in the landed
// 0x009500E0 OnActivate), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCPhysicsBase::GetFacingAngleXY (vslot 63, +0xFC),
// C3DVector::RotateXY @ 0x00A14040, CTCPhysicsBase::SetFacingAngleYZ (vslot 67, +0x10C), CThing::GetPos @ 0x004C73D0,
// vector<C3DVector>::push_back @ 0x0066E630, operator new @ 0x00BFEA1A, CCreatureAction_DragonHoverInto ctor @ 0x009534D0,
// CCountedPointer<CCreatureActionBase> ctor @ 0x00695880 / dtor @ 0x0048C5C0 (visible but called), StartFlying @ 0x00950C30,
// IsStillRunnable @ 0x0094FDF0, CAIStateGroupBase::Terminate @ 0x008FCF40, free @ 0x00BFEA14 (inline vector dtor).
// Shape notes: the marker select is `GoLeft ? PHoverMarker1.Get() : PHoverMarker2.Get()` (VC7.1 folds it into one ecx select and
// one GetPItem call; the `(cond ? a : b).Get()` spelling swaps the this/marker registers). The route point is summed component by
// component in source order (retail stores X, then Y, then Z); C3DVector::operator+ evaluates its ctor arguments Z first.
// Layout: Ego_r.pdb CAIStateGroup_JackDragon (packed, 0x140) and CJackDragonDef via the overlay opt-ins
// FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON / FABLE_RETAIL_CJACKDRAGONDEF (hdr/README.md).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CJackDragonDef.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_JackDragon.h"

// Same-TU body of the retail GetSubDef<CJackDragonDef> instance (0x008E3300, the landed body): with it VC7.1 knows the def
// pointer's frame word is not captured by the call.
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CJackDragonDef>(CDefPointer<const CJackDragonDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CJackDragonDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

// No shared header: RTTI .?AVCCreatureAction_DragonHoverInto@@ : CCombatActionBase (retail sizeof 0x114, `push 114h`), Ego_r.pdb
// ??0CCreatureAction_DragonHoverInto@@QAE@AAVCThingCreatureBase@@@Z (retail 0x009534D0).
class CCreatureAction_DragonHoverInto : public CCombatActionBase {
public:
    CCreatureAction_DragonHoverInto(CThingCreatureBase& creature);   // 0x009534D0
};

void CAIStateGroup_JackDragon::IntoHover()
{
    CDefPointer<const CJackDragonDef> def;
    GetCreature().PeekBaseDef().GetSubDef(&def);
    if (IS_THING_ALIVE(PHoverMarker1) && IS_THING_ALIVE(PHoverMarker2)
        && PHoverMarker1->GetPhysicsTC() && PHoverMarker2->GetPhysicsTC()) {
        CThing* marker = GoLeft ? PHoverMarker1.Get() : PHoverMarker2.Get();
        C3DVector offset(0.0f, -10.07981f - def->HoverBackOffOffset, 30.148378f);
        C3DVector back(0.0f, -80.0f, 30.0f);
        offset.RotateXY(marker->GetPhysicsTC()->GetFacingAngleXY());
        back.RotateXY(marker->GetPhysicsTC()->GetFacingAngleXY());
        GetCreature().GetPhysicsTC()->SetFacingAngleYZ(0.0f);
        std::vector<C3DVector> route;
        route.push_back(GetCreature().GetPos());
        {
            const C3DVector& pos = marker->GetPos();
            C3DVector hoverPos;
            hoverPos.X = offset.X + pos.X;
            hoverPos.Y = offset.Y + pos.Y;
            hoverPos.Z = offset.Z + pos.Z;
            route.push_back(hoverPos);
        }
        CCountedPointer<CCreatureActionBase> action(new CCreatureAction_DragonHoverInto(GetCreature()));
        StartFlying(GetCreature(), route, PreviousFlightPos, action);
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
    }
    CurrentState = DRAGON_STATE_HOVER;
}