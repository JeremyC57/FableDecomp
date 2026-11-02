// Behaviour test for 0x0085AD00 CCombatActionBase::CStrikeEvent::~CStrikeEvent (compiler-generated):
// StruckThingList's block freed (when allocated), then PWeapon's base destructor, then HelperPointName's destructor.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static int g_order[4];
static int g_n = 0;
static void Note(int e) { if (g_n < 4) g_order[g_n++] = e; }
static void FreeBlock(void* p) { if (p != 0) { ++g_frees; std::free(p); } }
struct CharString { void* PStringData; CharString() : PStringData(0) { } ~CharString() { Note(3); } };
struct BaseIntelligentPointer { void* PData; BaseIntelligentPointer() : PData(0) { } virtual ~BaseIntelligentPointer() { Note(2); } };
struct ThingPointerVector {
    void** Start; void** Finish; void** EndOfStorage;
    ThingPointerVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~ThingPointerVector() { if (Start != 0) Note(1); FreeBlock(Start); }
};
struct StrikeEvent {
    bool Legacy, PrimaryLegacy, Weapon;
    CharString HelperPointName;
    long StrikeEventIndex, StartFrame, EndFrame;
    BaseIntelligentPointer PWeapon;
    ThingPointerVector StruckThingList;
};
int main()
{
    {
        StrikeEvent e;
        e.StruckThingList.Start = (void**)std::malloc(4 * sizeof(void*));
    }
    if (g_frees != 1 || g_n != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3) { std::printf("0085ad00_TEST FAIL\n"); return 1; }
    g_n = 0; g_frees = 0;
    { StrikeEvent e; }
    if (g_frees != 0 || g_n != 2 || g_order[0] != 2 || g_order[1] != 3) { std::printf("0085ad00_TEST FAIL\n"); return 1; }
    std::printf("0085ad00_TEST PASS\n");
    return 0;
}