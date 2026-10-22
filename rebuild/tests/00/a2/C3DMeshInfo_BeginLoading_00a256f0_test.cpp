#include <cstdio>
// Behaviour copy of C3DMeshInfo::BeginLoading (0x00A256F0): clamp/walk the LOD, touch a loaded one, else start its read.
struct Info { unsigned long lod; unsigned long offset; unsigned long size; int priority; int touched; };
struct Mesh { int hasData; unsigned long id; Info* lods[4]; unsigned long sizes[4]; };
static unsigned long g_next = 7;
static int g_prio = 2;
static Info g_pool[8]; static int g_used = 0;
static void BeginLoading(Mesh& m, unsigned long lod, bool urgent) {
    if (lod > 3) lod = 3;
    while (m.sizes[lod] == 0) --lod;
    if (m.hasData && m.lods[lod]) { m.lods[lod]->touched++; return; }
    unsigned long size = m.sizes[lod];
    if (!m.hasData) { m.hasData = 1; unsigned long id = g_next++; m.id = id; }
    unsigned long offset = 0;
    for (unsigned long i = 0; i < lod; ++i) offset += m.sizes[i];
    Info* p = &g_pool[g_used++];
    p->lod = lod; p->offset = offset; p->size = size; p->touched = 0;
    int priority = g_prio;
    if (urgent) priority = 4;
    p->priority = priority;
    m.lods[lod] = p;
}
int main() {
    Mesh m = {0, 0, {0, 0, 0, 0}, {100, 50, 0, 0}};
    BeginLoading(m, 9, false);
    bool ok = m.hasData == 1 && m.id == 7 && g_next == 8 && m.lods[1] && m.lods[1]->offset == 100 && m.lods[1]->size == 50 && m.lods[1]->priority == 2;
    BeginLoading(m, 2, true);
    ok = ok && g_used == 1 && m.lods[1]->touched == 1;
    BeginLoading(m, 0, true);
    ok = ok && g_used == 2 && m.lods[0]->offset == 0 && m.lods[0]->size == 100 && m.lods[0]->priority == 4 && g_next == 8;
    if (!ok) { std::printf("00a256f0_TEST FAIL\n"); return 1; }
    std::printf("00a256f0_TEST PASS\n"); return 0;
}