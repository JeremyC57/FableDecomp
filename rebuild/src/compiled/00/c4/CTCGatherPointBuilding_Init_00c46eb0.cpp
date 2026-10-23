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
