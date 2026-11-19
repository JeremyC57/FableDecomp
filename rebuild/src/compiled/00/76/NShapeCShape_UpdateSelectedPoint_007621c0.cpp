// NShape::CShape::UpdateSelectedPoint(const C3DVector& pos) -- select-point edit step: add every outline point within 0.25 (2D) of pos to the selection once; true when one was hit, else clear the selection and return false
// Leaf: ?UpdateSelectedPoint@CShape@NShape@@QAE_NABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007621C0, 241 bytes (+ padding; graded 243), thiscall, `ret 4`.
// Name: Ego_r.pdb / FableWin.pdb ?UpdateSelectedPoint@CShape@NShape@@QAE_NABVC3DVector@@@Z (FableWin debug\tc_geometric_shapes.obj #100,
//   0x025DC6E0, before UpdateDeletePoint #101 = retail 0x00762010).
// Callees: STLport __find<vector<C3DVector>::iterator*, ..> 0x00764080 (out of line, `__fastcall`, value and tag by reference), the
//   selection vector's _M_insert_overflow 0x00765130 (push_back's overflow path), memmove 0x00BFEAE6 (clear); 0.0625f 0x0122DC98.
// Source: Ego_d.pdb locals this, pos, success; {it {this_point {it_already_there}}}; FableWin.pdb debug body 0x025DC6E0 gives the calls:
//   Points.begin()/end() iterator loop, `C3DVector this_point = *it`, GFWorld2DDistanceSmallerThan(this_point, pos, 0.25f, false) (FableWin
//   0x01F284E0, engine/GFWorldDistance.h), std::find over SelectedPoints, push_back(it) when not found, success = true; clear() on no hit.
// Headers: the shared engine/NShape_CClosedShapeBase.h (NShape::CShape: Owner, Points, SelectedPoints = vector<vector<C3DVector>::iterator>)
//   with a new declaration opt-in FABLE_RETAIL_NSHAPE_CSHAPE_UPDATESELECTEDPOINT (hdr/README.md); STLport find through
//   FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH + FABLE_RETAIL_STLP_FIND_NOINLINE_BODY (the visible, called __find: the tag homes on a live
//   argument slot, as retail 0x00762212). Ported from the old local model: RELOCATION_MATCH at the second grade.
#define FABLE_RETAIL_NSHAPE_CSHAPE_UPDATESELECTEDPOINT
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/NShape_CClosedShapeBase.h"
#include "engine/GFWorldDistance.h"
bool NShape::CShape::UpdateSelectedPoint(const C3DVector& pos)
{
    bool success = false;
    for (std::vector<C3DVector>::iterator it = Points.begin(); it != Points.end(); ++it) {
        C3DVector this_point = *it;
        if (GFWorld2DDistanceSmallerThan(this_point, pos, 0.25f, false)) {
            std::vector<std::vector<C3DVector>::iterator>::iterator it_already_there = std::find(SelectedPoints.begin(), SelectedPoints.end(), it);
            if (it_already_there == SelectedPoints.end())
                SelectedPoints.push_back(it);
            success = true;
        }
    }
    if (!success)
        SelectedPoints.clear();
    return success;
}