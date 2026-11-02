// Behaviour test for 0x00D34F80 CFireHeartPatternDef::~CFireHeartPatternDef (compiler-generated):
// PadActive's block freed, then the def / pointee base destructors and ~CResource.
#include <cstdio>
#include <cstdlib>
static int g_log[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_log[g_n++] = e; }
static void FreeBlock(void* p) { if (p) { Note(1); std::free(p); } }
struct BoolVector {
    bool* Start; bool* Finish; bool* EndOfStorage;
    BoolVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~BoolVector() { FreeBlock(Start); }
};
struct Resource { virtual ~Resource() { Note(3); } };
struct DefPointeeBase : Resource { virtual ~DefPointeeBase() { Note(2); } };
struct DefClassBase : DefPointeeBase { void* PDefManager; unsigned int GlobalIndex; };
struct FireHeartPatternDef : DefClassBase { BoolVector PadActive; };
int main()
{
    FireHeartPatternDef* def = new FireHeartPatternDef;
    def->PadActive.Start = (bool*)std::malloc(8);
    Resource* base = def;
    delete base;
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3) { std::printf("00d34f80_TEST FAIL\n"); return 1; }
    g_n = 0;
    { FireHeartPatternDef empty; }
    if (g_n != 2 || g_log[0] != 2) { std::printf("00d34f80_TEST FAIL\n"); return 1; }
    std::printf("00d34f80_TEST PASS\n");
    return 0;
}