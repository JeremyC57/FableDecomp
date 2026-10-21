// CMainGameComponent::PeekWorldChecksum() const -- 1 for a local game that is neither loading nor saving input, else the world's checksum
// Leaf: ?PeekWorldChecksum@CMainGameComponent@@QBEKXZ
// Retail: Fable.exe @ 0x00416392, 37 bytes; Ego_r.pdb same symbol. Tests Client.LocalGame (+0x1611a), then the input
// manager global (0x013B8388) Loading (+0x38) and Saving (+0x39); otherwise tail-jumps to CWorld::GetChecksum
// (0x0049E200) on PWorld (+0x24).
// Layout: Ego_r.pdb CMainGameComponent Client (CNetworkClient) +0x13ab8 with LocalGame +0x2662 -- retail agrees.
// Ego_r.pdb CInputManager Loading/Saving +0x44/+0x45; retail +0x38/+0x39 (PJoysticks is at +0x2c in retail, as the
// landed users of g_pInputManager read it, and is a 12-byte STLport vector). No engine/ headers for these classes.
// Opt: size TU (unaligned entry, `xor dl,dl` zero register for the byte compares, `xor eax,eax ; inc eax`).
#pragma optimize("s", on)
class CWorld {
public:
    unsigned long GetChecksum() const;                   // 0x0049E200
};
class CInputManager {
public:
    unsigned char _pad00[0x38];
    bool          Loading;                               // +0x38
    bool          Saving;                                // +0x39
};
extern CInputManager* g_pInputManager;                   // 0x013B8388
class CNetworkClient {
public:
    unsigned char _pad00[0x2662];
    bool          LocalGame;                             // +0x2662
};
class CMainGameComponent {
public:
    unsigned long PeekWorldChecksum() const;
    unsigned char  _pad00[0x24];
    CWorld*        PWorld;                               // +0x24 scoped_ptr<CWorld>
    unsigned char  _pad28[0x13ab8 - 0x28];
    CNetworkClient Client;                               // +0x13ab8
};
unsigned long CMainGameComponent::PeekWorldChecksum() const
{
    if (Client.LocalGame && !g_pInputManager->Loading && !g_pInputManager->Saving)
        return 1;
    return PWorld->GetChecksum();
}