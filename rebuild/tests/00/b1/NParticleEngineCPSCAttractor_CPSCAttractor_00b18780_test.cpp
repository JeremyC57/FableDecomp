// Behaviour test for 0x00B18780 NParticleEngine::CPSCAttractor::~CPSCAttractor (compiler-generated):
// the AttractorUserPointsArray block is freed (only when allocated), then the base destructor runs.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static int g_order[4];
static int g_n = 0;
static void Note(int e) { if (g_n < 4) g_order[g_n++] = e; }
static void FreeBlock(void* p) { if (p != 0) { ++g_frees; std::free(p); } }
struct Vec3 { float X, Y, Z; };
struct Vec3Array {
    Vec3* Start; Vec3* Finish; Vec3* EndOfStorage;
    Vec3Array() : Start(0), Finish(0), EndOfStorage(0) { }
    ~Vec3Array() { if (Start != 0) Note(1); FreeBlock(Start); }
};
struct BaseClassNonCopyable { virtual ~BaseClassNonCopyable() { Note(2); } };
struct PSCBase : BaseClassNonCopyable { void* ParticleSystem; unsigned char Flags; };
struct PSCAttractor : PSCBase {
    unsigned char Bits; long FallOff; unsigned long Param, ParamName; float Radius, Force;
    Vec3Array AttractorUserPointsArray;
};
int main()
{
    PSCAttractor* a = new PSCAttractor;
    a->AttractorUserPointsArray.Start = (Vec3*)std::malloc(4 * sizeof(Vec3));
    BaseClassNonCopyable* base = a;
    delete base;
    if (g_frees != 1 || g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("00b18780_TEST FAIL\n"); return 1; }
    g_n = 0; g_frees = 0;
    base = new PSCAttractor;                 // empty array: no free, base still runs
    delete base;
    if (g_frees != 0 || g_n != 1 || g_order[0] != 2) { std::printf("00b18780_TEST FAIL\n"); return 1; }
    std::printf("00b18780_TEST PASS\n");
    return 0;
}