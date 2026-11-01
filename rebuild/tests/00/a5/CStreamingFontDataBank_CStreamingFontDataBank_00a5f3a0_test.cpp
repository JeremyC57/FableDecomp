// Test for CStreamingFontDataBank::~CStreamingFontDataBank (0x00A5F3A0): CloseFonts runs first, then the streaming
// entry counted pointer is released, the file name destroyed, the font array freed, the state block destroyed, and the
// CBankFileAsync base runs last.
#include <cstdio>
#include <cstdlib>
static char g_log[16]; static int g_n = 0;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteEntry(void*) { g_log[g_n++] = 'E'; }
struct BankFileAsync { virtual ~BankFileAsync() { g_log[g_n++] = 'B'; } };
struct BankStateBlock { virtual ~BankStateBlock() { g_log[g_n++] = 'K'; } };
struct FontStateBlock : BankStateBlock { long h, w, s; bool i; };
struct CharString { void* p; ~CharString() { g_log[g_n++] = 'N'; } };
struct Fonts { void** start; void** finish; void** eos; ~Fonts() { if (start) { std::free(start); g_log[g_n++] = 'F'; } } };
struct StreamingFontDataBank : BankFileAsync {
    FontStateBlock StateBlock; Fonts FontArray; CharString Filename; void* EntryData; Info* EntryInfo;
    void CloseFonts() { g_log[g_n++] = 'C'; }
    ~StreamingFontDataBank()
    {
        CloseFonts();
        if (EntryInfo) { --EntryInfo->RefCount; if (EntryInfo->RefCount == 0) { EntryInfo->DeleteFunc(EntryInfo->Data); delete EntryInfo; } }
        EntryData = 0; EntryInfo = 0;
    }
};
int main()
{
    int e = 0;
    StreamingFontDataBank* b = new StreamingFontDataBank;
    b->FontArray.start = (void**)std::malloc(8); b->Filename.p = 0;
    b->EntryData = &e; b->EntryInfo = new Info; b->EntryInfo->RefCount = 1; b->EntryInfo->DeleteFunc = DeleteEntry; b->EntryInfo->Data = &e;
    delete b;
    const char* want = "CENFKB";
    for (int i = 0; i < 6; ++i) if (g_log[i] != want[i]) { std::printf("00a5f3a0_TEST FAIL\n"); return 1; }
    if (g_n != 6) { std::printf("00a5f3a0_TEST FAIL\n"); return 1; }
    std::printf("00a5f3a0_TEST PASS\n");
    return 0;
}