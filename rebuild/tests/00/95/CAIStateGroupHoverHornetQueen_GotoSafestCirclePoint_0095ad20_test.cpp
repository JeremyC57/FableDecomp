// Behaviour test for CAIStateGroup_HoverHornetQueen::GotoSafestCirclePoint (0x0095AD20): model and body verbatim, the
// intelligent-pointer / position / GotoCirclePoint callees stubbed; empty markers, both choices and both index wrap-arounds.
#include <stdio.h>
#include <math.h>

unsigned long g_InvSqrtMantissaTable[128];   // stub table (zero mantissa correction)
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(Z * Z + Y * Y + X * X); }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
class CThing;
__forceinline C3DVector Direction(const CThing* to, const CThing* from);
__forceinline float Dot(const C3DVector& a, const C3DVector& b) { return a.Z * b.Z + a.Y * b.Y + a.X * b.X; }
class CThing { public:
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CThingCreatureBase : public CThing { public: };
__forceinline C3DVector Direction(const CThing* to, const CThing* from) { return to->GetPosition() - from->GetPosition(); }
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template <class T> struct CVec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](long i) { return *(begin() + i); }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase { };
struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_HoverMovementBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    unsigned char _pad1c[0x40 - 0x1c];
    CVec<CBaseIntelligentPointer> Things;       // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CVec<CBaseIntelligentPointer> CircleMarkers;   // +0x50
    long GetNextCirclePoint() { long i = CurrentCirclePoint + 1; if (i == CircleMarkers.size()) i = 0; return i; }
    long GetPrevCirclePoint() { long i = CurrentCirclePoint - 1; if (i == -1) i = CircleMarkers.size() - 1; return i; }
    long GotoCirclePoint(long index, long movementType);   // 0x0095A6C0
    long GotoSafestCirclePoint(long movementType);
};

long CAIStateGroup_HoverHornetQueen::GotoSafestCirclePoint(long movementType)
{
    if (CircleMarkers.size() == 0)
        return 3;
    C3DVector toTarget = Direction(PTarget.GetPItem(), GetCreature());
    toTarget.Normalise();
    C3DVector toNext = Direction(CircleMarkers[GetNextCirclePoint()].GetPItem(), GetCreature());
    toNext.Normalise();
    C3DVector toPrev = Direction(CircleMarkers[GetPrevCirclePoint()].GetPItem(), GetCreature());
    toPrev.Normalise();
    if (Dot(toNext, toTarget) > Dot(toPrev, toTarget))
        return GotoCirclePoint(GetPrevCirclePoint(), movementType);
    return GotoCirclePoint(GetNextCirclePoint(), movementType);
}

struct CTestThing : CThingCreatureBase { C3DVector Pos; };
const C3DVector& CThing::GetPosition() const { return ((const CTestThing*)this)->Pos; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() {}
CAIStateGroupBase::~CAIStateGroupBase() {}

static long g_gotoIndex = -1, g_gotoType = -1, g_gotoCalls = 0;
long CAIStateGroup_HoverHornetQueen::GotoCirclePoint(long index, long movementType)
{
    ++g_gotoCalls; g_gotoIndex = index; g_gotoType = movementType;
    return 2;
}

static int fail(const char* why) { printf("FAIL: %s\n", why); return 1; }

int main()
{
    for (int i = 0; i < 128; ++i)
        g_InvSqrtMantissaTable[i] = 0;
    CTestThing creature; creature.Pos = C3DVector(0.0f, 0.0f, 0.0f);
    CTestThing target;   target.Pos = C3DVector(10.0f, 0.0f, 0.0f);
    CTestThing markers[3];
    markers[0].Pos = C3DVector(10.0f, 1.0f, 0.0f);     // towards the target
    markers[1].Pos = C3DVector(-10.0f, 1.0f, 0.0f);    // away from the target
    markers[2].Pos = C3DVector(0.0f, 10.0f, 0.0f);     // sideways
    CBaseIntelligentPointer markerPtrs[3];
    for (int i = 0; i < 3; ++i) markerPtrs[i].m_pItem = &markers[i];
    CAIBrain brain; brain.PCreature = &creature;

    CAIStateGroup_HoverHornetQueen group;
    group.PBrain = &brain;
    group.PTarget.m_pItem = &target;
    group.CircleMarkers._M_start = markerPtrs;
    group.CircleMarkers._M_finish = markerPtrs;
    group.CircleMarkers._M_end_of_storage = markerPtrs + 3;

    // No markers: 3 without moving.
    if (group.GotoSafestCirclePoint(4) != 3 || g_gotoCalls != 0) return fail("empty");

    group.CircleMarkers._M_finish = markerPtrs + 3;
    // Current 2: next = 0 (towards the target), prev = 1 (away): next faces the target more -> go to prev.
    group.CurrentCirclePoint = 2;
    if (group.GotoSafestCirclePoint(4) != 2 || g_gotoCalls != 1 || g_gotoIndex != 1 || g_gotoType != 4) return fail("prefer prev");
    // Current 1: next = 2 (sideways), prev = 0 (towards): prev faces the target more -> go to next (wraps: next of 2 is 0 is not used here).
    group.CurrentCirclePoint = 1;
    if (group.GotoSafestCirclePoint(5) != 2 || g_gotoCalls != 2 || g_gotoIndex != 2 || g_gotoType != 5) return fail("prefer next");
    // Current 0: next = 1 (away), prev wraps to 2 (sideways): next faces away -> go to next = 1.
    group.CurrentCirclePoint = 0;
    if (group.GotoSafestCirclePoint(4) != 2 || g_gotoCalls != 3 || g_gotoIndex != 1) return fail("prev wrap");
    // Current 2 with the markers reversed so next (wrapping to 0) faces away.
    markers[0].Pos = C3DVector(-10.0f, 1.0f, 0.0f);
    markers[1].Pos = C3DVector(10.0f, 1.0f, 0.0f);
    group.CurrentCirclePoint = 2;
    if (group.GotoSafestCirclePoint(4) != 2 || g_gotoCalls != 4 || g_gotoIndex != 0) return fail("next wrap");

    printf("0095ad20_TEST PASS\n");
    return 0;
}