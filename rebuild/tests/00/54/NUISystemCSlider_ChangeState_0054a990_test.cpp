#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CSlider::ChangeState observer registration, accept-button fade, cancel restore with actions, empty-range unselectable


#include <math.h>
class C2DVector { public: float X, Y; };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& c) : B(c.B), G(c.G), R(c.R), A(c.A) {}
};
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct IPrimary_0054a990 { virtual void v00() {} };
struct CObserver {
    virtual void a00() {} virtual void a01() {}
    virtual bool IsObservingEvent(long eventType);       // slot 2 (+0x08)
    virtual void ObserveEvent(long eventType);           // slot 3 (+0x0c)
    unsigned char _pad04[0x10];
};
struct ISecondaryB_0054a990 { virtual void b00() {} };
struct CComponentPrimary_0054a990 {
    virtual void c00() {} virtual void c01() {} virtual void c02() {} virtual void c03() {} virtual void c04() {} virtual void c05() {}
    virtual void c06() {} virtual void c07() {} virtual void c08() {} virtual void c09() {} virtual void c10() {} virtual void c11() {}
    virtual void c12() {} virtual void c13() {}
    virtual CRGBColour GetColour() const;                // slot 14 (+0x38)
    virtual void c15() {} virtual void c16() {} virtual void c17() {} virtual void c18() {} virtual void c19() {} virtual void c20() {}
    virtual void c21() {} virtual void c22() {} virtual void c23() {} virtual void c24() {} virtual void c25() {} virtual void c26() {}
    virtual void c27() {} virtual void c28() {} virtual void c29() {} virtual void c30() {} virtual void c31() {} virtual void c32() {}
    virtual void c33() {} virtual void c34() {} virtual void c35() {} virtual void c36() {} virtual void c37() {}
    virtual void ChangeColour(const CRGBColour& colour, float time, bool linear);   // slot 38 (+0x98)
};
struct CComponent : CComponentPrimary_0054a990, CObserver {
};
struct CChangingStateComponent : IPrimary_0054a990, CObserver, ISecondaryB_0054a990 {
    unsigned char _pad1c[0x87 - 0x1c];
    bool Selectable;                                     // +0x87
    unsigned char _pad88[0x188 - 0x88];
    void ChangeState(unsigned long state);               // 0x0052CF40
};
namespace NUISystem { class CAction; }
struct CFrontEndManager {
    virtual void v000() {} virtual void v001() {}
    virtual void AddObserver(CObserver* observer);       // slot 2 (+0x08)
    virtual void v003() {} virtual void v004() {}
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {}
    virtual void v011() {} virtual void v012() {} virtual void v013() {}
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& action);   // slot 14 (+0x38)
    virtual void v015() {} virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {}
    virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {}
    virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {}
    virtual void v030() {} virtual void v031() {} virtual void v032() {}
    virtual long GetLastInputResult();                   // slot 33 (+0x84)
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CSliderValue { unsigned char _pad[8]; float Value; };
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };
struct CSliderTargetRef { CSliderTarget* Target; };
namespace NUISystem {
class CAction { public:
    virtual void v00() {}
    CSliderTargetRef* PTarget;                           // +0x04
};
static __forceinline bool Differs(const C2DVector& a, const C2DVector& b)
{
    return !(fabs(a.X - b.X) <= 0.0001f) || !(fabs(a.Y - b.Y) <= 0.0001f);
}
#pragma pack(push, 1)
class CSlider : public CChangingStateComponent { public:
    unsigned char _pad188[0x18c - 0x188];
    C2DVector     Min;                           // +0x18c
    C2DVector     Max;                           // +0x194
    C2DVector     Step;                          // +0x19c
    C2DVector     Value;                         // +0x1a4
    bool          Move;                          // +0x1ac
    C2DVector     OldValue;                      // +0x1ad
    C2DVector     StartValue;                    // +0x1b5
    unsigned char _pad1bd[3];
    CRetailVector<CCountedPointer<CAction> > Actions;   // +0x1c0
    unsigned char _pad1cc[0x1e0 - 0x1cc];
    C2DVector     PreviousValue;                 // +0x1e0
    CComponent*   PAcceptButton;                 // +0x1e8
    virtual void d00() {} virtual void d01() {} virtual void d02() {} virtual void d03() {} virtual void d04() {} virtual void d05() {}
    virtual void d06() {} virtual void d07() {} virtual void d08() {} virtual void d09() {} virtual void d10() {} virtual void d11() {}
    virtual void d12() {} virtual void d13() {} virtual void d14() {} virtual void d15() {} virtual void d16() {} virtual void d17() {}
    virtual void d18() {} virtual void d19() {} virtual void d20() {} virtual void d21() {} virtual void d22() {} virtual void d23() {}
    virtual void d24() {} virtual void d25() {} virtual void d26() {} virtual void d27() {} virtual void d28() {} virtual void d29() {}
    virtual void d30() {} virtual void d31() {} virtual void d32() {} virtual void d33() {} virtual void d34() {} virtual void d35() {}
    virtual void d36() {} virtual void d37() {} virtual void d38() {} virtual void d39() {} virtual void d40() {} virtual void d41() {}
    virtual void d42() {} virtual void d43() {} virtual void d44() {} virtual void d45() {} virtual void d46() {} virtual void d47() {}
    virtual void d48() {} virtual void d49() {} virtual void d50() {} virtual void d51() {} virtual void d52() {} virtual void d53() {}
    virtual void d54() {} virtual void d55() {} virtual void d56() {} virtual void d57() {} virtual void d58() {} virtual void d59() {}
    virtual void d60() {} virtual void d61() {} virtual void d62() {} virtual void d63() {} virtual void d64() {} virtual void d65() {}
    virtual void d66() {} virtual void d67() {} virtual void d68() {} virtual void d69() {} virtual void d70() {} virtual void d71() {}
    virtual void d72() {} virtual void d73() {} virtual void d74() {} virtual void d75() {} virtual void d76() {} virtual void d77() {}
    virtual void d78() {} virtual void d79() {} virtual void d80() {} virtual void d81() {} virtual void d82() {} virtual void d83() {}
    virtual void d84() {} virtual void d85() {} virtual void d86() {} virtual void d87() {} virtual void d88() {} virtual void d89() {}
    virtual void d90() {} virtual void d91() {} virtual void d92() {} virtual void d93() {} virtual void d94() {} virtual void d95() {}
    virtual void d96() {} virtual void d97() {} virtual void d98() {} virtual void d99() {} virtual void d100() {} virtual void d101() {}
    virtual void d102() {} virtual void d103() {} virtual void d104() {} virtual void d105() {} virtual void d106() {} virtual void d107() {}
    virtual void d108() {} virtual void d109() {} virtual void d110() {} virtual void d111() {} virtual void d112() {} virtual void d113() {}
    virtual void d114() {} virtual void d115() {} virtual void d116() {} virtual void d117() {} virtual void d118() {} virtual void d119() {}
    virtual void d120() {} virtual void d121() {} virtual void d122() {} virtual void d123() {} virtual void d124() {} virtual void d125() {}
    virtual void d126() {} virtual void d127() {} virtual void d128() {} virtual void d129() {} virtual void d130() {} virtual void d131() {}
    virtual void d132() {} virtual void d133() {} virtual void d134() {} virtual void d135() {} virtual void d136() {} virtual void d137() {}
    virtual void d138() {} virtual void d139() {} virtual void d140() {} virtual void d141() {} virtual void d142() {} virtual void d143() {}
    virtual void d144() {} virtual void d145() {} virtual void d146() {} virtual void d147() {} virtual void d148() {}
    virtual void UpdateValue();                  // slot 150 (+0x258)
    void ChangeState(unsigned long state);
};
#pragma pack(pop)

void CSlider::ChangeState(unsigned long state)
{
    switch (state) {
    case 0:
        if (CFrontEndManager::GetInstance()->GetLastInputResult() == 1) {
            OldValue = Value;
            Value = StartValue;
            if (Differs(OldValue, Value)) {
                for (unsigned int i = 0; i < Actions.size(); ++i) {
                    Actions[i]->PTarget->Target->Value->Value = Value.X;
                    CFrontEndManager::GetInstance()->ExecuteAction(Actions[i]);
                }
                Move = true;
            }
        }
        UpdateValue();
        StartValue = Value;
        CChangingStateComponent::ChangeState(state);
        break;
    case 3:
        CFrontEndManager::GetInstance()->AddObserver(this);
        ObserveEvent(5);
        ObserveEvent(4);
        ObserveEvent(0x14);
        ObserveEvent(0x15);
        CChangingStateComponent::ChangeState(state);
        break;
    case 5:
        UpdateValue();
        PreviousValue = StartValue = Value;
        if (PAcceptButton && !PAcceptButton->IsObservingEvent(0x19)) {
            CRGBColour colour = PAcceptButton->GetColour();
            colour.A = 0x80;
            PAcceptButton->ChangeColour(colour, 0.2f, false);
        }
        CChangingStateComponent::ChangeState(state);
        break;
    default:
        CFrontEndManager::GetInstance()->RemoveObserver(this);
        CChangingStateComponent::ChangeState(state);
        break;
    }
    if (Max.X == Min.X)
        Selectable = false;
}
}

static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
static CObserver* g_added = 0; static CObserver* g_removed = 0;
static unsigned long g_baseState = 99;
static long g_inputResult = 0;
static int g_executed = 0;
static float g_fadeTime = 0; static bool g_fadeLinear = true; static unsigned char g_fadeAlpha = 0, g_fadeR = 0;
static bool g_observing = false;
static int g_updates = 0;
bool CObserver::IsObservingEvent(long e) { Log(1000 + (int)e); return g_observing; }
void CObserver::ObserveEvent(long e) { Log((int)e); }
CRGBColour CComponentPrimary_0054a990::GetColour() const { CRGBColour c; c.B = 1; c.G = 2; c.R = 3; c.A = 0xff; return c; }
void CComponentPrimary_0054a990::ChangeColour(const CRGBColour& c, float t, bool linear) { g_fadeAlpha = c.A; g_fadeR = c.R; g_fadeTime = t; g_fadeLinear = linear; }
void CChangingStateComponent::ChangeState(unsigned long s) { g_baseState = s; }
void CFrontEndManager::AddObserver(CObserver* o) { g_added = o; }
void CFrontEndManager::RemoveObserver(CObserver* o) { g_removed = o; }
void CFrontEndManager::ExecuteAction(const CCountedPointer<NUISystem::CAction>&) { ++g_executed; }
long CFrontEndManager::GetLastInputResult() { return g_inputResult; }
static CFrontEndManager g_mgr;
CFrontEndManager* __fastcall CFrontEndManager::GetInstance() { return &g_mgr; }
void NUISystem::CSlider::UpdateValue() { ++g_updates; }
static int fails = 0;
static void Check(bool c, int line) { if (!c) { std::printf("fail line %d\n", line); ++fails; } }
int main()
{
    using namespace NUISystem;
    static CSlider s; std::memset(&s.Min, 0, sizeof(C2DVector) * 4);
    s.Selectable = true; s.Min.X = 0; s.Max.X = 10; s.Min.Y = 0; s.Max.Y = 0;
    s.Value.X = 3; s.Value.Y = 4; s.StartValue.X = 0; s.StartValue.Y = 0;
    s.Actions._M_start = s.Actions._M_finish = s.Actions._M_end_of_storage = 0; s.PAcceptButton = 0;
    // default: unregister
    s.ChangeState(7);
    Check(g_removed == static_cast<CObserver*>(&s), __LINE__); Check(g_baseState == 7, __LINE__); Check(s.Selectable, __LINE__);
    // 3: register and observe four events
    g_n = 0; s.ChangeState(3);
    Check(g_added == static_cast<CObserver*>(&s), __LINE__); Check(g_baseState == 3, __LINE__);
    Check(g_n == 4 && g_log[0] == 5 && g_log[1] == 4 && g_log[2] == 0x14 && g_log[3] == 0x15, __LINE__);
    // 5: remember value, fade the accept button
    static CComponent accept; s.PAcceptButton = &accept; g_n = 0;
    s.ChangeState(5);
    Check(g_updates == 1 && g_baseState == 5, __LINE__);
    Check(s.StartValue.X == 3 && s.StartValue.Y == 4 && s.PreviousValue.X == 3 && s.PreviousValue.Y == 4, __LINE__);
    Check(g_n == 1 && g_log[0] == 1000 + 0x19, __LINE__);
    Check(g_fadeAlpha == 0x80 && g_fadeR == 3 && g_fadeTime == 0.2f && !g_fadeLinear, __LINE__);
    g_observing = true; g_fadeAlpha = 0; s.ChangeState(5); Check(g_fadeAlpha == 0, __LINE__);
    // 0 with a cancel: restore start value, run actions
    static CSliderValue val; val.Value = -1; static CSliderTarget tgt; tgt.Value = &val; static CSliderTargetRef ref; ref.Target = &tgt;
    static CAction act; act.PTarget = &ref;
    static CCountedPointer<CAction> acts[2]; acts[0].Data = &act; acts[1].Data = &act;
    s.Actions._M_start = acts; s.Actions._M_finish = acts + 2; s.Actions._M_end_of_storage = acts + 2;
    s.StartValue.X = 1; s.StartValue.Y = 2; s.Value.X = 6; s.Value.Y = 2; s.Move = false; g_inputResult = 1; g_updates = 0;
    s.ChangeState(0);
    Check(s.OldValue.X == 6 && s.OldValue.Y == 2 && s.Value.X == 1 && s.Value.Y == 2, __LINE__);
    Check(g_executed == 2 && val.Value == 1 && s.Move, __LINE__);
    Check(g_updates == 1 && s.StartValue.X == 1 && g_baseState == 0, __LINE__);
    // 0 with a cancel but no movement: no actions
    s.Move = false; s.ChangeState(0); Check(g_executed == 2 && !s.Move, __LINE__);
    // 0 without a cancel: value kept, start value follows it
    g_inputResult = 0; s.Value.X = 9; s.ChangeState(0); Check(s.Value.X == 9 && s.StartValue.X == 9 && g_executed == 2, __LINE__);
    // empty range -> unselectable
    s.Max.X = 0; s.ChangeState(4); Check(!s.Selectable && g_baseState == 4, __LINE__);
    if (fails) { std::printf("0054a990_TEST FAIL\n"); return 1; }
    std::printf("0054a990_TEST PASS\n");
    return 0;
}