// Behaviour test for 0x008CC330 NCreatureMode::CModeSummonerSummon::~CModeSummonerSummon (compiler-generated):
// members die in reverse order (CreatePositions block freed, PTarget released, Filter's Group string), then the base.
#include <cstdio>
#include <cstdlib>
static int g_log[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_log[g_n++] = e; }
static void FreeBlock(void* p) { if (p) { Note(1); std::free(p); } }
class C3DVector { public: float X, Y, Z; };
struct PositionVector {                      // STLport vector<C3DVector>: start, finish, end_of_storage
    C3DVector* Start; C3DVector* Finish; C3DVector* EndOfStorage;
    PositionVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~PositionVector() { FreeBlock(Start); }
};
struct IntelligentPointer { void* PData; IntelligentPointer() : PData(0) { } ~IntelligentPointer() { Note(2); } };
struct CharString { void* PStringData; CharString() : PStringData(0) { } ~CharString() { Note(3); } };
struct AnimFilterBase { virtual ~AnimFilterBase() { Note(4); } };
struct AnimFilterOfGroup : AnimFilterBase { CharString Group; unsigned int GroupCRC; };
struct ModeStandBase { virtual ~ModeStandBase() { Note(5); } };
struct ModeSummonerSummon : ModeStandBase {
    AnimFilterOfGroup  Filter;
    IntelligentPointer PTarget;
    PositionVector     CreatePositions;
    long               Timer;
};
int main()
{
    ModeSummonerSummon* mode = new ModeSummonerSummon;
    mode->CreatePositions.Start = (C3DVector*)std::malloc(3 * sizeof(C3DVector));
    mode->CreatePositions.Finish = mode->CreatePositions.Start + 2;
    mode->CreatePositions.EndOfStorage = mode->CreatePositions.Start + 3;
    delete mode;
    const int expected[5] = { 1, 2, 3, 4, 5 };
    if (g_n != 5) { std::printf("008cc330_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (g_log[i] != expected[i]) { std::printf("008cc330_TEST FAIL\n"); return 1; }
    g_n = 0;
    { ModeSummonerSummon empty; }            // empty vector: no free
    if (g_n != 4 || g_log[0] != 2) { std::printf("008cc330_TEST FAIL\n"); return 1; }
    std::printf("008cc330_TEST PASS\n");
    return 0;
}