// behaviour test for CAIStateGroupBase::AIAction_SpeakAISpeechToTarget (0x00981350)
#include <cstdio>
#include <cstring>
struct Data { const char* Chars; unsigned long Length; };
struct Str { Data* p; unsigned long Len() const { return p == 0 ? 0 : p->Length; } const char* Get() const { const char* s = ""; if (p) s = p->Chars; return s; } };
static unsigned long Calc(unsigned long seed, const char* d, unsigned long n) { unsigned long h = seed; for (unsigned long i = 0; i < n; ++i) h = h * 31 + (unsigned char)d[i]; return h; }
static long Lookup(unsigned long crc) { return (long)(crc % 1000); }
static long lastSpeech, lastMode;
static long SpeakToTarget(long mode, long speech) { lastMode = mode; lastSpeech = speech; return 7; }
static long SpeakAI(long mode, const Str& name) { long speech = Lookup(Calc(0, name.Get(), name.Len())); return SpeakToTarget(mode, speech); }
int main()
{
    Data d = { "HELLO", 5 }; Str s = { &d }; Str e = { 0 };
    bool ok = SpeakAI(3, s) == 7 && lastMode == 3 && lastSpeech == Lookup(Calc(0, "HELLO", 5));
    ok = ok && SpeakAI(1, e) == 7 && lastSpeech == 0 && std::strcmp(e.Get(), "") == 0;
    if (!ok) { std::printf("00981350_TEST FAIL\n"); return 1; }
    std::printf("00981350_TEST PASS\n");
    return 0;
}