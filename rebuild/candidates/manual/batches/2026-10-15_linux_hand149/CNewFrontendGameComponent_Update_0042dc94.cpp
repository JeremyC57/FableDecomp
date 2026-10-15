// CNewFrontendGameComponent::Update() -- advance the front end by the elapsed time (clamped to 0.1s): while a controller is disconnected show that error, otherwise clear any error being shown, then update the front-end manager and remember the time
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DC94, 148 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: GFGetTime @ 0x009E1BC0 (`__fastcall`, double), IsControllerDisconnected @ 0x0042DC63, the front-end manager's
//   (+0xb4) bool query @ 0x00595A03 (label "CThingFilter_IsUnconscious::operator()" is propagated) and Update(float) @ 0x00599E3F,
//   NUISystem::CManager::GetInstance @ 0x0041E5F2 (`__fastcall`; the error-shown byte at +0x9c), the static `__fastcall`
//   ShowErrorMessage(ERRORMESSAGE_TYPE) forwarder @ 0x0041E61B (called with 0), CManager::ClearErrorMessage @ 0x0041CE6A.
//   0.1f @ 0x0122EE58.
// Layout (retail): front-end manager pointer +0xb4, last update time (double) +0xc0.
// Shape: the elapsed time is a float local (`fst [ebp-4]`, compared on the FPU) clamped by `if (dt > 0.1f)`; the manager is
//   fetched again for every use; the float argument is pushed through `push ecx ; fld ; fstp [esp]` (size TU).
#pragma optimize("y", off)
#pragma optimize("s", on)
double __fastcall GFGetTime();                               // 0x009E1BC0
namespace NUISystem {
enum ERRORMESSAGE_TYPE { ERRORMESSAGE_CONTROLLER_DISCONNECTED = 0 };
struct CManager {
    unsigned char _pad00[0x9c];
    bool          m_errorMessageShowing;                     // +0x9c
    static CManager* __fastcall GetInstance();               // 0x0041E5F2
    static void __fastcall ShowErrorMessage(ERRORMESSAGE_TYPE type);   // 0x0041E61B (GetInstance()->ShowErrorMessage)
    void ClearErrorMessage();                                // 0x0041CE6A
};
}
struct CFrontEndManager {
    bool IsBlockingControllerMessage();                      // 0x00595A03
    void Update(float dt);                                   // 0x00599E3F
};
struct CNewFrontendGameComponent {
    void*             __vftable;                             // +0x00
    unsigned char     _pad04[0xb0];
    CFrontEndManager* m_pFrontEndManager;                    // +0xb4
    unsigned char     _padB8[0x08];
    double            m_lastUpdateTime;                      // +0xc0
    bool IsControllerDisconnected();                         // 0x0042DC63
    void Update();
};

void CNewFrontendGameComponent::Update()
{
    float dt = (float)(GFGetTime() - m_lastUpdateTime);
    if (dt > 0.1f)
        dt = 0.1f;
    if (IsControllerDisconnected() && !m_pFrontEndManager->IsBlockingControllerMessage()) {
        if (!NUISystem::CManager::GetInstance()->m_errorMessageShowing)
            NUISystem::CManager::ShowErrorMessage(NUISystem::ERRORMESSAGE_CONTROLLER_DISCONNECTED);
    } else if (NUISystem::CManager::GetInstance()->m_errorMessageShowing) {
        NUISystem::CManager::GetInstance()->ClearErrorMessage();
    }
    m_pFrontEndManager->Update(dt);
    m_lastUpdateTime = GFGetTime();
}
