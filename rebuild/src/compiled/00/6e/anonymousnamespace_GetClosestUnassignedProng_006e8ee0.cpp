// (anonymous namespace)::GetClosestUnassignedProng(const std::vector<C3DVector>& prongs, const C3DVector& prong_centre, long ring_num, std::vector<bool>& prong_assignments, const C3DVector& search_pos) -- of ring ring_num's three prongs not yet assigned, the index of the one whose position (prong offset + centre) is nearest to the search position in 2D; 0 when none (or when (ring_num + 1) * 3 overflows a long)
// Leaf: ?GetClosestUnassignedProng@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8EE0, 605 bytes (+ padding), __fastcall (prongs in ecx, prong_centre in edx), `ret 0Ch`.
// Identity (IDENTIFY row; was the descriptive catalog label FindNearestIndexToPoint): combat_manager.obj, between
//   GetFirstAttackerAlignedWithPlacementVector (retail 0x006E8E10) and CProng::SetBlocked (0x006E9140) in both retail and Ego_r.pdb's
//   object order (Ego_r #50 GetBestAttackerForPlacementVector = retail 0x006E8CE0 landed, #51, #52 this). Ego_r.pdb 0x00712240 and
//   FableWin.pdb 0x0241A500 ?GetClosestUnassignedProng@?A0xd8b789b9@@YIJABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@
//   ABVC3DVector@@JAAV?$vector@_NV?$allocator@_N@std@@@3@1@Z (fastcall, long return, the parameters as retail's: vector, vector, long,
//   vector<bool> by reference, vector). Retail's prong_assignments is a byte vector (`cmp byte ptr [ecx+ebp],0`), as the shared
//   STLport model (no bit specialisation).
// Callees: __allmul 0x00BFEB50 (the 64-bit overflow check).
// Shape: Ego_d.pdb's record (debug length 1132 = FableWin's): closest_distance_sq / closest_index / overflow_check, the loop counter
//   c1, and distance_sq / prong_pos in the loop body; FableWin's /Od body gives the statements: prong_pos = prongs[c1] + prong_centre
//   (C3DVector::operator+ on the vector element: retail loads the prong first, `fld [eax+edi] ; fadd [esi]`), distance_sq =
//   (prong_pos - search_pos).GetSquaredMagnitude2D() with the one-expression body (FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23:
//   retail squares Y on the stack before the spilled X, 0x006E8F90), and the debug build's two asserts (dead in release). Ego_d's
//   `no_prongs = prongs.size()` feeds only the first assert: omitted (same bytes with or without it).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
namespace {
long FABLE_FASTCALL GetClosestUnassignedProng(const std::vector<C3DVector>& prongs, const C3DVector& prong_centre, long ring_num,
                                              std::vector<bool>& prong_assignments, const C3DVector& search_pos)
{
    long closest_index = -1;
    float closest_distance_sq = 1e33f;
    __int64 overflow_check = ((__int64)ring_num + 1) * 3;
    if (overflow_check < 0 || overflow_check > 0x7fffffff)
        return 0;
    for (long c1 = ring_num * 3; c1 < (ring_num + 1) * 3; ++c1) {
        if (!prong_assignments[c1]) {
            C3DVector prong_pos = prongs[c1] + prong_centre;
            float distance_sq = (prong_pos - search_pos).GetSquaredMagnitude2D();
            if (distance_sq < closest_distance_sq) {
                closest_distance_sq = distance_sq;
                closest_index = c1;
            }
        }
    }
    if (closest_index == -1)
        closest_index = 0;
    return closest_index;
}
}
long (FABLE_FASTCALL* g_pGetClosestUnassignedProng)(const std::vector<C3DVector>&, const C3DVector&, long, std::vector<bool>&, const C3DVector&) = &GetClosestUnassignedProng;