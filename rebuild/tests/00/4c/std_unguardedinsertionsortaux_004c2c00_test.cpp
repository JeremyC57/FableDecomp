// Behaviour test for 004c2c00 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
struct CMeshInfo { int Index; float SizeX; unsigned char PrimaryMesh : 1; };
class CSortPrimaryMeshesLast { public: bool operator()(const CMeshInfo& a, const CMeshInfo&) const { return !a.PrimaryMesh; } };
template <class It, class T, class C> void linear_insert(It last, T val, C comp) { It next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val; }
template <class It, class T, class C> void insertion_sort_aux(It first, It last, T*, C comp) { for (It i = first; i != last; ++i) linear_insert(i, T(*i), comp); }
int main() {
    // primary meshes compare "not less" against anything, so a run of primaries is left in place
    CMeshInfo m[4]; for (int i = 0; i < 4; ++i) { m[i].Index = i; m[i].SizeX = 1.0f * i; m[i].PrimaryMesh = 1; }
    insertion_sort_aux(m + 1, m + 4, (CMeshInfo*)0, CSortPrimaryMeshesLast());
    for (int i = 0; i < 4; ++i) if (m[i].Index != i) { std::printf("004c2c00_TEST FAIL\n"); return 1; }
    CSortPrimaryMeshesLast c; CMeshInfo np; np.PrimaryMesh = 0;
    if (!c(np, m[0]) || c(m[0], np)) { std::printf("004c2c00_TEST FAIL\n"); return 1; }
    { std::printf("004c2c00_TEST PASS\n"); return 0; }
}