// Behaviour test for 0x00B94540 CEngineSceneMap::ResizeLevels (links the REAL row object: the row body plus the TU's COMDATs it
// carries -- the expanded CEngineSceneGrid constructor and its GridCells.resize, CCountedPointer<CEngineSceneGrid>::DeleteData, the
// vector's inline push_back / _M_insert_overflow / pop_back, the noinline __destroy_aux range destructor, the CEngineSceneGrid vtable).
// Fixtures (native layout): the map (GridLevels STLport vector of 8-byte {Data, Info} counted pointers at +0x18); grids of 0x20 bytes
// (vptr, Level +4, LevelUp +8, LevelDown +0xC, Size +0x10, GridCells vector +0x14); count blocks {RefCount, DeleteFunc, Data}.
// operator new / delete are malloc / free with a live counter; CBaseClassNonCopyable's ctor marks the object and its dtor records the
// deletion (the TU's own scalar deleting destructor); _M_fill_insert appends n copies of the value (malloc'd storage);
// MovePrimitivesDown / MovePrimitivesUp record the call order. Reference model: each run checks the level chain element by element.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }

#pragma comment(linker, "/alternatename:@CallRow@12=?ResizeLevels@CEngineSceneMap@@AAEXJ@Z")
#pragma comment(linker, "/alternatename:??2@YAPAXI@Z=_StubNew")
#pragma comment(linker, "/alternatename:??3@YAXPAX@Z=_StubDelete")
#pragma comment(linker, "/alternatename:??0CBaseClassNonCopyable@@IAE@XZ=@StubBaseCtor@8")
#pragma comment(linker, "/alternatename:??1CBaseClassNonCopyable@@MAE@XZ=@StubBaseDtor@8")
#pragma comment(linker, "/alternatename:?MovePrimitivesDown@CEngineSceneGrid@@AAEXXZ=@StubDown@8")
#pragma comment(linker, "/alternatename:?MovePrimitivesUp@CEngineSceneGrid@@AAEXXZ=@StubUp@8")
#pragma comment(linker, "/alternatename:?_M_fill_insert@?$vector@PAVCEngineSceneGridCell@@V?$allocator@PAVCEngineSceneGridCell@@@std@@@std@@QAEXPAPAVCEngineSceneGridCell@@IABQAV3@@Z=@StubFillInsert@20")
#pragma comment(linker, "/alternatename:_GridVtable=??_7CEngineSceneGrid@@6B@")
#pragma comment(linker, "/alternatename:@GridDeleteData@4=?DeleteData@?$CCountedPointer@VCEngineSceneGrid@@@@SIXPAX@Z")
extern "C" void* GridVtable[];
extern "C" void __fastcall GridDeleteData(void*);

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct Block { unsigned long refs; void (__fastcall* del)(void*); void* data; };
struct Vec { void** start; void** finish; void** eos; };
struct Grid { void** vptr; unsigned long level; Grid* up; Grid* down; unsigned long size; Vec cells; };
struct CP { Grid* data; Block* info; };
struct Map { void* vptr; float area[4]; unsigned long maxLevels; CP* start; CP* finish; CP* eos; void* scene; };

static int g_live = 0, g_baseCtor = 0, g_deleted = 0;
static Grid* g_order[32]; static char g_kind[32]; static int g_nOrder = 0;
extern "C" void* __cdecl StubNew(unsigned int n) { ++g_live; return malloc(n); }
extern "C" void __cdecl StubDelete(void* p) { if (p) { --g_live; free(p); } }
static void* const kBaseMark = (void*)0x5A5A5A5A;
extern "C" void* __fastcall StubBaseCtor(void* self, void*) { ++g_baseCtor; *(void**)self = kBaseMark; return self; }
// CBaseClassNonCopyable's destructor: reached from the TU's own ??_GCEngineSceneGrid (slot 0, via DeleteData) after the inline
// ~CEngineSceneGrid has released GridCells; it records the grid; ??_G then frees the object through operator delete (counted).
extern "C" void __fastcall StubBaseDtor(Grid* self, void*)
{
    ++g_deleted; if (g_nOrder < 32) { g_order[g_nOrder] = self; g_kind[g_nOrder++] = 'X'; }
}
extern "C" void __fastcall StubDown(Grid* self, void*) { if (g_nOrder < 32) { g_order[g_nOrder] = self; g_kind[g_nOrder++] = 'D'; } }
extern "C" void __fastcall StubUp(Grid* self, void*) { if (g_nOrder < 32) { g_order[g_nOrder] = self; g_kind[g_nOrder++] = 'U'; } }
extern "C" void __fastcall StubFillInsert(Vec* self, void*, void** pos, unsigned int n, void* const* value)
{
    CHECK(pos == self->finish);
    unsigned int old = (unsigned int)(self->finish - self->start);
    void** p = (void**)malloc((old + n) * sizeof(void*));   // released by the row's STLport allocator (free)
    for (unsigned int i = 0; i < old; ++i) p[i] = self->start[i];
    for (unsigned int i = 0; i < n; ++i) p[old + i] = *value;
    if (self->start) free(self->start);
    self->start = p; self->finish = p + old + n; self->eos = p + old + n;
}
extern "C" void __fastcall CallRow(Map* self, void* edx, long levels);

// The level-0 grid the map's Initialise would have made (fixture-built, deleted through the row's own DeleteData).
static Map g_map;
static void Init()
{
    memset(&g_map, 0, sizeof g_map);
    Grid* g = (Grid*)StubNew(sizeof(Grid)); memset(g, 0, sizeof(Grid));
    g->vptr = GridVtable; g->level = 0; g->size = 1;
    g->cells.start = (void**)malloc(sizeof(void*)); g->cells.start[0] = 0; g->cells.finish = g->cells.eos = g->cells.start + 1;
    Block* b = (Block*)StubNew(sizeof(Block)); b->refs = 1; b->del = GridDeleteData; b->data = g;
    g_map.start = (CP*)malloc(sizeof(CP)); g_map.start[0].data = g; g_map.start[0].info = b;
    g_map.finish = g_map.eos = g_map.start + 1;
}

static void Verify(unsigned long n)
{
    CHECK((unsigned long)(g_map.finish - g_map.start) == n);
    for (unsigned long i = 0; i < n; ++i) {
        CP& e = g_map.start[i]; Grid* g = e.data;
        CHECK(g != 0 && e.info != 0);
        if (!g || !e.info) continue;
        CHECK(e.info->refs == 1 && e.info->data == g && e.info->del == GridDeleteData);
        CHECK(g->vptr == GridVtable && g->level == i && g->size == (1UL << i));
        CHECK((unsigned long)(g->cells.finish - g->cells.start) == g->size * g->size);
        for (void** c = g->cells.start; c != g->cells.finish; ++c) CHECK(*c == 0);
        CHECK(g->up == (i ? g_map.start[i - 1].data : 0) || i == 0);
        CHECK(g->down == (i + 1 < n ? g_map.start[i + 1].data : 0));
    }
}

static void Run(long levels, unsigned long expect, const char* kinds)
{
    ++g_run; g_nOrder = 0; g_deleted = 0;
    unsigned long before = (unsigned long)(g_map.finish - g_map.start);
    Grid* old[16]; for (unsigned long i = 0; i < before && i < 16; ++i) old[i] = g_map.start[i].data;
    CallRow(&g_map, 0, levels);
    Verify(expect);
    CHECK((int)strlen(kinds) == g_nOrder);
    for (int k = 0; k < g_nOrder && kinds[k]; ++k) CHECK(g_kind[k] == kinds[k]);
    // grow: MovePrimitivesDown on the level above each new one, in order; shrink: MovePrimitivesUp on the top, then its deletion
    int k = 0;
    if (expect > before) for (unsigned long i = before; i < expect; ++i, ++k) CHECK(k < g_nOrder && g_order[k] == g_map.start[i - 1].data);
    if (expect < before) for (unsigned long i = before; i-- > expect;) {
        CHECK(k < g_nOrder && g_order[k] == old[i]); ++k;
        CHECK(k < g_nOrder && g_order[k] == old[i]); ++k;
    }
    for (CP* p = g_map.finish; p != g_map.eos; ++p) if (expect < before && p < g_map.start + before) CHECK(p->data == 0 && p->info == 0);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    Init(); int live0 = g_live;
    Run(3, 3, "DD");                       // 1 grow 1 -> 3 (two reallocations: capacity 1 -> 2 -> 4)
    CHECK(g_baseCtor == 2);
    Run(4, 4, "D");                        // 2 grow 3 -> 4 in spare capacity (no reallocation)
    CHECK((g_map.eos - g_map.start) == 4);
    Run(2, 2, "UXUX");                     // 3 shrink 4 -> 2: Up then delete, top first
    Run(0, 1, "UX");                       // 4 clamp to 1: shrink 2 -> 1
    Run(-5, 1, "");                        // 5 clamp, nothing to do
    Run(1, 1, "");                         // 6 equal, nothing to do
    Run(2, 2, "D");                        // 7 grow again into spare capacity
    Run(1, 1, "UX");                       // 8 back to one level
    CHECK(g_live == live0);                // every grid and count block made by the row was released through operator delete
    if (g_fail) { printf("00b94540_TEST FAIL\n"); return 1; }
    printf("00b94540_TEST PASS\n");
    return 0;
}