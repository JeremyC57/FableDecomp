// behaviour test for 0x006C54C0 COpinionDeed::COpinionDeed
#include <cstdio>
class CThing { public: int id; };
static long g_worldFrame = 1234;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
long CWorld::GetFrame() { return g_worldFrame; }
struct CBaseIntelligentPointer {
    const void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    void SetPItem(const void* item) { m_pItem = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem(item); }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class COpinionDeed { public:
    long DeedType, DeedHandle; C3DVector Position; long FrameHappened;
    CIntelligentPointer<const CThing> PTarget;
    bool WasSearchingForWitnesses, IsStillHappening, IsCrimeReportedToVillage, IsCrime; long ExtraData;
    COpinionDeed(long type, long handle, const C3DVector& position, const CThing* target, bool happening, bool isCrime, long extraData)
        : DeedType(type), DeedHandle(handle), Position(position), FrameHappened(CWorld::GetFrame()),
          PTarget(target), WasSearchingForWitnesses(happening), IsStillHappening(happening),
          IsCrimeReportedToVillage(false), IsCrime(isCrime), ExtraData(extraData) {}
};
int main()
{
    CThing t = { 3 };
    C3DVector p(1.0f, 2.0f, 3.0f);
    COpinionDeed d(5, 77, p, &t, true, false, 99);
    if (d.DeedType != 5 || d.DeedHandle != 77 || d.Position.Y != 2.0f || d.Position.Z != 3.0f || d.FrameHappened != 1234
        || d.PTarget.m_pItem != &t || !d.WasSearchingForWitnesses || !d.IsStillHappening || d.IsCrimeReportedToVillage
        || d.IsCrime || d.ExtraData != 99)
    { std::printf("006c54c0_TEST FAIL\n"); return 1; }
    std::printf("006c54c0_TEST PASS\n");
    return 0;
}