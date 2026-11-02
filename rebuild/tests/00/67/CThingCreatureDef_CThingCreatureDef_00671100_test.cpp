// Behaviour test for 0x00671100 CThingCreatureDef::~CThingCreatureDef (compiler-generated):
// NavigatorTypes freed, then the Components small vector delete[]d and reset, then SubDefInfoMap freed, then ~CResource.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static int g_order[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_order[g_n++] = e; }
static void FreeBlock(void* p) { if (p != 0) { ++g_frees; std::free(p); } }
struct Resource { virtual ~Resource() { Note(4); } void* Counts; void* List; void* Prev; void* Next; unsigned int Size, Frame; };
struct DefPointeeBase : Resource { };
struct DefClassBase : DefPointeeBase { void* PDefManager; unsigned int GlobalIndex; unsigned char Flags; };
struct PairArray {
    unsigned int* Start; unsigned int* Finish; unsigned int* EndOfStorage;
    PairArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~PairArray() { if (Start != 0) Note(3); FreeBlock(Start); }
};
struct VectorMap { PairArray Data; unsigned char Compare; bool Dirty; };
struct ParentDefClassBase : DefClassBase { long InstantiationName; VectorMap SubDefInfoMap; };
struct Entry { long Name; long DriverType; bool Override; };
struct SmallVector {
    Entry* Start; signed char Size;
    SmallVector() : Start(0), Size(0) { }
    ~SmallVector() { Clear(); }
    void Clear() { Note(2); delete[] Start; Start = 0; Size = 0; }
};
struct ThingBaseDef : ParentDefClassBase { SmallVector Components; long GroupDef; float Health; };
struct NavigatorVector {
    int* Start; int* Finish; int* EndOfStorage;
    NavigatorVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~NavigatorVector() { if (Start != 0) Note(1); FreeBlock(Start); }
};
struct ThingCreatureDef : ThingBaseDef { float StunTimeMin; int Sex; NavigatorVector NavigatorTypes; long InitialWeaponDef; };
int main()
{
    ThingCreatureDef* def = new ThingCreatureDef;
    def->NavigatorTypes.Start = (int*)std::malloc(3 * sizeof(int));
    def->Components.Start = new Entry[2];
    def->Components.Size = 2;
    def->SubDefInfoMap.Data.Start = (unsigned int*)std::malloc(4 * sizeof(unsigned int));
    Resource* base = def;
    delete base;
    if (g_frees != 2 || g_n != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4) { std::printf("00671100_TEST FAIL\n"); return 1; }
    std::printf("00671100_TEST PASS\n");
    return 0;
}