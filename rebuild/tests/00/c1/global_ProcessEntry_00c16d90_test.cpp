// Behaviour test for 0x00C16D90 (see src/00c16d90.cpp): fill the entry from the source with the settings value; run the
// processor only while the log is still ok.
#include <cstdio>
struct Log { bool ok; };
struct Settings { unsigned long a, b, value; };
static unsigned long g_filledWith; static int g_ran;
static void Fill(int source, unsigned long value, Log* log) { (void)source; g_filledWith = value; (void)log; }
static void Run() { g_ran++; }
static void ProcessEntry(const Settings* s, int source, Log* log)
{
    Fill(source, s->value, log);
    if (log->ok)
        Run();
}
int main()
{
    Settings s = { 1, 2, 77 };
    Log good = { true }; ProcessEntry(&s, 0, &good);
    if (g_filledWith != 77 || g_ran != 1) { std::printf("00c16d90_TEST FAIL\n"); return 1; }
    Log bad = { false }; ProcessEntry(&s, 0, &bad);
    if (g_ran != 1) { std::printf("00c16d90_TEST FAIL\n"); return 1; }
    std::printf("00c16d90_TEST PASS\n"); return 0;
}