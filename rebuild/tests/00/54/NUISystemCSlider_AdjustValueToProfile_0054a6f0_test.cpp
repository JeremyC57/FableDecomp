#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CSlider::AdjustValueToProfile reads the profile setting, lights the accept button and plays the slider sound on a change


#include <math.h>
struct C2DVector { float X, Y; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& c) : B(c.B), G(c.G), R(c.R), A(c.A) {}
};
struct CCPPointerInfo { long Count; };
template <class T> struct CCountedPointer {
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
namespace NUISystem { struct CAction; }
struct CRefCounted {                             // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CDefBase {
    virtual void v00() {}
    virtual void Destroy();                      // slot 1
    CRefCounted Ref;                             // +0x04
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
struct CSoundName { long Value; };
struct CFrontEndManagerDef : CDefBase {
    unsigned char _pad08[0x19c - 8];
    CSoundName    SliderSound;                   // +0x19c (descriptive)
};
template <class T> struct CDefPointer {
    T* Data;
    explicit CDefPointer(T* d) : Data(d) {}
    CDefPointer(const CDefPointer& o) : Data(o.Data) { if (Data) ++Data->Ref.Count; }
    ~CDefPointer()
    {
        if (Data)
            Data->ReleaseTemporary();
    }
    T* operator->() const { return Data; }
};
struct CUserProfileManager {                     // retail = Ego_r.pdb with 12-byte vectors
    unsigned char _pad00[0xc];
    bool  UseNewProfileValues;                   // +0x0c
    unsigned char _pad0d[0x40 - 0xd];
    float ShadowDetail;                          // +0x40
    unsigned char _pad44[0xfc - 0x44];
    float ShadowDetailNewProfile;                // +0xfc
    static CUserProfileManager* Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    float GetGamma() const;                      // 0x00437470
    float GetHUDAlpha() const;                   // 0x00437490
    float GetSoundVolume() const;                // 0x0047CFC2
    float GetDialogueVolume() const;             // 0x0047CFD3
    float GetMusicVolume() const;                // 0x0054A4F0
    float GetCameraSensitivity() const;          // 0x0054A510
    float GetTextureDetail() const;              // 0x0054A530
    float GetEffectsDetail() const;              // 0x0054A570
    float GetMeshDetail() const;                 // 0x0054A590
    float GetShadowDetail() const { return UseNewProfileValues ? ShadowDetailNewProfile : ShadowDetail; }
};
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
    virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {}
    virtual void PlaySound(const CSoundName& sound, bool a);   // slot 40 (+0xa0)
    virtual void v041() {}
    virtual CDefPointer<CFrontEndManagerDef> GetDef();   // slot 42 (+0xa8)
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CSliderValue { unsigned char _pad[8]; float Value; };
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };
struct CSliderTargetRef { CSliderTarget* Target; };
namespace NUISystem {
struct CAction {
    virtual void v00() {}
    CSliderTargetRef* PTarget;                           // +0x04
};
static __forceinline bool Differs(const C2DVector& a, const C2DVector& b)
{
    return !(fabs(a.X - b.X) <= 0.0001f) || !(fabs(a.Y - b.Y) <= 0.0001f);
}
#pragma pack(push, 1)
struct CSlider : CChangingStateComponent {
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
    unsigned char _pad1cc[0x1d4 - 0x1cc];
    long          PDefAction;                    // +0x1d4 EActionType
    unsigned char _pad1d8[0x1e0 - 0x1d8];
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
    void AdjustValueToProfile();
};
#pragma pack(pop)

void CSlider::AdjustValueToProfile()
{
    OldValue = Value;
    if (PDefAction != 0) {
        switch (PDefAction) {
        case 0x29:  Value.X = CUserProfileManager::Get()->GetSoundVolume(); break;
        case 0x58:  Value.X = CUserProfileManager::Get()->GetDialogueVolume(); break;
        case 0x2a:  Value.X = CUserProfileManager::Get()->GetMusicVolume(); break;
        case 0x4e:  Value.X = CUserProfileManager::Get()->GetHUDAlpha(); break;
        case 0x28:  Value.X = CUserProfileManager::Get()->GetGamma(); break;
        case 0x103: Value.X = CUserProfileManager::Get()->GetShadowDetail(); break;
        case 0x10a: Value.X = CUserProfileManager::Get()->GetTextureDetail(); break;
        case 0x13e: Value.X = CUserProfileManager::Get()->GetCameraSensitivity(); break;
        case 0x12a: Value.X = CUserProfileManager::Get()->GetMeshDetail(); break;
        case 0x12b: Value.X = CUserProfileManager::Get()->GetEffectsDetail(); break;
        }
    }
    if (Differs(OldValue, Value)) {
        Move = true;
        if (PAcceptButton && !PAcceptButton->IsObservingEvent(0x19)) {
            PAcceptButton->ObserveEvent(0x19);
            CRGBColour colour = PAcceptButton->GetColour();
            colour.A = 0xff;
            PAcceptButton->ChangeColour(colour, 0.2f, false);
        }
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetDef()->SliderSound, true);
    }
}
}

static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
static bool g_observing = false; static unsigned char g_fadeAlpha = 0; static float g_fadeTime = 0;
static long g_played = 0; static bool g_playedFlag = false; static int g_destroyed = 0;
bool CObserver::IsObservingEvent(long e) { Log(1000 + (int)e); return g_observing; }
void CObserver::ObserveEvent(long e) { Log((int)e); }
CRGBColour CComponentPrimary_0054a990::GetColour() const { CRGBColour c; c.B = 1; c.G = 2; c.R = 3; c.A = 0x10; return c; }
void CComponentPrimary_0054a990::ChangeColour(const CRGBColour& c, float t, bool) { g_fadeAlpha = c.A; g_fadeTime = t; }
void CChangingStateComponent::ChangeState(unsigned long) {}
void CFrontEndManager::AddObserver(CObserver*) {}
void CFrontEndManager::RemoveObserver(CObserver*) {}
void CFrontEndManager::ExecuteAction(const CCountedPointer<NUISystem::CAction>&) {}
long CFrontEndManager::GetLastInputResult() { return 0; }
void CFrontEndManager::PlaySound(const CSoundName& s, bool a) { g_played = s.Value; g_playedFlag = a; }
void CDefBase::Destroy() { ++g_destroyed; }
static CFrontEndManagerDef g_def;
CDefPointer<CFrontEndManagerDef> CFrontEndManager::GetDef() { ++g_def.Ref.Count; return CDefPointer<CFrontEndManagerDef>(&g_def); }
static CFrontEndManager g_mgr;
CFrontEndManager* __fastcall CFrontEndManager::GetInstance() { return &g_mgr; }
void NUISystem::CSlider::UpdateValue() {}
static CUserProfileManager g_prof;
CUserProfileManager* CUserProfileManager::Get() { return &g_prof; }
float CUserProfileManager::GetGamma() const { return 1.5f; }
float CUserProfileManager::GetHUDAlpha() const { return 2.5f; }
float CUserProfileManager::GetSoundVolume() const { return 3.5f; }
float CUserProfileManager::GetDialogueVolume() const { return 4.5f; }
float CUserProfileManager::GetMusicVolume() const { return 5.5f; }
float CUserProfileManager::GetCameraSensitivity() const { return 6.5f; }
float CUserProfileManager::GetTextureDetail() const { return 7.5f; }
float CUserProfileManager::GetEffectsDetail() const { return 8.5f; }
float CUserProfileManager::GetMeshDetail() const { return 9.5f; }
static int fails = 0;
static void Check(bool c, int line) { if (!c) { std::printf("fail line %d\n", line); ++fails; } }
int main()
{
    using namespace NUISystem;
    g_def.Ref.Count = 1; g_def.SliderSound.Value = 77;
    g_prof.UseNewProfileValues = false; g_prof.ShadowDetail = 0.25f; g_prof.ShadowDetailNewProfile = 0.75f;
    static CSlider s; s.Value.X = 0; s.Value.Y = 0; s.Move = false; s.PAcceptButton = 0;
    /* true mapping from retail tables 0x0054A8EC/0x0054A904 and 0x0054A938/0x0054A94C (0x103 inline) */
    const long acts[10] = { 0x28, 0x29, 0x2a, 0x4e, 0x58, 0x10a, 0x12a, 0x12b, 0x13e, 0x103 };
    const float want[10] = { 1.5f, 3.5f, 5.5f, 2.5f, 4.5f, 7.5f, 9.5f, 8.5f, 6.5f, 0.25f };
    for (int i = 0; i < 10; ++i) {
        s.PDefAction = acts[i]; s.Value.X = -1; s.AdjustValueToProfile();
        Check(s.Value.X == want[i] && s.OldValue.X == -1, __LINE__);
    }
    g_prof.UseNewProfileValues = true; s.Value.X = 0; s.AdjustValueToProfile(); Check(s.Value.X == 0.75f, __LINE__);
    Check(s.Move && g_played == 77 && g_playedFlag && g_def.Ref.Count == 1, __LINE__);
    // unchanged value: nothing happens
    s.Move = false; g_played = 0; s.AdjustValueToProfile(); Check(!s.Move && g_played == 0, __LINE__);
    // action 0 and unknown actions leave the value alone
    s.PDefAction = 0; s.Value.X = 42; s.AdjustValueToProfile(); Check(s.Value.X == 42 && s.OldValue.X == 42 && !s.Move, __LINE__);
    s.PDefAction = 0x2b; s.AdjustValueToProfile(); Check(s.Value.X == 42 && !s.Move, __LINE__);
    // accept button lit when not yet observing
    static CComponent accept; s.PAcceptButton = &accept; s.PDefAction = 0x28; g_n = 0;
    s.AdjustValueToProfile();
    Check(g_n == 2 && g_log[0] == 1000 + 0x19 && g_log[1] == 0x19 && g_fadeAlpha == 0xff && g_fadeTime == 0.2f, __LINE__);
    // already observing: no re-light; last reference released destroys the def
    g_observing = true; g_n = 0; g_fadeAlpha = 0; s.PDefAction = 0x29; g_def.Ref.Count = 0;
    s.AdjustValueToProfile();
    Check(g_n == 1 && g_fadeAlpha == 0 && g_destroyed == 1 && g_played == 77, __LINE__);
    if (fails) { std::printf("0054a6f0_TEST FAIL\n"); return 1; }
    std::printf("0054a6f0_TEST PASS\n");
    return 0;
}