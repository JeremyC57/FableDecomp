// Behaviour test for NUISystem::CTextSlider::AdjustValueToProfile (0x00548B50): each of the 18 default actions reads its
// own user-profile getter into SelectedChild (the tutorial flag through UseNewProfileValues, WASD read inline); an
// unknown or zero action leaves it. Only a change sets Move, stamps LastInputTime from m_time, makes the accept button
// observe event 0x19 and fade to full alpha (0.2 s, not linear) unless it already observes it, and plays the front-end
// def's text-slider sound; the def pointer taken by value is released again.
#include <stdio.h>
#include <string.h>

// ---- reconstruction under test (verbatim copy of src/00548b50.cpp) ----
// Leaf: AdjustValueToProfile
// Grade: RELOCATION_MATCH 996/996 on the corrected span 0x00548B50+996 (code + embedded jump table; bounds audit) -- catalog row 1616 B held a second function at 0x00548F40; really NUISystem::CTextSlider::AdjustValueToProfile (catalog: CSlider)
// NUISystem::CTextSlider::AdjustValueToProfile() -- read the text slider's selected child from the user-profile setting its default action names; when it changed, flag a move, stamp the input time, light the accept button (observe the accept event, full alpha) and play the front-end text-slider sound
// Retail: Fable.exe (TLC, VC7.1) @ 0x00548B50, 682 bytes, plain `ret`; two jump tables (0x2B..0x2F; 0x54..0x143 via a byte index table).
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0 and the profile getters (each `UseNewProfileValues ? <x>NewProfile : <x>`;
//   manifest labels them IsCameraResetting / PeekConnectedToTrackNode): see the declarations; TutorialSystemEnabled and NewProfileToUseWASD
//   are read inline. CFrontEndManager::GetInstance 0x0041E5F2, its PlaySound (+0xa0) and GetDef (+0xa8, CDefPointer by value).
// Label correction: catalog class "CSlider" -- the layout (SelectedChild +0x15c, PreviouslySelectedChild +0x160, Move +0x168,
//   LastInputTime +0x180, PDefAction +0x188, PAcceptButton +0x198) is NUISystem::CTextSlider (landed Initialise 0x00549230).
// Model: the landed CSlider::AdjustValueToProfile 0x0054A6F0 (button / front-end manager / def pointer); profile field names from Ego_r.pdb.
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
    unsigned char _pad08[0x1a0 - 8];
    CSoundName    TextSliderSound;               // +0x1a0 (descriptive)
};
template <class T> struct CDefPointer {
    T* Data;
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
    unsigned char _pad0d[0x17 - 0xd];
    bool  TutorialSystemEnabled;                 // +0x17
    unsigned char _pad18[0x78 - 0x18];
    bool  NewProfileToUseWASD;                   // +0x78
    unsigned char _pad79[0xd7 - 0x79];
    bool  TutorialSystemEnabledNewProfile;       // +0xd7
    static CUserProfileManager* Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    bool IsCameraXYInverted() const;             // 0x00548590
    bool IsCameraYZInverted() const;             // 0x005485B0
    bool IsVibrationEnabled() const;             // 0x00548570
    bool IsUsingHeadPhones() const;              // 0x00548630
    bool IsSubtitlingEnabled() const;            // 0x0048DB35
    bool IsGuildMasterEnabled() const;           // 0x005485F0
    bool IsHUDTooltipEnabled() const;            // 0x00548610
    bool IsShowingBuddyNames() const;            // 0x00548650
    unsigned long GetResolution() const;         // 0x005486B0
    unsigned long GetRefreshRate() const;        // 0x005486D0
    unsigned long GetAntialiasing() const;       // 0x00548670
    unsigned long GetScreenAspectRatio() const;  // 0x005486F0
    bool IsVerticalSyncEnabled() const;          // 0x00548710
    bool IsCameraResetting() const;              // 0x00548690
    bool IsCameraPCBowYZInverted() const;        // 0x005485D0
    bool IsShowTargetingStatus() const;          // 0x00548740
    bool IsTutorialSystemEnabled() const { return UseNewProfileValues ? TutorialSystemEnabledNewProfile : TutorialSystemEnabled; }
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
namespace NUISystem {
struct CTextSlider {
    void* __vftable;
    unsigned char _pad04[0x30 - 4];
    float m_time;                                // +0x30 frame time (draggable base)
    unsigned char _pad34[0x15c - 0x34];
    unsigned long SelectedChild;                 // +0x15c
    unsigned long PreviouslySelectedChild;       // +0x160
    unsigned long StartSelectedChild;            // +0x164
    bool Move;                                   // +0x168
    unsigned char _pad169[0x180 - 0x169];
    float LastInputTime;                         // +0x180
    float InputDelay;                            // +0x184
    long PDefAction;                             // +0x188 EActionType
    CComponent* PLeftArrow;                      // +0x18c
    CComponent* PRightArrow;                     // +0x190
    unsigned long OriginalSelectedChild;         // +0x194
    CComponent* PAcceptButton;                   // +0x198
    void AdjustValueToProfile();
};

void CTextSlider::AdjustValueToProfile()
{
    PreviouslySelectedChild = SelectedChild;
    if (PDefAction != 0) {
        switch (PDefAction) {
        case 0x2b:  SelectedChild = CUserProfileManager::Get()->IsCameraXYInverted(); break;
        case 0x2c:  SelectedChild = CUserProfileManager::Get()->IsCameraYZInverted(); break;
        case 0x2d:  SelectedChild = CUserProfileManager::Get()->IsVibrationEnabled(); break;
        case 0x2e:  SelectedChild = CUserProfileManager::Get()->IsUsingHeadPhones(); break;
        case 0x2f:  SelectedChild = CUserProfileManager::Get()->IsSubtitlingEnabled(); break;
        case 0x53:  SelectedChild = CUserProfileManager::Get()->IsTutorialSystemEnabled(); break;
        case 0x5d:  SelectedChild = CUserProfileManager::Get()->IsGuildMasterEnabled(); break;
        case 0x5e:  SelectedChild = CUserProfileManager::Get()->IsHUDTooltipEnabled(); break;
        case 0x54:  SelectedChild = CUserProfileManager::Get()->IsShowingBuddyNames(); break;
        case 0x139: SelectedChild = CUserProfileManager::Get()->NewProfileToUseWASD != 0; break;
        case 0x102: SelectedChild = CUserProfileManager::Get()->GetResolution(); break;
        case 0x13d: SelectedChild = CUserProfileManager::Get()->GetRefreshRate(); break;
        case 0x109: SelectedChild = CUserProfileManager::Get()->GetAntialiasing(); break;
        case 0x12c: SelectedChild = CUserProfileManager::Get()->GetScreenAspectRatio(); break;
        case 0x106: SelectedChild = CUserProfileManager::Get()->IsVerticalSyncEnabled(); break;
        case 0x140: SelectedChild = CUserProfileManager::Get()->IsCameraResetting(); break;
        case 0x13f: SelectedChild = CUserProfileManager::Get()->IsCameraPCBowYZInverted(); break;
        case 0x143: SelectedChild = CUserProfileManager::Get()->IsShowTargetingStatus(); break;
        }
    }
    if (PreviouslySelectedChild != SelectedChild) {
        Move = true;
        LastInputTime = m_time;
        if (PAcceptButton && !PAcceptButton->IsObservingEvent(0x19)) {
            PAcceptButton->ObserveEvent(0x19);
            CRGBColour colour = PAcceptButton->GetColour();
            colour.A = 0xff;
            PAcceptButton->ChangeColour(colour, 0.2f, false);
        }
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetDef()->TextSliderSound, true);
    }
}
}

// ---- harness ----
static int g_fail;
static const char* g_last;
static bool g_ret;
static unsigned long g_retL;
static CUserProfileManager* g_profile;
static int g_sounds;
static long g_soundValue;
static bool g_soundFlag;
static int g_observeCalls, g_colourCalls;
static bool g_observing;
static CRGBColour g_gotColour;
static float g_gotTime;
static bool g_gotLinear;
static CFrontEndManagerDef g_def;
static int g_destroyed;
static CDefPointer<CFrontEndManagerDef>* g_holder;

CUserProfileManager* CUserProfileManager::Get() { return g_profile; }
bool CUserProfileManager::IsCameraXYInverted() const { g_last = "XY"; return g_ret; }
bool CUserProfileManager::IsCameraYZInverted() const { g_last = "YZ"; return g_ret; }
bool CUserProfileManager::IsVibrationEnabled() const { g_last = "VIB"; return g_ret; }
bool CUserProfileManager::IsUsingHeadPhones() const { g_last = "HP"; return g_ret; }
bool CUserProfileManager::IsSubtitlingEnabled() const { g_last = "SUB"; return g_ret; }
bool CUserProfileManager::IsGuildMasterEnabled() const { g_last = "GM"; return g_ret; }
bool CUserProfileManager::IsHUDTooltipEnabled() const { g_last = "HUD"; return g_ret; }
bool CUserProfileManager::IsShowingBuddyNames() const { g_last = "BUDDY"; return g_ret; }
unsigned long CUserProfileManager::GetResolution() const { g_last = "RES"; return g_retL; }
unsigned long CUserProfileManager::GetRefreshRate() const { g_last = "REF"; return g_retL; }
unsigned long CUserProfileManager::GetAntialiasing() const { g_last = "AA"; return g_retL; }
unsigned long CUserProfileManager::GetScreenAspectRatio() const { g_last = "ASP"; return g_retL; }
bool CUserProfileManager::IsVerticalSyncEnabled() const { g_last = "VSYNC"; return g_ret; }
bool CUserProfileManager::IsCameraResetting() const { g_last = "CAMRESET"; return g_ret; }
bool CUserProfileManager::IsCameraPCBowYZInverted() const { g_last = "BOW"; return g_ret; }
bool CUserProfileManager::IsShowTargetingStatus() const { g_last = "TARGET"; return g_ret; }

bool CObserver::IsObservingEvent(long) { return false; }
void CObserver::ObserveEvent(long) {}
CRGBColour CComponentPrimary_0054a990::GetColour() const { CRGBColour c; c.B = c.G = c.R = c.A = 0; return c; }
void CComponentPrimary_0054a990::ChangeColour(const CRGBColour&, float, bool) {}
void CDefBase::Destroy() { ++g_destroyed; }
void CFrontEndManager::AddObserver(CObserver*) {}
void CFrontEndManager::RemoveObserver(CObserver*) {}
void CFrontEndManager::ExecuteAction(const CCountedPointer<NUISystem::CAction>&) {}
long CFrontEndManager::GetLastInputResult() { return 0; }
void CFrontEndManager::PlaySound(const CSoundName&, bool) {}
CDefPointer<CFrontEndManagerDef> CFrontEndManager::GetDef() { return *g_holder; }

struct CTestButton : CComponent {
    bool IsObservingEvent(long eventType) { if (eventType != 0x19) g_fail = 1; return g_observing; }
    void ObserveEvent(long eventType) { if (eventType != 0x19) g_fail = 1; ++g_observeCalls; }
    CRGBColour GetColour() const { CRGBColour c; c.B = 0x11; c.G = 0x22; c.R = 0x33; c.A = 0x44; return c; }
    void ChangeColour(const CRGBColour& colour, float time, bool linear) { g_gotColour = colour; g_gotTime = time; g_gotLinear = linear; ++g_colourCalls; }
};
struct CTestFrontEnd : CFrontEndManager {
    void PlaySound(const CSoundName& sound, bool a) { ++g_sounds; g_soundValue = sound.Value; g_soundFlag = a; }
    CDefPointer<CFrontEndManagerDef> GetDef() { ++g_def.Ref.Count; return *g_holder; }   // AddRef, the caller's temporary releases
};
static CTestFrontEnd* g_frontEnd;
CFrontEndManager* __fastcall CFrontEndManager::GetInstance() { return g_frontEnd; }

static int g_soundsBefore;
static void adjust(NUISystem::CTextSlider& s) { g_soundsBefore = g_sounds; s.AdjustValueToProfile(); }
static void expectChange(NUISystem::CTextSlider& s, const char* what, unsigned long want, bool observed)
{
    int sounds = g_soundsBefore;
    if (s.SelectedChild != want || s.PreviouslySelectedChild == want) { printf("FAIL %s: child %lu want %lu\n", what, s.SelectedChild, want); g_fail = 1; }
    if (!s.Move || s.LastInputTime != s.m_time) { printf("FAIL %s: move/time\n", what); g_fail = 1; }
    if (g_sounds != sounds + 1 || g_soundValue != 0x5150 || !g_soundFlag) { printf("FAIL %s: sound %d/%d %lx %d\n", what, g_sounds, sounds, g_soundValue, (int)g_soundFlag); g_fail = 1; }
    (void)observed;
}

int main()
{
    static const struct { long action; const char* getter; int isLong; } kCases[] = {
        { 0x2b, "XY", 0 }, { 0x2c, "YZ", 0 }, { 0x2d, "VIB", 0 }, { 0x2e, "HP", 0 }, { 0x2f, "SUB", 0 },
        { 0x5d, "GM", 0 }, { 0x5e, "HUD", 0 }, { 0x54, "BUDDY", 0 }, { 0x102, "RES", 1 }, { 0x13d, "REF", 1 },
        { 0x109, "AA", 1 }, { 0x12c, "ASP", 1 }, { 0x106, "VSYNC", 0 }, { 0x140, "CAMRESET", 0 }, { 0x13f, "BOW", 0 },
        { 0x143, "TARGET", 0 } };
    static unsigned char profileStore[0x100];
    CTestButton button;
    CTestFrontEnd frontEnd;
    NUISystem::CTextSlider s;
    unsigned int i;
    unsigned long want;
    g_holder = new CDefPointer<CFrontEndManagerDef>;
    g_holder->Data = &g_def;
    g_def.Ref.Count = 1;
    g_def.TextSliderSound.Value = 0x5150;
    g_frontEnd = &frontEnd;
    g_profile = (CUserProfileManager*)profileStore;
    memset(profileStore, 0, sizeof(profileStore));
    memset(&s, 0, sizeof(s));
    s.m_time = 12.5f;
    s.PAcceptButton = &button;

    for (i = 0; i < sizeof(kCases) / sizeof(kCases[0]); ++i) {
        s.PDefAction = kCases[i].action;
        s.SelectedChild = 9; s.Move = false; s.LastInputTime = 0.0f;
        g_last = 0; g_ret = true; g_retL = 3 + i;
        want = kCases[i].isLong ? 3 + i : 1;
        adjust(s);
        if (!g_last || strcmp(g_last, kCases[i].getter) != 0) { printf("FAIL action 0x%lx called %s\n", kCases[i].action, g_last ? g_last : "nothing"); g_fail = 1; }
        expectChange(s, kCases[i].getter, want, false);
    }
    // the inline reads: tutorial flag through UseNewProfileValues, WASD straight from the profile
    s.PDefAction = 0x53; s.SelectedChild = 0;
    g_profile->UseNewProfileValues = true; g_profile->TutorialSystemEnabledNewProfile = true; g_profile->TutorialSystemEnabled = false;
    adjust(s);
    expectChange(s, "tutorial(new)", 1, false);
    g_profile->UseNewProfileValues = false;
    adjust(s);
    expectChange(s, "tutorial(old)", 0, false);
    s.PDefAction = 0x139; s.SelectedChild = 0; g_profile->NewProfileToUseWASD = true;
    adjust(s);
    expectChange(s, "wasd", 1, false);

    // accept button: first change observes + fades; while it already observes, no second fade
    if (g_observeCalls != 19 || g_colourCalls != 19) { printf("FAIL button calls %d %d\n", g_observeCalls, g_colourCalls); g_fail = 1; }
    if (g_gotColour.B != 0x11 || g_gotColour.G != 0x22 || g_gotColour.R != 0x33 || g_gotColour.A != 0xff || g_gotTime != 0.2f || g_gotLinear) {
        printf("FAIL fade args\n"); g_fail = 1;
    }
    g_observing = true;
    s.SelectedChild = 0;
    adjust(s);
    expectChange(s, "observing", 1, true);
    if (g_observeCalls != 19 || g_colourCalls != 19) { printf("FAIL observing button touched\n"); g_fail = 1; }
    s.PAcceptButton = 0;
    s.SelectedChild = 0;
    adjust(s);
    expectChange(s, "no button", 1, true);

    // no change: same value, unknown action, zero action -> nothing happens
    {
        int sounds = g_sounds;
        s.Move = false; s.LastInputTime = 0.0f;
        s.AdjustValueToProfile();                                   // WASD still 1, child already 1
        s.PDefAction = 0x30; s.SelectedChild = 4; s.AdjustValueToProfile();
        s.PDefAction = 0; s.AdjustValueToProfile();
        if (s.Move || s.LastInputTime != 0.0f || g_sounds != sounds || s.SelectedChild != 4 || s.PreviouslySelectedChild != 4) {
            printf("FAIL unchanged paths\n"); g_fail = 1;
        }
    }
    if (g_def.Ref.Count != 1 || g_destroyed != 0) { printf("FAIL def refcount %ld destroyed %d\n", g_def.Ref.Count, g_destroyed); g_fail = 1; }
    if (g_fail) return 1;
    printf("00548b50_TEST PASS\n");
    return 0;
}