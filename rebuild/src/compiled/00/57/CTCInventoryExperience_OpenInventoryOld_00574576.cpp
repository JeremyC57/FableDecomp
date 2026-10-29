// CTCInventoryExperience::OpenInventoryOld() -- opens the experience-spending screen the pre-PC way: unless the screen was opened from the hero-information screens, remembers the current hero-info screen's type and selects screen type 2 on the manager; then the base open, the spend values set up and reset, and the UI_EXP_SPENDING_SCREEN component created, initialised, shown and appended to the inventory's components
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00574576, length 182 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: NHeroInformationScreens::CBase virtual slot 3 on the secondary base at +0x130 (bool; descriptive name), CPlayerGui::
// GetPHeroInfoScreensManager() const @ 0x00437CD0 on g_pPlayerGui (0x013B8790), NHeroInformationScreens::CManager::GetCurrentScreen()
// @ 0x0066E7BD, that screen's virtual slot 2 (a long; stored at +0x220), the manager's +0x28 setter @ 0x0066E799 (manifest
// "CObservable::AddExclusiveObserver" is propagated: the body is `mov [ecx+28h],arg`), CTCInventoryBase::OpenInventory @ 0x005BD4EA,
// CTCInventoryExperience::InitialiseExperienceSpendValues @ 0x005724CD and ResetExperienceSpendValues @ 0x0056F03D, NUISystem::
// CManager::GetInstance() @ 0x0041E5F2 (`__fastcall`, no arguments), NUISystem::CManager::CreateComponent(const CCharString&,
// bool) @ 0x0041DB1D with the implicit conversion of "UI_EXP_SPENDING_SCREEN" (0x0124EA2C; CCharString ctor @ 0x0099EBF0, dtor
// @ 0x0099EAE0), the component's virtual slots 43 (+0xAC, no arguments) and 48 (+0xC0, the visual-state setter, 1),
// vector<NUISystem::CComponent*>::push_back(CComponent* const&) @ 0x00486024 on Components (+0x0C).
// Layout: retail CTCInventoryBase is 0x130 bytes (Components at +0x0C), the NHeroInformationScreens::CBase secondary base at
// +0x130, the remembered screen type at +0x220.
// Shape: the front-end manager is fetched before the string temporary (a named local, edi) and the created component is a
// named local that push_back takes by reference (its home store [ebp-8] lands right after CreateComponent).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
};
namespace NUISystem {
class CComponent { public:
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
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
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
class CBase { public:                                                    // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual long GetScreenType();                                 // slot 2
    virtual bool IsOpenedFromHeroInformationScreens();            // slot 3
};
class CManager { public:
    CBase* GetCurrentScreen();                                    // 0x0066E7BD
    void SetCurrentScreenType(long type);                         // 0x0066E799 (stores +0x28)
};
}
class CPlayerGui { public:
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
    NUISystem::CManager* frontEnd = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* screen = frontEnd->CreateComponent("UI_EXP_SPENDING_SCREEN", false);
    screen->Initialise();
    screen->SetState(1);
    Components.push_back(screen);
}