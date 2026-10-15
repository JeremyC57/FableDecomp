// Behaviour test for 0x00bddc00 (CQuadTreeElement::ClearCache (full reset)): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
static int g_deleted = 0; static int g_released = -1;
struct Coll { Coll* Next; Coll(Coll* n) : Next(n) {} virtual ~Coll() { ++g_deleted; } };
struct Mgr { void ReleaseRequest(long pos) { g_released = pos; } };
struct Elem {
    Coll* Colls; Elem* Children[4]; long FileBlockPos, FileBlockSize, Offset;
    bool HasDyn, RenderUpd, Loaded, Loading;
    Elem() : Colls(0), FileBlockPos(0), FileBlockSize(0), Offset(0), HasDyn(false), RenderUpd(false), Loaded(false), Loading(false) { for (int i = 0; i < 4; ++i) Children[i] = 0; }
    virtual ~Elem() { ++g_deleted; }
    void RemoveObjectCollections() { while (Colls) { Coll* n = Colls->Next; delete Colls; Colls = n; } }
};
static Elem* Make() { Elem* e = new Elem; e->Loading = true; e->FileBlockPos = 42; e->FileBlockSize = 7; e->Offset = 3; e->HasDyn = e->RenderUpd = e->Loaded = true;
    e->Colls = new Coll(new Coll(0)); e->Children[0] = new Elem; e->Children[2] = new Elem; return e; }
static void ClearCache(Elem* e, Mgr* m)
{
    if (e->Loading) { m->ReleaseRequest(e->FileBlockPos); e->Loading = false; }
    for (long i = 0; i < 4; ++i) { delete e->Children[i]; e->Children[i] = 0; }
    e->RemoveObjectCollections(); e->FileBlockPos = 0; e->FileBlockSize = 0; e->Offset = 0; e->HasDyn = false; e->RenderUpd = false; e->Loaded = false;
}

int main()
{
    Mgr m; Elem* e = Make();
    ClearCache(e, &m);
    CHECK(g_released == 42 && !e->Loading && g_deleted == 4 && e->Colls == 0); for (int i = 0; i < 4; ++i) CHECK(e->Children[i] == 0);
    CHECK(!e->HasDyn && !e->Loaded && !e->RenderUpd && e->FileBlockPos == 0 && e->FileBlockSize == 0 && e->Offset == 0);
    delete e;
    if (g_fail) { std::printf("00bddc00_TEST FAIL\n"); return 1; }
    std::printf("00bddc00_TEST PASS\n");
    return 0;
}