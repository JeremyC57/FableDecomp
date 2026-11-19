// Behaviour test for 0x006E8EE0 (anonymous namespace)::GetClosestUnassignedProng: the row's defines, includes and body verbatim. Two
// rings of three prongs (offsets from the centre); the search picks, among ring_num's unassigned prongs, the one nearest to the search
// position in 2D (Z ignored; a tie keeps the first: strict <), returns 0 when every prong of the ring is assigned, and returns 0 for a
// ring number whose (ring_num + 1) * 3 is negative or above 0x7fffffff (the 64-bit check) without touching the vectors.
#include <stdio.h>
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

static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
int main()
{
    std::vector<C3DVector> prongs;
    prongs.push_back(V(1, 0, 0)); prongs.push_back(V(0, 1, 0)); prongs.push_back(V(-1, 0, 0));      // ring 0
    prongs.push_back(V(2, 0, 0)); prongs.push_back(V(0, 2, 0)); prongs.push_back(V(-2, 0, 50));     // ring 1
    std::vector<bool> assigned;
    for (int i = 0; i < 6; ++i) assigned.push_back(false);
    C3DVector centre = V(10, 20, 0);
    // ring 1: search near centre + (-2,0): prong 5 (its Z of 50 is ignored)
    check(g_pGetClosestUnassignedProng(prongs, centre, 1, assigned, V(7.5f, 20, -100)) == 5, "nearest in 2D, Z ignored");
    // ring 1 with prong 5 assigned: prong 3 (8.25 to 12) v prong 4 (distance 2.0^2+... ) -> nearest unassigned
    assigned[5] = true;
    check(g_pGetClosestUnassignedProng(prongs, centre, 1, assigned, V(7.5f, 20, 0)) == 4, "assigned prong skipped");
    // ring 0: search at the centre: all three at distance 1 -> the first (index 0) wins the tie
    check(g_pGetClosestUnassignedProng(prongs, centre, 0, assigned, centre) == 0, "tie keeps the first (strict <)");
    // ring 0: prong 1 strictly nearest; prong 0 assigned
    assigned[0] = true;
    check(g_pGetClosestUnassignedProng(prongs, centre, 0, assigned, V(10, 21.5f, 0)) == 1, "ring 0 nearest");
    // the centre is added to the prong: searching at the raw prong offset (0,1) picks the prong nearest to it after the offset
    check(g_pGetClosestUnassignedProng(prongs, centre, 0, assigned, V(9.5f, 20, 0)) == 2, "centre offset applied");
    // every prong of ring 0 assigned -> 0
    assigned[1] = true; assigned[2] = true;
    check(g_pGetClosestUnassignedProng(prongs, centre, 0, assigned, centre) == 0, "none free -> 0");
    // ring 1: only prong 3 free, far away -> still found (no distance limit below 1e33)
    assigned[4] = true;
    check(g_pGetClosestUnassignedProng(prongs, centre, 1, assigned, V(1e15f, 0, 0)) == 3, "far prong still found");
    // overflow / negative checks return 0 before reading the vectors
    std::vector<C3DVector> empty; std::vector<bool> none;
    check(g_pGetClosestUnassignedProng(empty, centre, 0x7fffffff, none, centre) == 0, "(ring+1)*3 above 0x7fffffff -> 0");
    check(g_pGetClosestUnassignedProng(empty, centre, -2, none, centre) == 0, "negative (ring+1)*3 -> 0");
    check(g_pGetClosestUnassignedProng(empty, centre, 0x2aaaaaaa, none, centre) == 0, "(ring+1)*3 = 0x80000001 -> 0");
    if (!g_fail) printf("006e8ee0_TEST PASS\n");
    return g_fail;
}