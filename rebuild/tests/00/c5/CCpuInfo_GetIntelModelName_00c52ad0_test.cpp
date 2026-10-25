#include <cstdio>
#include <cstring>
struct CCpuInfo {
    int Family; unsigned int Stepping; int Model; int BrandIndex; unsigned int L2CacheKB;
    const char* GetIntelModelName() const
    {
        switch (Family) {
        case 4: return "80486";
        case 5: return "Pentium";
        case 6:
            if (Model == 1) return "Pentium Pro";
            if (Model == 3) return "Pentium II";
            if (Model == 5) {
                if (L2CacheKB == 0) return "Celeron";
                if (L2CacheKB >= 1024) return "Pentium II Xeon";
                return "Pentium II";
            }
            if (Model == 6) return "Celeron";
            if (Model == 7) {
                if (L2CacheKB >= 1024) return "Pentium III Xeon";
                return "Pentium III";
            }
            if (Model == 8) {
                switch (BrandIndex) {
                case 1: return "Celeron";
                case 2: return "Pentium III";
                case 3: return "Pentium III Xeon";
                }
                break;
            }
            if (Model == 10) return "Pentium III Xeon";
            if (Model == 11) return "Pentium III";
            break;
        case 15:
            if (Stepping == 2) return "Pentium 4 (sample)";
            return "Pentium 4";
        }
        return "<unknown>";
    }
};
static const char* Name(int fam, int model, unsigned int step, int brand, unsigned int l2)
{
    CCpuInfo c; c.Family = fam; c.Model = model; c.Stepping = step; c.BrandIndex = brand; c.L2CacheKB = l2;
    return c.GetIntelModelName();
}
#define CHECK(got, want) do { if (std::strcmp((got), (want)) != 0) { std::printf("00c52ad0_TEST FAIL %s v %s\n", (got), (want)); return 1; } } while (0)
int main()
{
    CHECK(Name(4, 0, 0, 0, 0), "80486");
    CHECK(Name(5, 2, 0, 0, 0), "Pentium");
    CHECK(Name(6, 1, 0, 0, 0), "Pentium Pro");
    CHECK(Name(6, 3, 0, 0, 512), "Pentium II");
    CHECK(Name(6, 5, 0, 0, 0), "Celeron");
    CHECK(Name(6, 5, 0, 0, 2048), "Pentium II Xeon");
    CHECK(Name(6, 5, 0, 0, 512), "Pentium II");
    CHECK(Name(6, 6, 0, 0, 128), "Celeron");
    CHECK(Name(6, 7, 0, 0, 1024), "Pentium III Xeon");
    CHECK(Name(6, 7, 0, 0, 512), "Pentium III");
    CHECK(Name(6, 8, 0, 1, 0), "Celeron");
    CHECK(Name(6, 8, 0, 2, 0), "Pentium III");
    CHECK(Name(6, 8, 0, 3, 0), "Pentium III Xeon");
    CHECK(Name(6, 8, 0, 7, 0), "<unknown>");
    CHECK(Name(6, 10, 0, 0, 0), "Pentium III Xeon");
    CHECK(Name(6, 11, 0, 0, 0), "Pentium III");
    CHECK(Name(6, 9, 0, 0, 0), "<unknown>");
    CHECK(Name(15, 0, 2, 0, 0), "Pentium 4 (sample)");
    CHECK(Name(15, 0, 3, 0, 0), "Pentium 4");
    CHECK(Name(3, 0, 0, 0, 0), "<unknown>");
    std::printf("00c52ad0_TEST PASS\n");
    return 0;
}