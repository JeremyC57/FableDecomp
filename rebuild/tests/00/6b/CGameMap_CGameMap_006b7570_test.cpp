// Behaviour test for 0x006B7570 CGameMap::~CGameMap (user destructor, empty body):
// deleting through the base frees the Cells block (when allocated) and then runs the base destructor.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static int g_order[4];
static int g_n = 0;
static void Note(int e) { if (g_n < 4) g_order[g_n++] = e; }
static void FreeBlock(void* p) { if (p != 0) { ++g_frees; std::free(p); } }
struct GameMapCell { unsigned char AtmosThemes[3]; unsigned char AtmosBlends[2]; signed char SoundTheme; unsigned char Music; };
struct CellVector {
    GameMapCell* Start; GameMapCell* Finish; GameMapCell* EndOfStorage;
    CellVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~CellVector() { if (Start != 0) Note(1); FreeBlock(Start); }
};
struct BaseClassNonCopyable { virtual ~BaseClassNonCopyable() { Note(2); } };
struct GameMap : BaseClassNonCopyable {
    virtual ~GameMap() { }
    CellVector Cells; void* Map; void* World; unsigned int* WorldSeed; long SizeX, SizeY;
};
int main()
{
    GameMap* map = new GameMap;
    map->Cells.Start = (GameMapCell*)std::malloc(16 * sizeof(GameMapCell));
    BaseClassNonCopyable* base = map;
    delete base;
    if (g_frees != 1 || g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("006b7570_TEST FAIL\n"); return 1; }
    g_n = 0; g_frees = 0;
    base = new GameMap;
    delete base;
    if (g_frees != 0 || g_n != 1 || g_order[0] != 2) { std::printf("006b7570_TEST FAIL\n"); return 1; }
    std::printf("006b7570_TEST PASS\n");
    return 0;
}