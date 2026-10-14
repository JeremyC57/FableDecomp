#include <cstdio>
#include <cstring>
struct CCpuInfo {
    unsigned char _pad[4]; long Family; unsigned long Stepping; long Model;
    const char* GetCentaurModelName() const {
        switch (Family) {
        case 5: if (Model == 4) return "WinChip C6"; break;
        case 6: if (Model == 6) return "C3 Samuel"; if (Model == 7) { if (Stepping >= 8) return "C3 Ezra"; return "C3 Samuel 2"; } if (Model == 8) return "C3 Ezra-T"; break;
        }
        return "<unknown>";
    }
};
static bool is(const CCpuInfo& c, const char* s) { return std::strcmp(c.GetCentaurModelName(), s) == 0; }
int main()
{
    CCpuInfo c; c.Family = 5; c.Model = 4; c.Stepping = 0;
    if (!is(c, "WinChip C6")) { std::printf("00c52c90_TEST FAIL\n"); return 1; }
    c.Model = 5; if (!is(c, "<unknown>")) { std::printf("00c52c90_TEST FAIL\n"); return 1; }
    c.Family = 6; c.Model = 6; if (!is(c, "C3 Samuel")) { std::printf("00c52c90_TEST FAIL\n"); return 1; }
    c.Model = 7; c.Stepping = 7; if (!is(c, "C3 Samuel 2")) { std::printf("00c52c90_TEST FAIL\n"); return 1; }
    c.Stepping = 8; if (!is(c, "C3 Ezra")) { std::printf("00c52c90_TEST FAIL\n"); return 1; }
    c.Model = 8; if (!is(c, "C3 Ezra-T")) { std::printf("00c52c90_TEST FAIL\n"); return 1; }
    c.Model = 9; if (!is(c, "<unknown>")) { std::printf("00c52c90_TEST FAIL\n"); return 1; }
    c.Family = 7; c.Model = 6; if (!is(c, "<unknown>")) { std::printf("00c52c90_TEST FAIL\n"); return 1; }
    std::printf("00c52c90_TEST PASS\n");
    return 0;
}
