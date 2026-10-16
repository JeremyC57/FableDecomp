#include <cstdio>
// Behaviour model of NGameText::CTextEntry::~CTextEntry (0x009CB350): members released in reverse order, then the base.
static int g_log[16]; static int g_n = 0;
struct Str { int id; ~Str() { g_log[g_n++] = id; } };
struct Block { int refs; bool destroyed; };
struct Counted { void* Data; Block* Info; ~Counted() { if (Info) { --Info->refs; if (Info->refs == 0) Info->destroyed = true; } Data = 0; Info = 0; g_log[g_n++] = 100; } };
struct Resource { virtual ~Resource() { g_log[g_n++] = 999; } };
struct Entry : Resource { Str SpeechBank, Narrator, SoundName, Text; Counted PFileData; ~Entry() {} };
int main()
{
    Block b = { 1, false };
    {
        Entry e; e.SpeechBank.id = 1; e.Narrator.id = 2; e.SoundName.id = 3; e.Text.id = 4; e.PFileData.Data = 0; e.PFileData.Info = &b;
    }
    if (!b.destroyed || g_n != 6 || g_log[0] != 100 || g_log[1] != 4 || g_log[4] != 1 || g_log[5] != 999) { std::printf("009cb350_TEST FAIL\n"); return 1; }
    std::printf("009cb350_TEST PASS\n");
    return 0;
}
