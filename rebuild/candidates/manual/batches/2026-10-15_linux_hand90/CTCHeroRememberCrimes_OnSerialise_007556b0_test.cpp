#include <cstdio>
#include <cstring>
// Behaviour of CTCHeroRememberCrimes::OnSerialise (0x007556B0): the crime map with a {0, 0} default, then the
// paid-fine flag with a 0 default, in that order and under the persisted names.
struct SCrimeDurationAndFine {
    long Duration; long Fine;
    SCrimeDurationAndFine(long duration, long fine) : Duration(duration), Fine(fine) {}
};
struct CCrimeMap { int entries; };
static int g_order = 0; static bool g_ok = true;
struct CPersistContext {
    void TransferMap(const char* name, CCrimeMap& value, const SCrimeDurationAndFine& defaultValue)
    {
        if (++g_order != 1 || std::strcmp(name, "RegionNamesToCrimeDurationAndFine") != 0 || defaultValue.Duration != 0 || defaultValue.Fine != 0) g_ok = false;
        value.entries = 5;
    }
    void Transfer(const char* name, signed char& value, const signed char& defaultValue)
    {
        if (++g_order != 2 || std::strcmp(name, "HasPaidTheFine") != 0 || defaultValue != 0) g_ok = false;
        value = 1;
    }
};
struct CTCHeroRememberCrimes {
    CCrimeMap RegionNamesToCrimeDurationAndFine;
    signed char HasPaidTheFine;
    void OnSerialise(CPersistContext* context, long version)
    {
        context->TransferMap("RegionNamesToCrimeDurationAndFine", RegionNamesToCrimeDurationAndFine,
                             SCrimeDurationAndFine(0, 0));
        context->Transfer("HasPaidTheFine", HasPaidTheFine, 0);
    }
};
int main()
{
    CPersistContext context; CTCHeroRememberCrimes crimes;
    crimes.RegionNamesToCrimeDurationAndFine.entries = 0; crimes.HasPaidTheFine = 0;
    crimes.OnSerialise(&context, 0);
    if (!g_ok || g_order != 2 || crimes.RegionNamesToCrimeDurationAndFine.entries != 5 || crimes.HasPaidTheFine != 1) {
        std::printf("007556b0_TEST FAIL\n"); return 1;
    }
    std::printf("007556b0_TEST PASS\n");
    return 0;
}
