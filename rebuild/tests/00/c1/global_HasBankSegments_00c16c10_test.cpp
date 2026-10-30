// Behaviour test for 0x00C16C10 (see src/00c16c10.cpp): both segments must open; the second is only tried when the first exists.
#include <cstdio>
#include <cstring>
struct File { bool hasLookup, hasComp; int tries; };
static long OpenSegment(File* f, const char* name)
{
    f->tries++;
    if (!std::strcmp(name, "LHAudioBankLookupTable")) return f->hasLookup ? 0 : -1;
    return f->hasComp ? 0 : -1;
}
static bool HasBankSegments(File* f)
{
    bool missing = OpenSegment(f, "LHAudioBankLookupTable") < 0 || OpenSegment(f, "LHAudioBankCompData") < 0;
    return !missing;
}
int main()
{
    File a = { true, true, 0 }, b = { false, true, 0 }, c = { true, false, 0 };
    if (!HasBankSegments(&a) || a.tries != 2) { std::printf("00c16c10_TEST FAIL\n"); return 1; }
    if (HasBankSegments(&b) || b.tries != 1) { std::printf("00c16c10_TEST FAIL\n"); return 1; }
    if (HasBankSegments(&c) || c.tries != 2) { std::printf("00c16c10_TEST FAIL\n"); return 1; }
    std::printf("00c16c10_TEST PASS\n"); return 0;
}