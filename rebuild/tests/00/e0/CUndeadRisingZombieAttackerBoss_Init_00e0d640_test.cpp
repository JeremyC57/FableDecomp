// Behaviour test for CUndeadRisingZombieAttackerBoss::Init (0x00E0D640).
#include <cstdio>

struct C3DVector { float X, Y, Z; };

struct CBossTarget {
    virtual void v000() { }
    virtual void v001() { }
    virtual void v002() { }
    virtual void v003() { }
    virtual void v004() { }
    virtual void v005() { }
    virtual const C3DVector& GetPosition() { return m_pos; }
    C3DVector m_pos;
};

struct CBossOwner {
    virtual void v000() { }
    virtual void v001() { }
    virtual void v002() { }
    virtual void v003() { }
    virtual void v004() { }
    virtual void v005() { }
    virtual void v006() { }
    virtual void v007() { }
    virtual void v008() { }
    virtual void v009() { }
    virtual void v00a() { }
    virtual void v00b() { }
    virtual void v00c() { }
    virtual void v00d() { }
    virtual void v00e() { }
    virtual void v00f() { }
    virtual void v010() { }
    virtual void v011() { }
    virtual void v012() { }
    virtual void v013() { }
    virtual void v014() { }
    virtual void v015() { }
    virtual void v016() { }
    virtual void v017() { }
    virtual void v018() { }
    virtual void v019() { }
    virtual void v01a() { }
    virtual void v01b() { }
    virtual void v01c() { }
    virtual void v01d() { }
    virtual void v01e() { }
    virtual void v01f() { }
    virtual void v020() { }
    virtual void v021() { }
    virtual void v022() { }
    virtual void v023() { }
    virtual void v024() { }
    virtual void v025() { }
    virtual void v026() { }
    virtual void v027() { }
    virtual void v028() { }
    virtual void v029() { }
    virtual void v02a() { }
    virtual void v02b() { }
    virtual void v02c() { }
    virtual void v02d() { }
    virtual void v02e() { }
    virtual void v02f() { }
    virtual void v030() { }
    virtual void v031() { }
    virtual void v032() { }
    virtual void v033() { }
    virtual void v034() { }
    virtual void v035() { }
    virtual void v036() { }
    virtual void v037() { }
    virtual void v038() { }
    virtual void v039() { }
    virtual void v03a() { }
    virtual void v03b() { }
    virtual void v03c() { }
    virtual void v03d() { }
    virtual void v03e() { }
    virtual void v03f() { }
    virtual void v040() { }
    virtual void v041() { }
    virtual void v042() { }
    virtual void v043() { }
    virtual void v044() { }
    virtual void v045() { }
    virtual void v046() { }
    virtual void v047() { }
    virtual void v048() { }
    virtual void v049() { }
    virtual void v04a() { }
    virtual void v04b() { }
    virtual void v04c() { }
    virtual void v04d() { }
    virtual void v04e() { }
    virtual void v04f() { }
    virtual void v050() { }
    virtual void v051() { }
    virtual void v052() { }
    virtual void v053() { }
    virtual void v054() { }
    virtual void v055() { }
    virtual void v056() { }
    virtual void v057() { }
    virtual void v058() { }
    virtual void v059() { }
    virtual void v05a() { }
    virtual void v05b() { }
    virtual void v05c() { }
    virtual void v05d() { }
    virtual void v05e() { }
    virtual void v05f() { }
    virtual void v060() { }
    virtual void v061() { }
    virtual void v062() { }
    virtual void v063() { }
    virtual void v064() { }
    virtual void v065() { }
    virtual void v066() { }
    virtual void v067() { }
    virtual void v068() { }
    virtual void v069() { }
    virtual void v06a() { }
    virtual void v06b() { }
    virtual void v06c() { }
    virtual void v06d() { }
    virtual void v06e() { }
    virtual void v06f() { }
    virtual void v070() { }
    virtual void v071() { }
    virtual void v072() { }
    virtual void v073() { }
    virtual void v074() { }
    virtual void v075() { }
    virtual void v076() { }
    virtual void v077() { }
    virtual void v078() { }
    virtual void v079() { }
    virtual void v07a() { }
    virtual void v07b() { }
    virtual void v07c() { }
    virtual void v07d() { }
    virtual void v07e() { }
    virtual void v07f() { }
    virtual void v080() { }
    virtual void v081() { }
    virtual void v082() { }
    virtual void v083() { }
    virtual void v084() { }
    virtual void v085() { }
    virtual void v086() { }
    virtual void v087() { }
    virtual void v088() { }
    virtual void v089() { }
    virtual void v08a() { }
    virtual void v08b() { }
    virtual void v08c() { }
    virtual void v08d() { }
    virtual void v08e() { }
    virtual void v08f() { }
    virtual void v090() { }
    virtual void v091() { }
    virtual void v092() { }
    virtual void v093() { }
    virtual void v094() { }
    virtual void v095() { }
    virtual void v096() { }
    virtual void v097() { }
    virtual void v098() { }
    virtual void v099() { }
    virtual void v09a() { }
    virtual void v09b() { }
    virtual void v09c() { }
    virtual void v09d() { }
    virtual void v09e() { }
    virtual void v09f() { }
    virtual void v0a0() { }
    virtual void v0a1() { }
    virtual void v0a2() { }
    virtual void v0a3() { }
    virtual void v0a4() { }
    virtual void v0a5() { }
    virtual void v0a6() { }
    virtual void v0a7() { }
    virtual void v0a8() { }
    virtual void v0a9() { }
    virtual void v0aa() { }
    virtual void v0ab() { }
    virtual void v0ac() { }
    virtual void v0ad() { }
    virtual void v0ae() { }
    virtual void v0af() { }
    virtual void v0b0() { }
    virtual void v0b1() { }
    virtual void v0b2() { }
    virtual void v0b3() { }
    virtual void v0b4() { }
    virtual void v0b5() { }
    virtual void v0b6() { }
    virtual void v0b7() { }
    virtual void v0b8() { }
    virtual void v0b9() { }
    virtual void v0ba() { }
    virtual void v0bb() { }
    virtual void v0bc() { }
    virtual void v0bd() { }
    virtual void v0be() { }
    virtual void v0bf() { }
    virtual void v0c0() { }
    virtual void v0c1() { }
    virtual void v0c2() { }
    virtual void v0c3() { }
    virtual void v0c4() { }
    virtual void v0c5() { }
    virtual void v0c6() { }
    virtual void v0c7() { }
    virtual void v0c8() { }
    virtual void v0c9() { }
    virtual void v0ca() { }
    virtual void v0cb() { }
    virtual void v0cc() { }
    virtual void v0cd() { }
    virtual void v0ce() { }
    virtual void v0cf() { }
    virtual void v0d0() { }
    virtual void v0d1() { }
    virtual void v0d2() { }
    virtual void v0d3() { }
    virtual void v0d4() { }
    virtual void v0d5() { }
    virtual void v0d6() { }
    virtual void v0d7() { }
    virtual void v0d8() { }
    virtual void v0d9() { }
    virtual void v0da() { }
    virtual void v0db() { }
    virtual void v0dc() { }
    virtual void v0dd() { }
    virtual void v0de() { }
    virtual void v0df() { }
    virtual void v0e0() { }
    virtual void v0e1() { }
    virtual void v0e2() { }
    virtual void v0e3() { }
    virtual void v0e4() { }
    virtual void v0e5() { }
    virtual void v0e6() { }
    virtual void v0e7() { }
    virtual void v0e8() { }
    virtual void v0e9() { }
    virtual void v0ea() { }
    virtual void v0eb() { }
    virtual void v0ec() { }
    virtual void v0ed() { }
    virtual void v0ee() { }
    virtual void v0ef() { }
    virtual void v0f0() { }
    virtual void v0f1() { }
    virtual void v0f2() { }
    virtual void v0f3() { }
    virtual void v0f4() { }
    virtual void v0f5() { }
    virtual void v0f6() { }
    virtual void v0f7() { }
    virtual void v0f8() { }
    virtual void v0f9() { }
    virtual void v0fa() { }
    virtual void v0fb() { }
    virtual void v0fc() { }
    virtual void v0fd() { }
    virtual void v0fe() { }
    virtual void v0ff() { }
    virtual void v100() { }
    virtual void v101() { }
    virtual void v102() { }
    virtual void v103() { }
    virtual void v104() { }
    virtual void v105() { }
    virtual void v106() { }
    virtual void v107() { }
    virtual float MeasureAgainst(CBossTarget* target) { m_seen = target; return 42.5f; }
    CBossTarget* m_seen;
};

struct CUndeadRisingZombieAttackerBoss {
    virtual void v000() { }
    CBossOwner* m_pOwner;
    CBossTarget m_target;
    C3DVector m_targetPosition;
    float m_measure;
    void Init();
};

void CUndeadRisingZombieAttackerBoss::Init()
{
    m_measure = m_pOwner->MeasureAgainst(&m_target);
    m_targetPosition = m_target.GetPosition();
}

int main()
{
    CBossOwner owner;
    owner.m_seen = 0;
    CUndeadRisingZombieAttackerBoss b;
    b.m_pOwner = &owner;
    b.m_target.m_pos.X = 1.0f; b.m_target.m_pos.Y = 2.0f; b.m_target.m_pos.Z = 3.0f;
    b.m_measure = 0.0f;
    b.Init();
    if (owner.m_seen != &b.m_target || b.m_measure != 42.5f || b.m_targetPosition.X != 1.0f
        || b.m_targetPosition.Y != 2.0f || b.m_targetPosition.Z != 3.0f) {
        std::printf("00e0d640_TEST FAIL\n");
        return 1;
    }
    std::printf("00e0d640_TEST PASS\n");
    return 0;
}