#include <cstdio>

struct GPBOut {
    int   f0;   // +0
    float f4;   // +4
    int   f8;   // +8
    GPBOut(int a, float b) : f0(a), f4(b), f8(0) { }
};

struct CTCGatherPointBuilding {
    int   f0;   // +0
    float f4;   // +4
    int   f8;   // +8
    GPBOut Init();
};

GPBOut CTCGatherPointBuilding::Init()
{
    return GPBOut(f0, f4);
}

int main()
{
    CTCGatherPointBuilding s;
    s.f0 = 0x12345678;
    s.f4 = 3.5f;
    s.f8 = 999;

    GPBOut o = s.Init();

    if (o.f0 == 0x12345678 && o.f4 == 3.5f && o.f8 == 0)
        printf("00c46eb0_TEST PASS\n");
    else
        printf("FAIL f0=%d f4=%f f8=%d\n", o.f0, o.f4, o.f8);
    return 0;
}