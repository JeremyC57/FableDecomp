// Behaviour test for 0x00C52BB0 CCpuInfo::GetAMDModelName: a standalone model of the parked source (the residue is the
// K5 compare's register / constant form). Family 4 is "486 or 5x86"; family 5 models 0-3 K5, 6-7 K6, 8 K6-2, 9 K6-III;
// family 6 model 1 K7, 2/4 Athlon, 3/7 Duron, 6/8 Athlon XP; anything else "<unknown>". Prints exactly one line.
#include <cstdio>
#include <cstring>

struct CpuInfo { unsigned long Family; unsigned long Model; };

static const char* GetAMDModelName(const CpuInfo& cpu)
{
    switch (cpu.Family) {
    case 4:
        return "486 or 5x86";
    case 5:
        if (cpu.Model < 4) return "K5";
        if (cpu.Model == 6 || cpu.Model == 7) return "K6";
        if (cpu.Model == 8) return "K6-2";
        if (cpu.Model == 9) return "K6-III";
        break;
    case 6:
        if (cpu.Model == 1) return "K7";
        if (cpu.Model == 2 || cpu.Model == 4) return "Athlon";
        if (cpu.Model == 3 || cpu.Model == 7) return "Duron";
        if (cpu.Model == 6 || cpu.Model == 8) return "Athlon XP";
        break;
    }
    return "<unknown>";
}

static int Check(unsigned long family, unsigned long model, const char* expected)
{
    CpuInfo cpu = { family, model };
    return std::strcmp(GetAMDModelName(cpu), expected) == 0 ? 0 : 1;
}

int main()
{
    int fails = 0;
    fails += Check(4, 0, "486 or 5x86");
    fails += Check(4, 9, "486 or 5x86");
    fails += Check(5, 0, "K5");
    fails += Check(5, 3, "K5");
    fails += Check(5, 4, "<unknown>");
    fails += Check(5, 5, "<unknown>");
    fails += Check(5, 6, "K6");
    fails += Check(5, 7, "K6");
    fails += Check(5, 8, "K6-2");
    fails += Check(5, 9, "K6-III");
    fails += Check(5, 10, "<unknown>");
    fails += Check(6, 1, "K7");
    fails += Check(6, 2, "Athlon");
    fails += Check(6, 4, "Athlon");
    fails += Check(6, 3, "Duron");
    fails += Check(6, 7, "Duron");
    fails += Check(6, 6, "Athlon XP");
    fails += Check(6, 8, "Athlon XP");
    fails += Check(6, 0, "<unknown>");
    fails += Check(6, 9, "<unknown>");
    fails += Check(7, 1, "<unknown>");
    fails += Check(15, 4, "<unknown>");
    std::printf(fails == 0 ? "00c52bb0_TEST PASS\n" : "00c52bb0_TEST FAIL\n");
    return fails == 0 ? 0 : 1;
}
