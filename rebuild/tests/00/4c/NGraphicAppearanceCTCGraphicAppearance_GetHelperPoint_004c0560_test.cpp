// Behaviour test for 0x004C0560 CTCGraphicAppearance::GetHelperPoint: the same control flow over a small model of the meshes.
#include <cstdio>
struct Vec { float x, y, z; };
struct Mesh { unsigned long Index; bool loaded; int helpers; bool animated; int primary; };
static int g_begin_loading = 0;
static bool IsLoadedFor(Mesh& m, bool start) { bool l = m.loaded; if (!l && start) { ++g_begin_loading; } return l; }
static bool GetHelperPoint(Mesh* meshes, int count, int n, unsigned long flags, Vec& point, bool lod_ignorable, int* which)
{
    bool reduced_lod = false;
    bool force_accurate = true;
    if (flags & 1) force_accurate = false;
    bool ignore_animation = false;
    if (flags & 4) ignore_animation = true;
    if (!force_accurate && lod_ignorable) reduced_lod = true;
    // sort: primary first / last (stable two-pass for the model)
    Mesh sorted[8]; int k = 0;
    if (flags & 24) {
        int want = (flags & 8) ? 1 : 0;
        for (int i = 0; i < count; ++i) if (meshes[i].primary == want) sorted[k++] = meshes[i];
        for (int i = 0; i < count; ++i) if (meshes[i].primary != want) sorted[k++] = meshes[i];
    } else for (int i = 0; i < count; ++i) sorted[k++] = meshes[i];
    int it = 0;
    while (it != count) {
        Mesh& mi = sorted[it];
        if (mi.Index != 0) {
            bool start_loading = (flags & 2) == 0;
            bool return_if_not_loaded = !start_loading || (flags & 32) != 0;
            if (!IsLoadedFor(mi, start_loading) && return_if_not_loaded) return false;
            if (mi.helpers > 0) {
                if (n >= mi.helpers) return false;
                *which = (int)mi.Index;
                if (mi.animated && !ignore_animation) {
                    if (reduced_lod) { point.x = 1; point.y = 1; point.z = 1; }
                    else { point.x = 2; point.y = (float)n; point.z = 0; }
                } else { point.x = 3; point.y = (float)n; point.z = 0; }
                return true;
            }
        }
        ++it;
    }
    return false;
}
int main()
{
    Mesh m[3] = { {0, true, 5, true, 0}, {7, true, 0, false, 0}, {9, true, 2, true, 1} };
    Vec p = {0, 0, 0}; int which = 0; int fails = 0;
    if (!GetHelperPoint(m, 3, 1, 0, p, false, &which) || which != 9 || p.x != 2 || p.y != 1) ++fails;      // animated
    if (!GetHelperPoint(m, 3, 0, 4, p, false, &which) || p.x != 3) ++fails;                                   // ignore animation
    if (!GetHelperPoint(m, 3, 0, 1, p, true, &which) || p.x != 1) ++fails;                                    // reduced LOD
    if (GetHelperPoint(m, 3, 2, 0, p, false, &which)) ++fails;                                                // n >= count
    m[1].loaded = false;
    if (GetHelperPoint(m, 3, 0, 2, p, false, &which)) ++fails;                                                // not loaded, no start
    g_begin_loading = 0;
    if (!GetHelperPoint(m, 3, 0, 0, p, false, &which) || g_begin_loading != 1) ++fails;                       // starts loading, goes on
    if (GetHelperPoint(m, 3, 0, 32, p, false, &which)) ++fails;                                               // must be loaded
    m[1].loaded = true; m[1].helpers = 1;
    if (!GetHelperPoint(m, 3, 0, 8, p, false, &which) || which != 9) ++fails;                                 // primary first
    if (!GetHelperPoint(m, 3, 0, 16, p, false, &which) || which != 7) ++fails;                                // primary last
    Mesh none[1] = { {4, true, 0, false, 0} };
    if (GetHelperPoint(none, 1, 0, 0, p, false, &which)) ++fails;                                             // nobody has it
    if (fails) { std::printf("004c0560_TEST FAIL\n"); return 1; }
    std::printf("004c0560_TEST PASS\n");
    return 0;
}