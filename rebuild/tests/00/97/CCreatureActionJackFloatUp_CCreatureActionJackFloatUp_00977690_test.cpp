// behaviour test for 0x00977690 CCreatureAction_JackFloatUp::CCreatureAction_JackFloatUp(const CCreatureAction_JackFloatUp&) (model; the base copy constructor is a stub)
#include <cstdio>
#include <cstring>
static int g_copies = 0;
class CCreatureActionBase { public: virtual ~CCreatureActionBase() { } };
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase() { }
    CCombatActionBase(const CCombatActionBase&) { ++g_copies; }
};
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
class CCreatureAction_JackFloatUp : public CCombatActionBase {
public:
    CCreatureAction_JackFloatUp() { }
    CCreatureAction_JackFloatUp(const CCreatureAction_JackFloatUp& other);
    float FloatHeight;                        // +0x114 (PDB +300)
    float FloatSpeed;                         // +0x118
    bool Moving;                              // +0x11C
    float StartHeight;                        // +0x120
};
CCreatureAction_JackFloatUp::CCreatureAction_JackFloatUp(const CCreatureAction_JackFloatUp& other) : CCombatActionBase(other)
    , FloatHeight(other.FloatHeight)
    , FloatSpeed(other.FloatSpeed)
    , Moving(other.Moving)
    , StartHeight(other.StartHeight) { }
int main()
{
    CCreatureAction_JackFloatUp a;
    a.FloatHeight = 2.25f;
    a.FloatSpeed = 3.25f;
    a.Moving = true;
    a.StartHeight = 5.25f;
    CCreatureAction_JackFloatUp b(a);
    int ok = g_copies == 1 && b.FloatHeight == 2.25f && b.FloatSpeed == 3.25f && b.Moving && b.StartHeight == 5.25f;
    if (!ok) { std::printf("00977690_TEST FAIL\n"); return 1; }
    std::printf("00977690_TEST PASS\n");
    return 0;
}