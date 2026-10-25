// Behaviour test for 0x00661D70: CThingCreatureBase::IsThingInFrontCone(const CThing* thing) const -- true when the direction to the other thing lies within 20 degrees of this thing's physics facing (normalised dot product against cos 20 degrees)
// A copy of the source model and logic (compiled separately from src/00661d70.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
// test stand-in for the retail FastInvSqrt over the 128-entry mantissa table at 0x013CE558: the same estimate + one
// Newton step, with the table rebuilt from 1/sqrt at each bucket (unsigned int words for the 64-bit g++ host)
static unsigned int g_InvSqrtMantissaTable[128];
static unsigned int FloatBits(float f) { unsigned int b; std::memcpy(&b, &f, 4); return b; }
static float BitsFloat(unsigned int b) { float f; std::memcpy(&f, &b, 4); return f; }
static void BuildInvSqrtTable()
{
    for (unsigned int i = 0; i < 128; ++i) {
        unsigned int e = (i >> 6) ? 127 : 128;                  // bit 6 of the index is the exponent LSB
        unsigned int bits = (e << 23) | ((i & 0x3f) << 17) | (1u << 16);   // bucket midpoint
        float x = BitsFloat(bits);
        float r = 1.0f / (float)std::sqrt(x);
        unsigned int estimateExp = ((0x17c - e) >> 1) << 23;
        float scale = BitsFloat(estimateExp);
        float m = (r / scale - 1.0f) * 256.0f;
        int t = (int)(m + 0.5f);
        if (t < 0) t = 0;
        if (t > 255) t = 255;
        g_InvSqrtMantissaTable[i] = (unsigned int)t;
    }
}
inline float FastInvSqrt(float lengthSquared)
{
    unsigned int bits = FloatBits(lengthSquared);
    unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                            (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = BitsFloat(estimate);
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}

#pragma intrinsic(cos)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline void Normalise()
    {
        float inv = FastInvSqrt(X * X + Y * Y + Z * Z);
        X *= inv;
        Y *= inv;
        Z *= inv;
    }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing() const;               // slot 70 (+0x118)
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                         // +0x60
    const C3DVector& GetPosition() const;              // 0x006A4D00 (PhysicsTC +0xc; out of line, catalog "CVertexBufferWin32::DoSizeof")
};
class CThingCreatureBase : public CThing {
public:
    bool IsThingInFrontCone(const CThing* thing) const;
};

bool CThingCreatureBase::IsThingInFrontCone(const CThing* thing) const
{
    C3DVector forward = PhysicsTC->GetFacing();
    const C3DVector& position = GetPosition();
    C3DVector direction = thing->GetPosition() - position;
    direction.Normalise();
    if (Dot(forward, direction) > (float)cos(0.34906585039886590))
        return true;
    return false;
}

// ---- test stand-ins ----
#define DEFV(n) void CTCPhysicsBase::v##n() { }
DEFV(00) DEFV(01) DEFV(02) DEFV(03) DEFV(04) DEFV(05) DEFV(06) DEFV(07) DEFV(08) DEFV(09)
DEFV(10) DEFV(11) DEFV(12) DEFV(13) DEFV(14) DEFV(15) DEFV(16) DEFV(17) DEFV(18) DEFV(19)
DEFV(20) DEFV(21) DEFV(22) DEFV(23) DEFV(24) DEFV(25) DEFV(26) DEFV(27) DEFV(28) DEFV(29)
DEFV(30) DEFV(31) DEFV(32) DEFV(33) DEFV(34) DEFV(35) DEFV(36) DEFV(37) DEFV(38) DEFV(39)
DEFV(40) DEFV(41) DEFV(42) DEFV(43) DEFV(44) DEFV(45) DEFV(46) DEFV(47) DEFV(48) DEFV(49)
DEFV(50) DEFV(51) DEFV(52) DEFV(53) DEFV(54) DEFV(55) DEFV(56) DEFV(57) DEFV(58) DEFV(59)
DEFV(60) DEFV(61) DEFV(62) DEFV(63) DEFV(64) DEFV(65) DEFV(66) DEFV(67) DEFV(68) DEFV(69)
C3DVector CTCPhysicsBase::GetFacing() const { return C3DVector(0.0f, 0.0f, 1.0f); }

class TestPhysics : public CTCPhysicsBase {
public:
    C3DVector Facing;
    C3DVector Position;
    virtual C3DVector GetFacing() const { return Facing; }
};
const C3DVector& CThing::GetPosition() const { return static_cast<TestPhysics*>(PhysicsTC)->Position; }

static int Fail() { std::printf("00661d70_TEST FAIL\n"); return 1; }

int main()
{
    BuildInvSqrtTable();
    for (float v = 0.01f; v < 5000.0f; v *= 1.37f) {           // the fast reciprocal square root is close
        float want = 1.0f / (float)std::sqrt(v);
        float got = FastInvSqrt(v);
        if (got < want * 0.999f || got > want * 1.001f) return Fail();
    }
    TestPhysics heroPhysics, otherPhysics;
    heroPhysics.Facing = C3DVector(1.0f, 0.0f, 0.0f);
    heroPhysics.Position = C3DVector(5.0f, 2.0f, -1.0f);
    CThingCreatureBase hero; hero.PhysicsTC = &heroPhysics;
    CThing other; other.PhysicsTC = &otherPhysics;
    const float pi = 3.14159265f;
    struct Case { float degrees; float distance; bool inFront; };
    const Case cases[] = { { 0.0f, 10.0f, true }, { 5.0f, 3.0f, true }, { 18.0f, 40.0f, true }, { -18.0f, 7.0f, true },
                           { 22.0f, 10.0f, false }, { -25.0f, 2.0f, false }, { 90.0f, 10.0f, false }, { 180.0f, 1.0f, false } };
    for (unsigned int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        float a = cases[i].degrees * pi / 180.0f;
        otherPhysics.Position = C3DVector(5.0f + cases[i].distance * (float)std::cos(a), 2.0f + cases[i].distance * (float)std::sin(a), -1.0f);
        if (hero.IsThingInFrontCone(&other) != cases[i].inFront) return Fail();
    }
    heroPhysics.Facing = C3DVector(0.0f, 0.0f, -1.0f);         // facing down -Z: a thing below and slightly aside
    otherPhysics.Position = C3DVector(5.5f, 2.0f, -9.0f);
    if (!hero.IsThingInFrontCone(&other)) return Fail();
    otherPhysics.Position = C3DVector(5.0f, 2.0f, 3.0f);
    if (hero.IsThingInFrontCone(&other)) return Fail();
    std::printf("00661d70_TEST PASS\n");
    return 0;
}