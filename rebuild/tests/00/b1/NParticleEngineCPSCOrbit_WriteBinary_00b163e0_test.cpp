#include <cstdio>
#include <cstring>

// CPSCOrbit::WriteBinary: the centre parameter id, then the three orbit settings as one raw block.
static unsigned long g_ulong = 0;
static const void* g_dataPtr = 0;
static unsigned long g_dataSize = 0;
static int g_order = 0, g_ulongOrder = 0, g_dataOrder = 0;
class CDataOutputStream { public:
    virtual void WriteData(const void* data, unsigned long size) { g_dataPtr = data; g_dataSize = size; g_dataOrder = ++g_order; }
    void WriteULong(unsigned long value) { g_ulong = value; g_ulongOrder = ++g_order; }
};
struct COrbitSettings {
    long Type; bool Enabled; float Radius; float Expand; bool CycleFlag; float CycleTime;
    float SqueezeScale; float SqueezeAngle; float RotateSpeed; float RotateStart; float RotateSpeedRandom; float RotateStartRandom;
};
struct CPSCOrbit {
    unsigned long  CentreParam;
    COrbitSettings Orbits[3];
    void WriteBinary(CDataOutputStream* out);
};
void CPSCOrbit::WriteBinary(CDataOutputStream* out)
{
    out->WriteULong(CentreParam);
    out->WriteData(Orbits, sizeof(Orbits));
}

int main()
{
    CPSCOrbit o;
    o.CentreParam = 0x1234;
    CDataOutputStream out;
    o.WriteBinary(&out);
    if (g_ulong != 0x1234 || g_dataPtr != o.Orbits || g_dataSize != sizeof(o.Orbits)
        || g_dataSize != 3 * sizeof(COrbitSettings) || g_ulongOrder != 1 || g_dataOrder != 2) {
        std::printf("00b163e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b163e0_TEST PASS\n");
    return 0;
}