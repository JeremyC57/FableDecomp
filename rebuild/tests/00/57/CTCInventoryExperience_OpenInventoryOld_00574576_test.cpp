// Behaviour test for 00574576: src/00574576.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryExperience::OpenInventoryOld()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryExperience::OpenInventoryOld() -- opens the experience-spending screen the pre-PC way: unless the screen was opened from the hero-information screens, remembers the current hero-info screen's type and selects screen type 2 on the manager; then the base open, the spend values set up and reset, and the UI_EXP_SPENDING_SCREEN component created, initialised, shown and appended to the inventory's components
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00574576, length 182 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: NHeroInformationScreens::CBase virtual slot 3 on the secondary base at +0x130 (bool; descriptive name), CPlayerGui::
// GetPHeroInfoScreensManager() const @ 0x00437CD0 on g_pPlayerGui (0x013B8790), NHeroInformationScreens::CManager::GetCurrentScreen()
// @ 0x0066E7BD, that screen's virtual slot 2 (a long; stored at +0x220), the manager's +0x28 setter @ 0x0066E799 (manifest
// "CObservable::AddExclusiveObserver" is propagated: the body is `mov [ecx+28h],arg`), CTCInventoryBase::OpenInventory @ 0x005BD4EA,
// CTCInventoryExperience::InitialiseExperienceSpendValues @ 0x005724CD and ResetExperienceSpendValues @ 0x0056F03D, NUISystem::
// CFrontEndManager::GetInstance() @ 0x0041E5F2 (`__fastcall`, no arguments), NUISystem::CManager::CreateComponent(const CCharString&,
// bool) @ 0x0041DB1D with the implicit conversion of "UI_EXP_SPENDING_SCREEN" (0x0124EA2C; CCharString ctor @ 0x0099EBF0, dtor
// @ 0x0099EAE0), the component's virtual slots 43 (+0xAC, no arguments) and 48 (+0xC0, the visual-state setter, 1),
// vector<NUISystem::CComponent*>::push_back(CComponent* const&) @ 0x00486024 on Components (+0x0C).
// Layout: retail CTCInventoryBase is 0x130 bytes (Components at +0x0C), the NHeroInformationScreens::CBase secondary base at
// +0x130, the remembered screen type at +0x220.
// Shape: the front-end manager is fetched before the string temporary (a named local, edi) and the created component is a
// named local that push_back takes by reference (its home store [ebp-8] lands right after CreateComponent).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
};
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                                    // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetState(long state);                            // slot 48 (+0xC0)
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();            // 0x0041E5F2
};
}
struct CComponentVector {                                         // retail std::vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** m_first;
    NUISystem::CComponent** m_last;
    NUISystem::CComponent** m_end;
    void push_back(NUISystem::CComponent* const& value);          // 0x00486024
};
// Same-TU stand-in for vector::push_back 0x00486024: reads the value through its const reference (never writes it), so
// VC7.1 knows the component local's address goes no further.
#pragma auto_inline(off)
__declspec(noinline) void CComponentVector::push_back(NUISystem::CComponent* const& value)
{
    if (m_last != m_end)
        *m_last++ = value;
}
#pragma auto_inline(on)
namespace NHeroInformationScreens {
struct CBase {                                                    // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual long GetScreenType();                                 // slot 2
    virtual bool IsOpenedFromHeroInformationScreens();            // slot 3
};
struct CManager {
    CBase* GetCurrentScreen();                                    // 0x0066E7BD
    void SetCurrentScreenType(long type);                         // 0x0066E799 (stores +0x28)
};
}
struct CPlayerGui {
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                                  // 0x013B8790

struct CTCInventoryBase_Retail {                                  // retail 0x130 bytes (the polymorphic primary base)
    virtual void v00();                                           // +0x00 vptr
    void*            ParentThing;                                 // +0x04
    unsigned char    _pad08[4];
    CComponentVector Components;                                  // +0x0C
    unsigned char    _pad18[0x130 - 0x18];
    void OpenInventory();                                         // 0x005BD4EA
};
class CTCInventoryExperience : public CTCInventoryBase_Retail, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad_0x134[0x220 - 0x134];
    long          PreviousHeroInfoScreenType;                     // +0x220
    void InitialiseExperienceSpendValues();                       // 0x005724CD
    void ResetExperienceSpendValues();                            // 0x0056F03D
    void OpenInventoryOld();
};
void CTCInventoryExperience::OpenInventoryOld()
{
    if (!IsOpenedFromHeroInformationScreens()) {
        PreviousHeroInfoScreenType = g_pPlayerGui->GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        g_pPlayerGui->GetPHeroInfoScreensManager()->SetCurrentScreenType(2);
    }
    CTCInventoryBase_Retail::OpenInventory();
    InitialiseExperienceSpendValues();
    ResetExperienceSpendValues();
    NUISystem::CFrontEndManager* frontEnd = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CComponent* screen = frontEnd->CreateComponent("UI_EXP_SPENDING_SCREEN", false);
    screen->Initialise();
    screen->SetState(1);
    Components.push_back(screen);
}

// ---- stubs and driver ----
void NUISystem::CComponent::v00() {}
void NUISystem::CComponent::v01() {}
void NUISystem::CComponent::v02() {}
void NUISystem::CComponent::v03() {}
void NUISystem::CComponent::v04() {}
void NUISystem::CComponent::v05() {}
void NUISystem::CComponent::v06() {}
void NUISystem::CComponent::v07() {}
void NUISystem::CComponent::v08() {}
void NUISystem::CComponent::v09() {}
void NUISystem::CComponent::v10() {}
void NUISystem::CComponent::v11() {}
void NUISystem::CComponent::v12() {}
void NUISystem::CComponent::v13() {}
void NUISystem::CComponent::v14() {}
void NUISystem::CComponent::v15() {}
void NUISystem::CComponent::v16() {}
void NUISystem::CComponent::v17() {}
void NUISystem::CComponent::v18() {}
void NUISystem::CComponent::v19() {}
void NUISystem::CComponent::v20() {}
void NUISystem::CComponent::v21() {}
void NUISystem::CComponent::v22() {}
void NUISystem::CComponent::v23() {}
void NUISystem::CComponent::v24() {}
void NUISystem::CComponent::v25() {}
void NUISystem::CComponent::v26() {}
void NUISystem::CComponent::v27() {}
void NUISystem::CComponent::v28() {}
void NUISystem::CComponent::v29() {}
void NUISystem::CComponent::v30() {}
void NUISystem::CComponent::v31() {}
void NUISystem::CComponent::v32() {}
void NUISystem::CComponent::v33() {}
void NUISystem::CComponent::v34() {}
void NUISystem::CComponent::v35() {}
void NUISystem::CComponent::v36() {}
void NUISystem::CComponent::v37() {}
void NUISystem::CComponent::v38() {}
void NUISystem::CComponent::v39() {}
void NUISystem::CComponent::v40() {}
void NUISystem::CComponent::v41() {}
void NUISystem::CComponent::v42() {}
void NUISystem::CComponent::v44() {}
void NUISystem::CComponent::v45() {}
void NUISystem::CComponent::v46() {}
void NUISystem::CComponent::v47() {}
void NHeroInformationScreens::CBase::v00() {}
void NHeroInformationScreens::CBase::v01() {}
void CTCInventoryBase_Retail::v00() {}

static int g_seq = 0, g_open = 0, g_initSpend = 0, g_resetSpend = 0, g_compInit = 0, g_setStateSeq = 0; static long g_state = -1;
static char g_def[64]; static bool g_defFlag = true; static long g_setType = -1; static bool g_fromHeroInfo = false;
CCharString::CCharString(const char* text, long) { m_pStringData = (void*)text; }
CCharString::~CCharString() {}
void NUISystem::CComponent::Initialise() { g_compInit = ++g_seq; }
void NUISystem::CComponent::SetState(long state) { g_state = state; g_setStateSeq = ++g_seq; }
static NUISystem::CComponent g_screen;
NUISystem::CComponent* NUISystem::CManager::CreateComponent(const CCharString& definition, bool b) { strcpy(g_def, (const char*)definition.m_pStringData); g_defFlag = b; return &g_screen; }
static NUISystem::CFrontEndManager g_fem;
NUISystem::CFrontEndManager* __fastcall NUISystem::CFrontEndManager::GetInstance() { return &g_fem; }
struct CScreen7 : NHeroInformationScreens::CBase { long GetScreenType() { return 7; } bool IsOpenedFromHeroInformationScreens() { return false; } };
static CScreen7 g_current;
long NHeroInformationScreens::CBase::GetScreenType() { return 0; }
bool NHeroInformationScreens::CBase::IsOpenedFromHeroInformationScreens() { return false; }
NHeroInformationScreens::CBase* NHeroInformationScreens::CManager::GetCurrentScreen() { return &g_current; }
void NHeroInformationScreens::CManager::SetCurrentScreenType(long type) { g_setType = type; }
static NHeroInformationScreens::CManager g_mgr;
NHeroInformationScreens::CManager* CPlayerGui::GetPHeroInfoScreensManager() const { return &g_mgr; }
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
void CTCInventoryBase_Retail::OpenInventory() { g_open = ++g_seq; }
void CTCInventoryExperience::InitialiseExperienceSpendValues() { g_initSpend = ++g_seq; }
void CTCInventoryExperience::ResetExperienceSpendValues() { g_resetSpend = ++g_seq; }
struct CExpTest : CTCInventoryExperience { bool IsOpenedFromHeroInformationScreens() { return g_fromHeroInfo; } };
int main()
{
    static CExpTest e; NUISystem::CComponent* slots[4]; memset(slots, 0, sizeof(slots));
    e.Components.m_first = slots; e.Components.m_last = slots; e.Components.m_end = slots + 4; e.PreviousHeroInfoScreenType = -1;
    e.OpenInventoryOld();
    CHECK(e.PreviousHeroInfoScreenType == 7); CHECK(g_setType == 2);
    CHECK(g_open == 1 && g_initSpend == 2 && g_resetSpend == 3 && g_compInit == 4 && g_setStateSeq == 5); CHECK(g_state == 1);
    CHECK(strcmp(g_def, "UI_EXP_SPENDING_SCREEN") == 0); CHECK(!g_defFlag);
    CHECK(e.Components.m_last == slots + 1 && slots[0] == &g_screen);
    g_fromHeroInfo = true; g_setType = -1; e.PreviousHeroInfoScreenType = 3; e.OpenInventoryOld();
    CHECK(e.PreviousHeroInfoScreenType == 3); CHECK(g_setType == -1); CHECK(e.Components.m_last == slots + 2);
    if (fails == 0) printf("00574576_TEST PASS\n"); else printf("00574576_TEST FAIL\n");
    return fails;
}