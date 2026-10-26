// Behaviour test for CReactionMatch_MRPyramid::TransferBinaryOut @ 0x006FECA0: base state first, then the three shifts.
#include <cstdio>

class CDataOutputStream { public:
    float values[16]; int n; int baseCalls;
    CDataOutputStream() : n(0), baseCalls(0) {}
    void WriteFloat(float v) { values[n++] = v; }
};

struct CReactionMatch_AsymmetricPyramid {
    float XCentre;
    void TransferBinaryOut(CDataOutputStream& stream) const { ++stream.baseCalls; stream.WriteFloat(XCentre); }
};

struct CReactionMatch_MRPyramid : CReactionMatch_AsymmetricPyramid {
    float ScarinessShift, AgreeablenessShift, AttractivenessShift;
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        CReactionMatch_AsymmetricPyramid::TransferBinaryOut(stream);
        stream.WriteFloat(ScarinessShift);
        stream.WriteFloat(AgreeablenessShift);
        stream.WriteFloat(AttractivenessShift);
    }
};

int main()
{
    CReactionMatch_MRPyramid m;
    m.XCentre = 9.0f; m.ScarinessShift = 0.5f; m.AgreeablenessShift = -0.25f; m.AttractivenessShift = 2.0f;
    CDataOutputStream s;
    m.TransferBinaryOut(s);
    if (s.baseCalls != 1 || s.n != 4 || s.values[0] != 9.0f || s.values[1] != 0.5f || s.values[2] != -0.25f || s.values[3] != 2.0f) {
        std::printf("006feca0_TEST FAIL\n"); return 1;
    }
    std::printf("006feca0_TEST PASS\n");
    return 0;
}