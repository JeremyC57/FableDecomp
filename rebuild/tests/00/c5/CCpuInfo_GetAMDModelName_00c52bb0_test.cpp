// Behaviour test for CCpuInfo::GetAMDModelName (0x00C52BB0): the row body verbatim (with its local CCpuInfo model). Kills: any
// family / model mapping changed (family 4 "486 or 5x86"; family 5 models 0-3 K5, 6-7 K6, 8 K6-2, 9 K6-III; family 6 model 1 K7,
// 2/4 Athlon, 3/7 Duron, 6/8 Athlon XP), the K5 bound off by one, and anything else not "<unknown>" (including the fitted tail test).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
// ---- row (verbatim) ----
struct CCpuInfo {
    unsigned char _pad[4];
    long Family;                                         // +4
    unsigned long Stepping;                              // +8
    unsigned long Model;                                 // +0xc
    const char* GetAMDModelName() const;
};

const char* CCpuInfo::GetAMDModelName() const
{
    switch (Family) {
    case 4:
        return "486 or 5x86";
    case 5:
        if (Model <= 3)
            return "K5";
        if (Model == 6 || Model == 7)
            return "K6";
        if (Model == 8)
            return "K6-2";
        if (Model == 9)
            return "K6-III";
        if (Model > 9)
            return "<unknown>";
        break;
    case 6:
        if (Model == 1)
            return "K7";
        if (Model == 2 || Model == 4)
            return "Athlon";
        if (Model == 3 || Model == 7)
            return "Duron";
        if (Model == 6 || Model == 8)
            return "Athlon XP";
        break;
    }
    return "<unknown>";
}
// ---- end row ----

static const char* name(long family, unsigned long model)
{
    CCpuInfo cpu; memset(&cpu, 0, sizeof(cpu)); cpu.Family = family; cpu.Model = model; cpu.Stepping = 7;
    return cpu.GetAMDModelName();
}
static const char* expected(long f, unsigned long m)
{
    if (f == 4) return "486 or 5x86";
    if (f == 5) {
        if (m <= 3) return "K5";
        if (m == 6 || m == 7) return "K6";
        if (m == 8) return "K6-2";
        if (m == 9) return "K6-III";
    }
    if (f == 6) {
        if (m == 1) return "K7";
        if (m == 2 || m == 4) return "Athlon";
        if (m == 3 || m == 7) return "Duron";
        if (m == 6 || m == 8) return "Athlon XP";
    }
    return "<unknown>";
}

int main()
{
    long cases = 0;
    const long families[] = { 0, 3, 4, 5, 6, 7, 15, -1 };
    for (int f = 0; f != 8; ++f)
        for (unsigned long m = 0; m != 20; ++m) {
            CHECK(strcmp(name(families[f], m), expected(families[f], m)) == 0);
            ++cases;
        }
    CHECK(strcmp(name(5, 0xFFFFFFFFul), "<unknown>") == 0); ++cases;   // unsigned model: a huge value is not K5
    CHECK(strcmp(name(5, 3), "K5") == 0 && strcmp(name(5, 4), "<unknown>") == 0); ++cases;
    CHECK(cases == 162);
    puts("00c52bb0_TEST PASS");
    return 0;
}