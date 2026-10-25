// Behaviour test for 00cbe551: GetDistanceBetweenThingAndPosition is the 3D distance to a point
#include <cstdio>
#include <cstring>
#include <math.h>
class CCharString {
public:
    const char* m_pStringData;
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    float GetLengthSquared() const { return X * X + Y * Y; }
};
// the slots the helpers use (retail vtable 0x01238C8C order kept)
class CScriptThing {
public:
    virtual ~CScriptThing() { }
    virtual const CCharString& GetName() const = 0;
    virtual const C3DVector& GetPos() const = 0;
    virtual bool MsgIsTriggeredBy(const CCharString& by) const = 0;
    virtual bool MsgIsHitBy(const CCharString& by) const = 0;
    virtual bool MsgIsKnockedOutBy(const CCharString& by) const = 0;
    virtual bool IsAlive() const = 0;
    virtual bool IsNull() const = 0;
};
class CTestThing : public CScriptThing {
public:
    CCharString Name;
    C3DVector   Pos;
    bool        Alive;
    bool        Null;
    int         AliveFor;              // >= 0: IsAlive() answers true only this many more times
    mutable int AliveCalls;
    const char* HitBy;
    const char* KnockedOutBy;
    const char* TriggeredBy;
    CTestThing(const char* name, float x, float y, float z)
        : Pos(x, y, z), Alive(true), Null(false), AliveFor(-1), AliveCalls(0), HitBy(0), KnockedOutBy(0), TriggeredBy(0)
    {
        Name.m_pStringData = name;
    }
    static bool Same(const char* a, const CCharString& b) { return a && std::strcmp(a, b.m_pStringData) == 0; }
    const CCharString& GetName() const { return Name; }
    const C3DVector& GetPos() const { return Pos; }
    bool MsgIsTriggeredBy(const CCharString& by) const { return Same(TriggeredBy, by); }
    bool MsgIsHitBy(const CCharString& by) const { return Same(HitBy, by); }
    bool MsgIsKnockedOutBy(const CCharString& by) const { return Same(KnockedOutBy, by); }
    bool IsAlive() const
    {
        ++AliveCalls;
        if (AliveFor >= 0) return AliveCalls <= AliveFor;
        return Alive;
    }
    bool IsNull() const { return Null; }
};
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }
// ---- the function under test, copied from src/00cbe551.cpp (calling convention dropped) ----
float GetDistanceBetweenThingAndPosition(const CScriptThing& thing, const C3DVector& position)
{
    return (thing.GetPos() - position).GetLength();
}
static int Fail() { std::printf("00cbe551_TEST FAIL\n"); return 1; }
int main()
{
    CTestThing a("a", 0, 0, 0);
    C3DVector p(2, 10, 11);                               // 15
    if (!Near(GetDistanceBetweenThingAndPosition(a, p), 15.0f)) return Fail();
    std::printf("00cbe551_TEST PASS\n");
    return 0;
}