#include <cstdio>
// Behaviour copy of C3DMeshInfo::GetMesh (0x00A25E70): requested LOD, else start load / wait, else lower then higher loaded LOD, else block.
struct LOD { int mesh; int touched; int built; };
struct Info { LOD* lods[4]; unsigned long sizes[4]; int hasData; int loads; };
static LOD g_pool[8]; static int g_used = 0;
static bool Present(const Info& m, unsigned long i) { return i < 4 && m.sizes[i] != 0; }
static LOD* GetLOD(const Info& m, unsigned long i) { return m.hasData ? m.lods[i] : 0; }
static void BeginLoading(Info& m, unsigned long lod) { m.hasData = 1; if (!m.lods[lod]) { m.lods[lod] = &g_pool[g_used++]; m.lods[lod]->mesh = 0; } m.loads++; }
static bool Copy(LOD* p, int& out) { if (p->mesh) { out = p->mesh; return true; } return false; }
static bool GetMesh(Info& m, int& out, unsigned long lod, unsigned long flags) {
    if (lod > 3) lod = 3;
    while (m.sizes[lod] == 0) --lod;
    LOD* info = GetLOD(m, lod);
    if (info) { info->touched++; if (Copy(info, out)) return true; }
    if (!(flags & 4)) {
        BeginLoading(m, lod);
        info = GetLOD(m, lod);
        if (flags & 0x10) {
            if (flags & 3) { info->built++; info->mesh = 50 + (int)lod; return Copy(info, out); }
            return false;
        }
    }
    if (m.hasData) {
        for (long i = (long)lod - 1; i >= 0; --i) { LOD* p = m.lods[i]; if (p && p->mesh) { p->touched++; return Copy(p, out); } }
        for (unsigned long j = lod + 1; Present(m, j); ++j) { LOD* p = m.lods[j]; if (p && p->mesh) { p->touched++; return Copy(p, out); } }
    }
    if ((flags & 5) == 1) { info->built++; info->mesh = 70 + (int)lod; return Copy(info, out); }
    return false;
}
int main() {
    Info m = {{0, 0, 0, 0}, {10, 10, 0, 0}, 0, 0};
    int out = 0;
    bool ok = !GetMesh(m, out, 3, 0) && m.loads == 1 && m.lods[1] != 0;
    ok = ok && GetMesh(m, out, 1, 0x11) && out == 51 && m.loads == 2;
    out = 0;
    ok = ok && GetMesh(m, out, 9, 4) && out == 51 && m.lods[1]->touched == 2;
    ok = ok && GetMesh(m, out, 0, 0) && out == 51 && m.lods[0] != 0 && m.lods[1]->touched == 3;
    ok = ok && GetMesh(m, out, 0, 1) && out == 51;
    m.lods[1]->mesh = 0;
    ok = ok && GetMesh(m, out, 0, 1) && out == 70 && m.lods[0]->built == 1;
    if (!ok) { std::printf("00a25e70_TEST FAIL\n"); return 1; }
    std::printf("00a25e70_TEST PASS\n"); return 0;
}
