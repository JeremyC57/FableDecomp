// Behaviour test for 0095a490: final.cpp copied verbatim, the callees stubbed and the inverse-square-root table filled
// below so the directions, the dot test and the XY distance are observable. CAIStateGroup_HoverHornetQueen::IsRouteToPosBlockedByTarget
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CAIStateGroup_HoverHornetQueen::IsRouteToPosBlockedByTarget(long circlePoint) -- true when the target and the circle marker are alive, the direction to the marker is within 60 degrees of the direction to the target (dot > 0.5) and the target is within 3 units in XY
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A490, length 558 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0.
// Globals: inverse-square-root mantissa table @ 0x013CE558; 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C.
// Shape: both directions use the inline table-driven Normalise written as `*this *= FastInvSqrt(lenSq)` with
// `operator*=(const float&)`: the const-reference binding to the inline temporary spills the reciprocal
// (`fstp [r] ; fxch ; fmul [r]` x3) as retail does (chunk 278's lesson; a plain `X *= r` keeps it on the x87 stack);
// the XY distance shares the X*X/Y*Y products with the normalise length (VC CSE). The thing pointers are named
// before their GetPosition calls.
// Layout: Ego_r.pdb CAIStateGroup_HoverMovementBase target pointer +0x14; HoverHornetQueen CircleMarkers +0x50 in
// retail (PDB +0x54: the STLport vector Things at +0x40 is 12 bytes, not 16).
#include <math.h>
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
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
    float GetXYMagnitude() const { return (float)sqrt(X * X + Y * Y); }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(X * X + Y * Y + Z * Z); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IsAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1); }
struct CIntelligentPointerVector {              // STLport vector<CIntelligentPointer<CThing> > (12 bytes)
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
    CBaseIntelligentPointer* begin() const { return _M_start; }
    CBaseIntelligentPointer& operator[](long n) const { return *(begin() + n); }
};

class CAIBrain { public:
    virtual void v00();
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_HoverMovementBase {
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool bOneTime;                              // +0x34
    CThing* PEmitter;                           // +0x38
    CThing* PEmitter2;                          // +0x3c
    CIntelligentPointerVector Things;           // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CIntelligentPointerVector CircleMarkers;    // +0x50
    long EnemyKnockDownFrame;                   // +0x5c
    bool TimingCritical;                        // +0x60
    long RouteDirection;                        // +0x64
    bool IsRouteToPosBlockedByTarget(long circlePoint);
};

bool CAIStateGroup_HoverHornetQueen::IsRouteToPosBlockedByTarget(long circlePoint)
{
    if (IsAlive(PTarget) && IsAlive(CircleMarkers[circlePoint])) {
        const CThing* creature = GetCreature();
        const CThing* target = PTarget.GetPItem();
        C3DVector toTarget = target->GetPosition() - creature->GetPosition();
        float distanceXY = toTarget.GetXYMagnitude();
        toTarget.Normalise();
        const CThing* me = GetCreature();
        const CThing* marker = CircleMarkers[circlePoint].GetPItem();
        C3DVector toMarker = marker->GetPosition() - me->GetPosition();
        toMarker.Normalise();
        if (Dot(toMarker, toTarget) > 0.5f && distanceXY < 3.0f)
            return true;
    }
    return false;
}
// ---- stubs ----
unsigned long g_InvSqrtMantissaTable[128];
static void FillTable()                           // the 8-bit mantissa of 1/sqrt(x) for each (exponent LSB, top 6 mantissa bits) class
{
    for (int i = 0; i < 128; ++i) {
        unsigned long bits = ((unsigned long)(126 + (i >> 6)) << 23) | ((unsigned long)(i & 63) << 17) | (1UL << 16);   // exponent LSB = index bit 6
        float x; memcpy(&x, &bits, 4);
        float y = 1.0f / (float)sqrt((double)x);
        unsigned long ybits; memcpy(&ybits, &y, 4);
        unsigned long expPart = ((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23;
        g_InvSqrtMantissaTable[i] = ((ybits - expPart) >> 15) & 0xff;
    }
}
static CThing g_creature, g_target, g_marker0, g_marker1;
static C3DVector g_pos[4];
static int Index(const CThing* t) { return t == &g_creature ? 0 : t == &g_target ? 1 : t == &g_marker0 ? 2 : 3; }
const C3DVector& CThing::GetPosition() const { char b[32]; _snprintf(b, sizeof b, "pos(%d)", Index(this)); note(b); return g_pos[Index(this)]; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() { }
void CAIBrain::v00() { }
CAIStateGroupBase::~CAIStateGroupBase() { }
static void Set(C3DVector& v, float x, float y, float z) { v.X = x; v.Y = y; v.Z = z; }
int main()
{
    FillTable();
    { C3DVector t; Set(t, 3.0f, 4.0f, 12.0f); t.Normalise(); CHECK(fabs(t.X - 3.0f / 13.0f) < 2e-3f && fabs(t.Z - 12.0f / 13.0f) < 2e-3f); }
    CAIStateGroup_HoverHornetQueen* q = (CAIStateGroup_HoverHornetQueen*)calloc(1, sizeof(CAIStateGroup_HoverHornetQueen));
    CAIBrain* brain = (CAIBrain*)calloc(1, sizeof(CAIBrain)); brain->PCreature = &g_creature; q->PBrain = brain;
    CBaseIntelligentPointer markers[2]; markers[0].m_pItem = &g_marker0; markers[1].m_pItem = &g_marker1;
    q->CircleMarkers._M_start = markers; q->CircleMarkers._M_finish = markers + 2; q->CircleMarkers._M_end_of_storage = markers + 2;
    memset(&g_creature, 0, sizeof g_creature); memset(&g_target, 0, sizeof g_target); memset(&g_marker0, 0, sizeof g_marker0); memset(&g_marker1, 0, sizeof g_marker1);
    Set(g_pos[0], 0, 0, 0); Set(g_pos[1], 1, 0, 0); Set(g_pos[2], 1, 0.5f, 0); Set(g_pos[3], 0, 1, 0);
    // 1. no target
    q->PTarget.m_pItem = 0; g_log[0] = 0; CHECK(!q->IsRouteToPosBlockedByTarget(0)); CHECKLOG("");
    // 2. dead target
    q->PTarget.m_pItem = &g_target; g_target.m_flags91 = 1; g_log[0] = 0; CHECK(!q->IsRouteToPosBlockedByTarget(0)); CHECKLOG("");
    // 3. dead marker
    g_target.m_flags91 = 0; g_marker0.m_flags91 = 1; g_log[0] = 0; CHECK(!q->IsRouteToPosBlockedByTarget(0)); CHECKLOG("");
    // 4. marker 0 within 60 degrees of the target direction, target 1 unit away: blocked
    g_marker0.m_flags91 = 0; g_log[0] = 0; CHECK(q->IsRouteToPosBlockedByTarget(0)); CHECKLOG("pos(0);pos(1);pos(0);pos(2);");   // operator- evaluates its right operand first
    // 5. marker 1 at 90 degrees: not blocked
    g_log[0] = 0; CHECK(!q->IsRouteToPosBlockedByTarget(1)); CHECKLOG("pos(0);pos(1);pos(0);pos(3);");
    // 6. target too far (5 units in XY) although aligned
    Set(g_pos[1], 5, 0, 0); g_log[0] = 0; CHECK(!q->IsRouteToPosBlockedByTarget(0));
    // 7. XY distance ignores Z: target 1 unit away in XY but 10 up; marker 0 aligned in XY only -> dot of 3D directions
    Set(g_pos[1], 1, 0, 0); Set(g_pos[2], 2, 0, 0); g_log[0] = 0; CHECK(q->IsRouteToPosBlockedByTarget(0));
    free(brain); free(q);
    if (fails == 0) printf("0095a490_TEST PASS\n"); else printf("0095a490_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}