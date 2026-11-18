#include <cassert>
#include <cstdio>
#include <cstring>

class CCpuInfo
{
public:
    int Family;
    int Model;

    const char* GetCyrixModelName() const
    {
        switch (Family) {
        case 4:
            return "MediaGX";
        case 5:
            if (Model == 2)
                return "6x86";
            if (Model == 4)
                return "GXm";
            break;
        case 6:
            if (Model == 0)
                return "6x86MX";
            if (Model == 5)
                return "Cyrix III";
            break;
        }
        return "<unknown>";
    }
};

int main()
{
    struct ModelInfo { int family, model; const char* name; };
    const ModelInfo names[] = {
        {4, -1, "MediaGX"}, {5, 2, "6x86"}, {5, 4, "GXm"},
        {6, 0, "6x86MX"}, {6, 5, "Cyrix III"}
    };
    for (int family = -1; family < 17; ++family) {
        for (int model = -1; model < 17; ++model) {
            CCpuInfo info;
            info.Family = family;
            info.Model = model;
            const char* expected = "<unknown>";
            for (unsigned i = 0; i < sizeof names / sizeof *names; ++i)
                if (family == names[i].family && (names[i].model == model || family == 4))
                    expected = names[i].name;
            assert(std::strcmp(info.GetCyrixModelName(), expected) == 0);
        }
    }
    std::puts("00c52c40_TEST PASS");
    return 0;
}
