// CNewFrontendGameComponent::InitialiseInput() -- cache the system manager's input manager in m_pInputManager
// Retail: Fable.exe @ 0x0042DEC5, 16 bytes. GFGetSystemManager (0x009A4EC0) returns the CSystemManager singleton
// (0x013CA618); its CCountedPointer<CInputManager> PInputManager sits at +0x58 (Ego_r.pdb), and the raw pointer is
// stored in CNewFrontendGameComponent::m_pInputManager (+0x68). The call clobbers ecx, hence `this` in esi.
// No engine headers exist for either class: local models sized to the PDB offsets.
class CInputManager;
template <class T> class CCountedPointer { public:
    T*   Data;                      // +0x00
    long* RefCount;                 // +0x04
};
class CSystemManager { public:
    unsigned char _pad00[0x58];
    CCountedPointer<CInputManager> PInputManager;   // +0x58
};
CSystemManager* __fastcall GFGetSystemManager();     // 0x009A4EC0
class CNewFrontendGameComponent {
public:
    void InitialiseInput();
    unsigned char _pad00[0x68];
    CInputManager* m_pInputManager;                  // +0x68
};
void CNewFrontendGameComponent::InitialiseInput()
{
    m_pInputManager = GFGetSystemManager()->PInputManager.Data;
}
