// CTCBoastUI::AllowUIToOpen(bool allow) -- record the permission; when forbidden, a pending activation is deactivated and remembered for later; when allowed again, a remembered activation is re-requested
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056A930, length 74 bytes (+ int3 padding).
// Callees: CTCBoastUI::Deactivate @ 0x0056A880; CTCBoastUI::RequestActivate (landed out of line
// @ 0x0056A860) is inlined: `if (!ActivationRequested && !Active && !Quitting) ActivationRequested = true`.
// Layout: engine/CTCBoastUI.h (Ego_r.pdb) with the retail 12-byte map and vector: Active +0x18,
// Quitting +0x28, AllowedToOpen +0x29, ActivationRequested +0x2a, ActivateLater +0x2b (PDB +0x1c,
// +0x30..+0x33), so the header offsets do not fit and the class is modelled locally (as the landed
// RequestActivate 0x0056A860).
// Shape: the store of the argument is scheduled between its test and the branch; the forbidden path
// is the `if` arm, the re-request the `else if`.
struct CGameCameraManager {
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
    void Deactivate();                                      // 0x0056A880
    void RequestActivate()                                  // 0x0056A860 (inlined here)
    {
        if (!ActivationRequested && !Active && !Quitting)
            ActivationRequested = true;
    }
    void AllowUIToOpen(bool allow);
};

void CTCBoastUI_Retail::AllowUIToOpen(bool allow)
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
