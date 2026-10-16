#include <cstdio>
// RemoveLOD: the LOD slot is cleared; with keep=true a stats-capable LOD moves into the empty stats slot.
struct LOD { int refs; bool UsedMeshStats; bool converted; };
struct MeshData { LOD* lods[4]; LOD* stats; };
static void RemoveLOD(MeshData& d, unsigned int lod, bool keep) {
    if (lod == 4) { if (d.stats) --d.stats->refs; d.stats = 0; return; }
    LOD* info = d.lods[lod]; if (info) ++info->refs;
    if (d.lods[lod]) --d.lods[lod]->refs; d.lods[lod] = 0;
    if (keep && info->UsedMeshStats && !d.stats) { d.stats = info; ++info->refs; info->converted = true; }
    if (info) --info->refs; }
int main() {
    LOD a = { 1, true, false }, b = { 1, false, false }; MeshData d = { { &a, &b, 0, 0 }, 0 };
    RemoveLOD(d, 1, true); RemoveLOD(d, 0, true);
    if (d.lods[0] || d.lods[1] || d.stats != &a || a.refs != 1 || !a.converted || b.refs != 0) { std::printf("00a249a0_TEST FAIL\n"); return 1; }
    RemoveLOD(d, 4, false);
    if (d.stats || a.refs != 0) { std::printf("00a249a0_TEST FAIL\n"); return 1; }
    std::printf("00a249a0_TEST PASS\n"); return 0; }
