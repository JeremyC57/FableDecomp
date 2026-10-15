// Behaviour test for 0x00bddfb0 (CQuadTreeElement::~CQuadTreeElement): the reconstructed logic run over a host model.
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
struct Map { Mgr* FileBlockManager; };
struct Elem2 : Elem { Map* map; ~Elem2()
    { if (Loading) { map->FileBlockManager->ReleaseRequest(FileBlockPos); Loading = false; } RemoveObjectCollections();
      delete Children[0]; delete Children[1]; delete Children[2]; delete Children[3]; } };

int main()
{
    Mgr m; Map map; map.FileBlockManager = &m;
    Elem2* e = new Elem2; e->map = &map; e->Loading = true; e->FileBlockPos = 9; e->Colls = new Coll(0); e->Children[1] = new Elem; e->Children[3] = new Elem;
    delete e;
    CHECK(g_released == 9 && g_deleted == 4);   // one collection, two children, the element itself
    if (g_fail) { std::printf("00bddfb0_TEST FAIL\n"); return 1; }
    std::printf("00bddfb0_TEST PASS\n");
    return 0;
}