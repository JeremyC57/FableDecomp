// Behaviour test for CThing::GetTCDriver<CTCDExperienceOrb> @ 0x004ACB50: bitset word 2 mask 0x800 gates the driver hand-back.
#include <cstdio>
class CTCDriverBase { public: int id; };
class CTCDExperienceOrb;
class CThing { public:
    unsigned long  UsingComponents[9];
    CTCDriverBase* PTCDriver;
    template <class T> bool GetTCDriver(T** out)
    {
        if (UsingComponents[2] & 0x800) { *out = (T*)PTCDriver; return true; }
        return false;
    }
};
int main()
{
    CTCDriverBase d; d.id = 7;
    CThing t; for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0; t.PTCDriver = &d;
    CTCDExperienceOrb* out = 0;
    if (t.GetTCDriver(&out) || out != 0) { std::printf("004acb50_TEST FAIL\n"); return 1; }
    t.UsingComponents[2] = 0x800;
    if (!t.GetTCDriver(&out) || (void*)out != (void*)&d) { std::printf("004acb50_TEST FAIL\n"); return 1; }
    t.UsingComponents[2] = ~0x800UL; out = 0;
    if (t.GetTCDriver(&out) || out != 0) { std::printf("004acb50_TEST FAIL\n"); return 1; }
    std::printf("004acb50_TEST PASS\n");
    return 0;
}