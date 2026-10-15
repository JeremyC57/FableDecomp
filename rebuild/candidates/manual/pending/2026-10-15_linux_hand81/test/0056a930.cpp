// Behaviour test for 0x0056A930 CTCBoastUI::AllowUIToOpen: forbidding deactivates a pending request and
// remembers it; allowing again re-requests it unless the UI is active or quitting, and clears the memory.
#include <cstdio>
static int g_cameraCalls, g_closeCalls;
struct CGameCameraManager { void SetAutomaticCameraControl() { ++g_cameraCalls; } };
struct CGameWorldCalls { CGameCameraManager* m_pCameraManager; };
struct CEngineCalls { CGameWorldCalls* m_pWorld; };
static CGameCameraManager g_camera;
static CGameWorldCalls g_worldObj;
static CEngineCalls g_engineObj;
static CEngineCalls* g_pEngine = &g_engineObj;
struct CPlayerGuiCalls { void Close() { ++g_closeCalls; } };
static CPlayerGuiCalls g_gui;
static CPlayerGuiCalls* g_pPlayerGui = &g_gui;

struct CTCBoastUI {
    bool Active, Quitting, AllowedToOpen, ActivationRequested, ActivateLater;
    int  m_deactivateCalls;
    void Deactivate()
    {
        ++m_deactivateCalls;
        if (ActivationRequested) {
            if (Active) {
                g_pEngine->m_pWorld->m_pCameraManager->SetAutomaticCameraControl();
                Quitting = true;
                g_pPlayerGui->Close();
            } else {
                ActivationRequested = false;
            }
        }
    }
    void RequestActivate()
    {
        if (!ActivationRequested && !Active && !Quitting)
            ActivationRequested = true;
    }
    void AllowUIToOpen(bool allow)
    {
        AllowedToOpen = allow;
        if (!allow) {
            if (ActivationRequested) {
                Deactivate();
                ActivateLater = true;
            }
        } else if (ActivateLater) {
            RequestActivate();
            ActivateLater = false;
        }
    }
};
static void Reset(CTCBoastUI& u, bool active, bool quitting, bool requested, bool later)
{
    u.Active = active; u.Quitting = quitting; u.AllowedToOpen = false; u.ActivationRequested = requested; u.ActivateLater = later;
    u.m_deactivateCalls = 0; g_cameraCalls = 0; g_closeCalls = 0;
    g_worldObj.m_pCameraManager = &g_camera; g_engineObj.m_pWorld = &g_worldObj;
}

int main()
{
    CTCBoastUI u;
    Reset(u, false, false, true, false);
    u.AllowUIToOpen(false);
    if (u.AllowedToOpen || u.m_deactivateCalls != 1 || !u.ActivateLater || u.ActivationRequested) { std::printf("0056a930_TEST FAIL\n"); return 1; }
    u.AllowUIToOpen(true);
    if (!u.AllowedToOpen || u.ActivateLater || !u.ActivationRequested) { std::printf("0056a930_TEST FAIL\n"); return 1; }
    Reset(u, false, false, false, false);
    u.AllowUIToOpen(false);
    if (u.m_deactivateCalls != 0 || u.ActivateLater) { std::printf("0056a930_TEST FAIL\n"); return 1; }
    Reset(u, false, true, false, true);
    u.AllowUIToOpen(true);
    if (u.ActivationRequested || u.ActivateLater) { std::printf("0056a930_TEST FAIL\n"); return 1; }
    Reset(u, false, false, false, false);
    u.AllowUIToOpen(true);
    if (u.ActivationRequested || u.ActivateLater || !u.AllowedToOpen) { std::printf("0056a930_TEST FAIL\n"); return 1; }
    std::printf("0056a930_TEST PASS\n");
    return 0;
}
