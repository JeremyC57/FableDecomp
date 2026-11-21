// CTCFlammable::GetClosestFirePointTo(const C3DVector&, bool) -- the fire point whose helper point lies nearest to pos (end() with no points, begin() with one; unlit_only is not read)
// Leaf: ?GetClosestFirePointTo@CTCFlammable@@AAEPAVCFirePoint@NFire@@ABVC3DVector@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00780430, 271 bytes, thiscall, `ret 8`. Caller: CTCFlammable::AddFireDamage 0x00780860 (`push 0 ; push pos`).
// Identity: Ego_r.pdb ?GetClosestFirePointTo@CTCFlammable@@AAE?AV?$_Vector_iterator@..@@ABVC3DVector@@_N@Z (private; Ego_r 0x005945C0,
//   tc_flammable.obj between SetOnFire and SetAsSmoldering); retail STLport's vector<NFire::CFirePoint>::iterator is the raw pointer (the
//   0x2C stride divide at 0x0078043D). Ego_d.pdb (debug length 1542) parameters pos / unlit_only, locals num_fp, best_point, it_best,
//   best_pt_dist_sqr, ptc_grap_app (const NGraphicAppearance::CTCGraphicAppearance*, TPI 0xB872), it, and in the loop test_fire_point
//   (NFire::CFirePoint&), test_point, test_pt_dist_sqr.
// Callees: NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector&, float) const 0x004C0560.
// Ego_d lines (VC10 /Od): 363 (17 B) num_fp = FirePoints.size(); 364 / 368 (6 B) the two tests; 375 (21 B) ptc_grap_app =
//   GetParentThing().GetTCGraphicAppearance() (two calls; the direct ParentThing-> read is 17 B); 377 (25 B) `it = FirePoints.begin()`
//   (begin into the local + the first EH state, 7 B); 378 (18 B) `it_best = it++` (operator++(int) returned straight into it_best, 14 B,
//   + EH state 4 B: retail's `mov ebp,esi ; add esi,2Ch` before the first GetHelperPoint, 0x00780480 / 0x0078048D); 380 / 388 (8 B) the
//   C3DVector default constructions; 382 (26 B) / 391 (29 B) the squared distances from a pos - point temporary; 384 (113 B) the for
//   loop with no init statement (its condition is IsShapeConvex 313's 100-B end() test + 3 B for the member vector, + the increment
//   call 8 B + a short jump); 386 (11 B) the test_fire_point reference; 394 (12 B) `it_best = it` (operator= call) BEFORE 395 (6 B)
//   the distance store; 397 (5 B) the loop jump.
// Inferred: Ego_d lines 381 (482 B) and 389 (520 B) hold the two GetHelperPoint calls (~45 B each at /Od) inside a debug-only check;
//   the release bytes are the plain calls (0x00780490 / 0x007804E5), written as such (as the twin lines 50 / 71 of the landed
//   SetAsBurning 0x00780540 / SetAsSmoldering 0x007805D0).
// Header: FABLE_RETAIL_CTCFLAMMABLE_GETCLOSESTFIREPOINTTO (new, add-only) declares the private member in engine/CTCFlammable.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS
#define FABLE_RETAIL_CTCFLAMMABLE_GETCLOSESTFIREPOINTTO
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NFire_CFirePoint.h"
#include "engine/CTCFlammable.h"

std::vector<NFire::CFirePoint>::iterator CTCFlammable::GetClosestFirePointTo(const C3DVector& pos, bool unlit_only)
{
    long num_fp = FirePoints.size();
    if (num_fp == 0)
        return FirePoints.end();
    if (num_fp == 1)
        return FirePoints.begin();
    const NGraphicAppearance::CTCGraphicAppearance* ptc_grap_app = GetParentThing().GetTCGraphicAppearance();
    std::vector<NFire::CFirePoint>::iterator it = FirePoints.begin();
    std::vector<NFire::CFirePoint>::iterator it_best = it++;
    C3DVector best_point;
    ptc_grap_app->GetHelperPoint(it_best->HelperName, it_best->HelperIndex, 0, best_point, 1.0f);
    float best_pt_dist_sqr = (pos - best_point).GetSquaredMagnitude();
    for (; it != FirePoints.end(); ++it) {
        NFire::CFirePoint& test_fire_point = *it;
        C3DVector test_point;
        ptc_grap_app->GetHelperPoint(test_fire_point.HelperName, test_fire_point.HelperIndex, 0, test_point, 1.0f);
        float test_pt_dist_sqr = (pos - test_point).GetSquaredMagnitude();
        if (test_pt_dist_sqr < best_pt_dist_sqr) {
            it_best = it;
            best_pt_dist_sqr = test_pt_dist_sqr;
        }
    }
    return it_best;
}