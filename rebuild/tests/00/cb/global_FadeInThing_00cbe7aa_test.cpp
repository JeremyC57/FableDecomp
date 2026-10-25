// Behaviour test for 00cbe7aa: FadeInThing steps the alpha up from 0 to 1 at 15 steps a second (frames advance even when the thing is dead)
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
class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface() { }
    virtual void NewScriptFrame() const = 0;
    virtual void RemoveThing(const CScriptThing& thing, bool withEffect, bool killPersistent) const = 0;
    virtual void EntitySetAlpha(const CScriptThing& thing, float alpha, bool b) const = 0;
};
class CTestInterface : public CGameScriptInterface {
public:
    mutable int   Frames;
    mutable int   AlphaCalls;
    mutable float FirstAlpha;
    mutable float LastAlpha;
    mutable int   Removes;
    mutable bool  RemoveEffect;
    mutable bool  RemovePersistent;
    CTestInterface() { Reset(); }
    void Reset() { Frames = 0; AlphaCalls = 0; FirstAlpha = -1.0f; LastAlpha = -1.0f; Removes = 0; RemoveEffect = true; RemovePersistent = false; }
    void NewScriptFrame() const { ++Frames; }
    void RemoveThing(const CScriptThing&, bool withEffect, bool killPersistent) const { ++Removes; RemoveEffect = withEffect; RemovePersistent = killPersistent; }
    void EntitySetAlpha(const CScriptThing&, float alpha, bool) const
    {
        if (AlphaCalls == 0) FirstAlpha = alpha;
        LastAlpha = alpha;
        ++AlphaCalls;
    }
};
static CTestInterface g_testInterface;
static CGameScriptInterface* g_pGameScriptInterface = &g_testInterface;
// ---- the function under test, copied from src/00cbe7aa.cpp (calling convention dropped) ----
void FadeInThing(CScriptThing& thing, float seconds)
{
    float step = 1.0f / (seconds * 15.0f);
    float alpha = 0.0f;
    do {
        if (thing.IsAlive())
            g_pGameScriptInterface->EntitySetAlpha(thing, alpha, true);
        g_pGameScriptInterface->NewScriptFrame();
        alpha += step;
    } while (alpha < 1.0f);
}
static int Fail() { std::printf("00cbe7aa_TEST FAIL\n"); return 1; }
int main()
{
    CTestThing spirit("spirit", 0, 0, 0);
    FadeInThing(spirit, 1.0f);
    if (g_testInterface.Frames < 15 || g_testInterface.Frames > 16 || g_testInterface.AlphaCalls != g_testInterface.Frames) return Fail();
    if (!Near(g_testInterface.FirstAlpha, 0.0f) || g_testInterface.LastAlpha >= 1.0f) return Fail();
    g_testInterface.Reset();
    CTestThing dead("dead", 0, 0, 0);
    dead.Alive = false;
    FadeInThing(dead, 0.2f);
    if (g_testInterface.Frames < 3 || g_testInterface.Frames > 4 || g_testInterface.AlphaCalls != 0) return Fail();
    std::printf("00cbe7aa_TEST PASS\n");
    return 0;
}