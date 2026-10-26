// Behaviour test for CThing::GetTCDriver<CTCDExperienceOrb> @ 0x006AE5C0: bitset word 3 mask 0x200 gates the driver hand-back.
#include <cstdio>
class CTCDriverBase { public: int id; };
class CTCDExperienceOrb;
class CThing { public:
    unsigned long  UsingComponents[9];
    CTCDriverBase* PTCDriver;
    template <class T> bool GetTCDriver(T** out)
    {
        if (UsingComponents[3] & 0x200) { *out = (T*)PTCDriver; return true; }
        return false;
    }
};
int main()
{
    CTCDriverBase d; d.id = 7;
    CThing t; for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0; t.PTCDriver = &d;
    CTCDExperienceOrb* out = 0;
    if (t.GetTCDriver(&out) || out != 0) { std::printf("006ae5c0_TEST FAIL\n"); return 1; }
    t.UsingComponents[3] = 0x200;
    if (!t.GetTCDriver(&out) || (void*)out != (void*)&d) { std::printf("006ae5c0_TEST FAIL\n"); return 1; }
    t.UsingComponents[3] = ~0x200UL; out = 0;
    if (t.GetTCDriver(&out) || out != 0) { std::printf("006ae5c0_TEST FAIL\n"); return 1; }
    std::printf("006ae5c0_TEST PASS\n");
    return 0;
}