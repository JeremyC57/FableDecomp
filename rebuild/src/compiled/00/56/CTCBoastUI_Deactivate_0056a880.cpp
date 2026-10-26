// CTCBoastUI::Deactivate() -- a pending activation is cancelled; when the UI is already active it hands the camera back to automatic control, marks Quitting and closes the player GUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056A880, length 55 bytes (+ int3 padding).
// Callees: CGameCameraManager::SetAutomaticCameraControl @ 0x00699090 (no arguments), CPlayerGui::Close
// @ 0x004374B0 (tail jump; manifest "CPlayerGui::Open" is propagated, the landed 0x005BD597 names it Close).
// Globals: the engine singleton @ 0x013B86A0 (+0x24 -> +0x34 = the camera manager) and the player GUI
// @ 0x013B8790.
// Layout: engine/CTCBoastUI.h (Ego_r.pdb) with the retail 12-byte map and vector: Active +0x18,
// Quitting +0x28, AllowedToOpen +0x29, ActivationRequested +0x2a, ActivateLater +0x2b (PDB +0x1c,
// +0x30..+0x33), so the header offsets do not fit and the class is modelled locally (as the landed
// RequestActivate 0x0056A860).
// Shape: nested ifs; the inner else clears ActivationRequested; the Close call is the last statement
// so it becomes the sibling-call jmp with the global loaded into ecx.
class CGameCameraManager { public:
    void SetAutomaticCameraControl();                       // 0x00699090
};

struct CGameWorldCalls {                                    // reached through the engine singleton at +0x24
    unsigned char       _pad00[0x34];
    CGameCameraManager* m_pCameraManager;                   // +0x34
};

struct CEngineCalls {                                       // the engine singleton behind 0x013B86A0
    unsigned char    _pad00[0x24];
    CGameWorldCalls* m_pWorld;                              // +0x24
};
extern CEngineCalls* g_pEngine;                             // 0x013B86A0

struct CPlayerGuiCalls {
    void Close();                                           // 0x004374B0
};
extern CPlayerGuiCalls* g_pPlayerGui;                       // 0x013B8790

struct CTCBoastUI_Retail {
    void*         __vftable;                                // +0x00
    unsigned char _pad04[0x18 - 0x04];                      // CTCBase, BoastGroups (12-byte map)
    bool          Active;                                   // +0x18 (PDB +0x1c)
    unsigned char _pad19[0x28 - 0x19];                      // Components (12-byte vector)
    bool          Quitting;                                 // +0x28 (PDB +0x30)
    bool          AllowedToOpen;                            // +0x29 (PDB +0x31)
    bool          ActivationRequested;                      // +0x2a (PDB +0x32)
    bool          ActivateLater;                            // +0x2b (PDB +0x33)
    void Deactivate();
};

void CTCBoastUI_Retail::Deactivate()
{
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