// test for 004bc000 CTCGraphicAppearance::GetAnims
#include <stdio.h>
#include <string.h>
class C3DAnimationBlendState { public: long Data[2]; };
class C3DAnimationInterpolator {
public:
    long                   _pad00;                       // +0x00
    C3DAnimationBlendState StartState;                   // +0x04
    C3DAnimationBlendState EndState;                     // +0x0c
};
class CTCAnimationBase {                                 // vtable 0x01265024
public:
    virtual ~CTCAnimationBase();                         // slot 0
    virtual void _slot01();
    virtual void _slot02();
    virtual void _slot03();
    virtual void _slot04();
    virtual void _slot05();
    virtual void _slot06();
    virtual void _slot07();
    virtual void _slot08();
    virtual void _slot09();
    virtual void _slot10();
    virtual void _slot11();
    virtual void _slot12();
    virtual void _slot13();
    virtual void _slot14();
    virtual void _slot15();
    virtual void _slot16();
    virtual void _slot17();
    virtual void _slot18();
    virtual void _slot19();
    virtual void _slot20();
    virtual void _slot21();
    virtual void _slot22();
    virtual void _slot23();
    virtual void _slot24();
    virtual void _slot25();
    virtual void _slot26();
    virtual void _slot27();
    virtual void GetInterpolatedMeshAnim(C3DAnimationBlendState& state, float t) const = 0;            // slot 28
    virtual void GetMeshAnims(C3DAnimationBlendState& start, C3DAnimationBlendState& end) const = 0;  // slot 29
};
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char     _pad00[0x24];                      // +0x00 CTCBase, MeshEffects, TintColours
    CTCAnimationBase* PTCAnimation;                      // +0x24
public:
    bool GetAnims(C3DAnimationInterpolator& interpolator) const;
};
bool CTCGraphicAppearance::GetAnims(C3DAnimationInterpolator& interpolator) const
{
    if (PTCAnimation) {
        PTCAnimation->GetMeshAnims(interpolator.StartState, interpolator.EndState);
        return true;
    }
    return false;
}
}

static void* g_s; static void* g_e; static int g_calls;
CTCAnimationBase::~CTCAnimationBase() {}
void CTCAnimationBase::_slot01() {}
void CTCAnimationBase::_slot02() {}
void CTCAnimationBase::_slot03() {}
void CTCAnimationBase::_slot04() {}
void CTCAnimationBase::_slot05() {}
void CTCAnimationBase::_slot06() {}
void CTCAnimationBase::_slot07() {}
void CTCAnimationBase::_slot08() {}
void CTCAnimationBase::_slot09() {}
void CTCAnimationBase::_slot10() {}
void CTCAnimationBase::_slot11() {}
void CTCAnimationBase::_slot12() {}
void CTCAnimationBase::_slot13() {}
void CTCAnimationBase::_slot14() {}
void CTCAnimationBase::_slot15() {}
void CTCAnimationBase::_slot16() {}
void CTCAnimationBase::_slot17() {}
void CTCAnimationBase::_slot18() {}
void CTCAnimationBase::_slot19() {}
void CTCAnimationBase::_slot20() {}
void CTCAnimationBase::_slot21() {}
void CTCAnimationBase::_slot22() {}
void CTCAnimationBase::_slot23() {}
void CTCAnimationBase::_slot24() {}
void CTCAnimationBase::_slot25() {}
void CTCAnimationBase::_slot26() {}
void CTCAnimationBase::_slot27() {}
struct TestAnim : CTCAnimationBase {
    virtual void GetInterpolatedMeshAnim(C3DAnimationBlendState&, float) const {}
    virtual void GetMeshAnims(C3DAnimationBlendState& s, C3DAnimationBlendState& e) const { g_s = &s; g_e = &e; ++g_calls; }
};
int main()
{
    TestAnim anim; C3DAnimationInterpolator interp;
    static double buf[8]; NGraphicAppearance::CTCGraphicAppearance* ga = (NGraphicAppearance::CTCGraphicAppearance*)buf;
    ga->PTCAnimation = 0;
    int ok = ga->GetAnims(interp) == false && g_calls == 0;
    ga->PTCAnimation = &anim;
    ok &= ga->GetAnims(interp) == true && g_calls == 1 && g_s == &interp.StartState && g_e == &interp.EndState;
    printf(ok ? "004bc000_TEST PASS\n" : "004bc000_TEST FAIL\n");
    return ok ? 0 : 1;
}