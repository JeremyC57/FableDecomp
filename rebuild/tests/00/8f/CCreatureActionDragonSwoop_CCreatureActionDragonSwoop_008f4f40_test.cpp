// Behaviour test for 0x008F4F40 CCreatureAction_DragonSwoop constructor: base gets owner, code 150 and a silent sound
// pair; the animation argument is forwarded to SetAnimationName and the three points are copied in order.
#include <cstdio>
#include <cstring>

struct CThingCreatureBase { int id; };
struct CCharString { const char* m_pData; };
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct C3DVector { float X, Y, Z; };
struct CCombatActionBase {
    CThingCreatureBase* m_owner; long m_code; CSoundPair m_sound; const CCharString* m_anim;
    CCombatActionBase(CThingCreatureBase* owner, long code, const CSoundPair& s)
        : m_owner(owner), m_code(code), m_sound(s), m_anim(0) { }
    void SetAnimationName(const CCharString& n) { m_anim = &n; }
};
struct CCreatureAction_DragonSwoop : CCombatActionBase {
    C3DVector SwoopPoints[3];
    CCreatureAction_DragonSwoop(CThingCreatureBase* owner, const CCharString& animation,
                                const C3DVector& p0, const C3DVector& p1, const C3DVector& p2)
        : CCombatActionBase(owner, 150, CSoundPair())
    {
        SetAnimationName(animation);
        SwoopPoints[0] = p0; SwoopPoints[1] = p1; SwoopPoints[2] = p2;
    }
};
int main()
{
    CThingCreatureBase owner = { 1 };
    CCharString anim = { "SWOOP" };
    C3DVector a = { 1, 2, 3 }, b = { 4, 5, 6 }, c = { 7, 8, 9 };
    CCreatureAction_DragonSwoop s(&owner, anim, a, b, c);
    if (s.m_owner != &owner || s.m_code != 150 || s.m_sound.FirstSound != 0 || s.m_anim != &anim ||
        s.SwoopPoints[0].X != 1 || s.SwoopPoints[0].Z != 3 || s.SwoopPoints[1].Y != 5 || s.SwoopPoints[2].X != 7 ||
        s.SwoopPoints[2].Z != 9) {
        std::printf("008f4f40_TEST FAIL\n"); return 1;
    }
    std::printf("008f4f40_TEST PASS\n");
    return 0;
}