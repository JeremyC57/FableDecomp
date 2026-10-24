#include <cstdio>

struct CBaseObject { int id; };
static int g_sets = 0;
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; ++g_sets; }
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct C3DVector { float X, Y, Z; };
struct CThing;
struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;
    long                        Index;
    C3DVector                   TargetingPosition;
    long                        FrameRejected;
    CTargetPoint& operator=(const CTargetPoint& other)
    {
        PThing = other.PThing;
        Index = other.Index;
        TargetingPosition = other.TargetingPosition;
        FrameRejected = other.FrameRejected;
        return *this;
    }
};
struct STag { STag() { } };

static CTargetPoint* CopyBackward(CTargetPoint* first, CTargetPoint* last, CTargetPoint* result, STag, long*)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static CBaseObject g_things[5];
static void Fill(CTargetPoint& p, int k) { p.PThing.PData = &g_things[k - 1]; p.Index = k; p.TargetingPosition.X = k * 1.5f; p.TargetingPosition.Y = -k; p.TargetingPosition.Z = k * 2.0f; p.FrameRejected = k * 7; }
static bool Is(const CTargetPoint& p, int k) { return p.PThing.GetPItem() == &g_things[k - 1] && p.Index == k && p.TargetingPosition.X == k * 1.5f && p.TargetingPosition.Y == -k && p.TargetingPosition.Z == k * 2.0f && p.FrameRejected == k * 7; }

int main()
{
    STag tag;
    CTargetPoint buf[5];
    for (int i = 0; i < 5; ++i) Fill(buf[i], i + 1);
    g_sets = 0;
    CTargetPoint* r = CopyBackward(buf, buf + 3, buf + 4, tag, 0);
    bool ok = r == buf + 1 && g_sets == 3;
    if (!Is(buf[0], 1) || !Is(buf[1], 1) || !Is(buf[2], 2) || !Is(buf[3], 3) || !Is(buf[4], 5)) ok = false;
    if (CopyBackward(buf, buf, buf + 2, tag, 0) != buf + 2) ok = false;
    if (!ok) { std::printf("008076d0_TEST FAIL\n"); return 1; }
    std::printf("008076d0_TEST PASS\n");
    return 0;
}