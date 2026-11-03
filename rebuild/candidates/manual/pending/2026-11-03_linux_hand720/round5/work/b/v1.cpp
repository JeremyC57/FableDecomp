// Leaf: ?CalculateSecuritySweepTasks@CTCVillage@@AAEXXZ
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_GETSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_VILLAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PROPERTY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_MEMBERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMETIME
#define FABLE_RETAIL_CTCBASE_GETWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BEST
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_C3DVECTOR_NORMALISE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CVILLAGETASKPATROL_CTOR
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS_CTOR
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingMarker.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CVillageTaskSecuritySweep.h"
#include "fable_game.h"
#include <math.h>
// ---- TU-local models (no engine header) ----
class CThingCompare_Nearest {                            // Ego_r.pdb CThingCompare_Nearest { C3DVector Pos; } (12 bytes)
public:
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}   // ??0CThingCompare_Nearest@@QAE@ABVC3DVector@@@Z (inline)
    C3DVector Pos;
};
namespace std {                                          // STLport 4.6 <stl/_function.h>
template <class _Tp> struct logical_or : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x || __y; }
};
template <class _Operation1, class _Operation2, class _Operation3>
class binary_compose : public unary_function<typename _Operation2::argument_type, typename _Operation1::result_type> {
protected:
    _Operation1 _M_op1;
    _Operation2 _M_op2;
    _Operation3 _M_op3;
public:
    binary_compose(const _Operation1& __x, const _Operation2& __y, const _Operation3& __z) : _M_op1(__x), _M_op2(__y), _M_op3(__z) { }
};
template <class _Operation1, class _Operation2, class _Operation3>
inline binary_compose<_Operation1, _Operation2, _Operation3> __fastcall
compose2(const _Operation1& __op1, const _Operation2& __op2, const _Operation3& __op3)
{
    return binary_compose<_Operation1, _Operation2, _Operation3>(__op1, __op2, __op3);
}
}
struct HasGuardProperty {                                // FableWin.pdb UHasGuardProperty@@ (a struct); operator() 0x0065F152
    bool operator()(CIntelligentPointer<const CThing> thing) const;
};
namespace std {
// The out-of-line algorithm COMDATs the function calls (STLport 4.6 _algo.h; __fastcall as FableWin.pdb's YI spellings):
// count_if 0x0065F6A6 and find_if 0x0066000A over list<CIntelligentPointer<CThingAICreature> >::iterator.
template <class _InputIter, class _Predicate> long __fastcall count_if(_InputIter __first, _InputIter __last, _Predicate __pred);
template <class _InputIter, class _Predicate> _InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);
}
void CTCVillage::CalculateSecuritySweepTasks()
{
    long numberOfGuards = std::count_if(VillagePeople.begin(), VillagePeople.end(), HasGuardProperty());
    std::list<CIntelligentPointer<CThingAICreature> >::iterator guard =
        std::find_if(VillagePeople.begin(), VillagePeople.end(), HasGuardProperty());
    if (guard == VillagePeople.end())
        return;
    CTCCreatureNavigation* navigation = 0;
    (*guard)->GetTC(&navigation);
    CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!player)
        return;
    CThing* exit = 0;
    PositionPlayerWasInWhenSecuritySweepCalculated = player->GetPos();
    CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
    GetWorld().GetThingSearchTools().GetPBestThing(
        std::compose2(std::logical_or<bool>(), CThingFilter_HasTC(TCI_REGION_EXIT), CThingFilter_HasTC(TCI_TELEPORTER)),
        CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &exit);
    if (!exit)
        return;
    float radius = InitialSecuritySweepRadius -
        (float)((PeekWorld().GetFrame() - FrameStartedSecuritySweep) / CMainGameComponent::GetConstantFPS()) * 0.4f;
    CThingAICreature* nearestGuard;
    if (GetWorld().GetThingSearchTools().GetPBestThing(CThingFilter_HasTC(TCI_GUARD),
            CThingCompare_Nearest(PositionPlayerWasInWhenSecuritySweepCalculated), &nearestGuard)) {
        const C3DVector& guardPos = nearestGuard->GetPos();
        float dx = PositionPlayerWasInWhenSecuritySweepCalculated.X - guardPos.X;
        float dy = PositionPlayerWasInWhenSecuritySweepCalculated.Y - guardPos.Y;
        float distance = (float)sqrt(dx * dx + dy * dy);
        if (radius > distance) {
            InitialSecuritySweepRadius -= radius - distance;
            radius = distance;
        }
    }
    const float minimumRadius = 2.0f;
    if (SecuritySweepUrgent)
        radius = minimumRadius;
    if (radius < minimumRadius)
        radius = minimumRadius;
    C3DVector offset = exit->GetPos() - PositionPlayerWasInWhenSecuritySweepCalculated;
    if (offset.GetSquaredMagnitude2D() < radius * radius)
        radius = offset.GetMagnitude2D() * 0.8f;
    offset.Normalise();
    offset *= radius;
    float step = 1.0f / numberOfGuards;
    for (long i = 0; i < numberOfGuards; ++i) {
        float angle = 0.0f;
        C3DVector rotated = offset;
        C3DVector position;
        bool found = navigation->GetNearestNavigablePosTo(offset + PositionPlayerWasInWhenSecuritySweepCalculated, 5.0f, position);
        while (!found) {
            rotated = offset;
            if (angle < 0.0f)
                angle *= -1.0f;
            else {
                angle += step * 0.2f;
                if (!(angle < step))
                    break;
            }
            rotated.RotateXY(angle);
            found = navigation->GetNearestNavigablePosTo(rotated + PositionPlayerWasInWhenSecuritySweepCalculated, 5.0f, position);
        }
        if (found)
            SecuritySweepTasks.push_back(CCountedPointer<CVillageTaskSecuritySweep>(
                new CVillageTaskSecuritySweep(position, 5.0f, PositionPlayerWasInWhenSecuritySweepCalculated, 0.25f)));
        offset.RotateXY(step);
    }
}
