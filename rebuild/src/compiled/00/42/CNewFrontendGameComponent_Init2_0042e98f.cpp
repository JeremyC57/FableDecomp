// CNewFrontendGameComponent::Init2() -- second-stage front-end start-up: create the front-end manager, graphics/sounds/input, load the localised text bank, initialise the UI and set the 1024x768 display mode
// Leaf: ?Init2@CNewFrontendGameComponent@@QAEXXZ (Ego_r.pdb AAEXXZ @ 0x0043B547, private; modelled public)
// Retail: Fable.exe @ 0x0042E98F, 256 bytes, plain `ret` after `leave`.
// Body: m_pFrontEndManager = CFrontEndManager::GetInstance() (0x00595582, static fastcall, creates the 0xe0-byte
// singleton); its PGameComponent (+0x1c) = this; its +0xc0 flag = !g_byte_01375448; InitialiseGraphics() (0x0042DDB3);
// InitialiseSounds() (0x0042E17C); InitialiseInput() (0x0042DEC5); a fastcall global setter (0x00AEAA80, stores ecx to
// 0x013D2E08) gets m_pFrontEndGraphicBank's object; m_pTextBank.Reset(new NGameText::CDataBank) (0x214 bytes, ctor
// 0x009C85A0, Reset 0x00403E40); m_pTextBank->OpenReadOnly(NLocalisation::GetTextBankName(), 0x100) (slot 1, +4;
// GetTextBankName 0x00415260 is __fastcall with the hidden result in ecx; ~CCharString 0x0099EAE0);
// NUISystem::CManager::Clear() (0x0042FB40); m_pFrontEndManager->Init(m_pFrontEndGraphicBank, m_pMeshBank,
// m_pInputManager, GetDefinitionManager()) (0x005958F5; by-value CCountedPointer copies 0x0041BA30 / 0x00428905; the
// getter 0x0043368D returns the global 0x013B8760); m_pFrontEndManager->BuildComponents(!WindowsMediaPlayerInstalled)
// (0x00598A1C); g_pDisplayManager (0x013B8390)->SetDisplayMode(C2DExtentsI(1024, 768), 16, 0, 60) (0x009BFF40).
// Names: PDB symbols except the 0x00AEAA80 setter (labelled SetCurrentThingManager by body folding; it publishes the
// front-end graphic bank), the 0x01375448 flag and the CFrontEndManager +0xc0 flag (m_bQuitFromGame in the matched
// 0x005953E2 layout); OpenReadOnly is the CBankFile slot-1 name (Ego_r.pdb ?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z).
// Layout: retail CNewFrontendGameComponent (Ego_r.pdb, 8 lower after the two STLport vectors at +0x6c):
// m_pFrontEndGraphicBank +0x40, m_pMeshBank +0x48, m_pTextBank +0x60, m_pInputManager +0x68, m_pFrontEndManager +0xb4
// (Ego_r +0xbc), WindowsMediaPlayerInstalled +0x144 (Ego_r +0x14c). CFrontEndManager PGameComponent +0x1c.
// Opt: size TU (unaligned entry) with a frame pointer (`leave`).
// Layout (supersedes any local-model note above): engine/CFrontEndManager.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CFrontEndManager CFrontEndManagerDonor
#include "engine/CFrontEndManager.h"
#undef CFrontEndManager
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
namespace NLocalisation {
CCharString __fastcall GetTextBankName();                // 0x00415260
}
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Object;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other);       // 0x0041BA30 (CGraphicDataBank) / 0x00428905 (CMeshDataBank)
    void Reset(T* object);                               // 0x00403E40 for NGameText::CDataBank
    T* operator->() const { return Object; }
};
class CGraphicDataBank;
class CMeshDataBank;
class CInputManager;
class CDefinitionManager;
CDefinitionManager* GetDefinitionManager();              // 0x0043368D
void __fastcall SetCurrentGraphicBank(CGraphicDataBank* bank);   // 0x00AEAA80
namespace NGameText {
class CDataBank {
public:
    CDataBank();                                         // 0x009C85A0
    virtual ~CDataBank();
    virtual bool OpenReadOnly(const CCharString& name, unsigned long flags);   // slot 1 (+4)
    unsigned char _pad004[0x214 - 4];
};
}
class C2DExtentsI {
public:
    long X;
    long Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };
class CDisplayManager {
public:
    enum ECreateDeviceSuccess { CREATE_DEVICE_0 };
    ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI& size, unsigned long bitDepth, ESurfaceMultisampleType multisample,
                                        unsigned long refreshRate);   // 0x009BFF40
};
extern CDisplayManager* g_pDisplayManager;               // 0x013B8390
extern bool g_bFrontEndFlag_01375448;                    // 0x01375448 (name not recovered)
class CNewFrontendGameComponent;
namespace NUISystem {
class CManager {
public:
    static void __fastcall Clear();                      // 0x0042FB40
};
class CFrontEndManager : public CFrontEndManagerDonor {
public:
    static CFrontEndManager* __fastcall GetInstance_00595582();   // 0x00595582
    void Init(CCountedPointer<CGraphicDataBank> graphics, CCountedPointer<CMeshDataBank> meshes, CInputManager* input,
              CDefinitionManager* defs);                 // 0x005958F5
    void BuildComponents(bool flag);                     // 0x00598A1C
};
}
class CNewFrontendGameComponent {
public:
    virtual ~CNewFrontendGameComponent();
    unsigned char                         _pad004[0x40 - 4];
    CCountedPointer<CGraphicDataBank>     m_pFrontEndGraphicBank;   // +0x40
    CCountedPointer<CMeshDataBank>        m_pMeshBank;              // +0x48
    unsigned char                         _pad050[0x60 - 0x50];
    CCountedPointer<NGameText::CDataBank> m_pTextBank;              // +0x60
    CInputManager*                        m_pInputManager;          // +0x68
    unsigned char                         _pad06c[0xb4 - 0x6c];
    NUISystem::CFrontEndManager*          m_pFrontEndManager;       // +0xb4
    unsigned char                         _pad0b8[0x144 - 0xb8];
    bool                                  WindowsMediaPlayerInstalled;   // +0x144
    bool InitialiseGraphics();                           // 0x0042DDB3
    void InitialiseSounds();                             // 0x0042E17C
    void InitialiseInput();                              // 0x0042DEC5
    void Init2();
};
void CNewFrontendGameComponent::Init2()
{
    m_pFrontEndManager = NUISystem::CFrontEndManager::GetInstance_00595582();
    m_pFrontEndManager->PGameComponent = this;
    m_pFrontEndManager->m_bQuitFromGame = !g_bFrontEndFlag_01375448;
    InitialiseGraphics();
    InitialiseSounds();
    InitialiseInput();
    SetCurrentGraphicBank(m_pFrontEndGraphicBank.Object);
    m_pTextBank.Reset(new NGameText::CDataBank);
    m_pTextBank->OpenReadOnly(NLocalisation::GetTextBankName(), 0x100);
    NUISystem::CManager::Clear();
    m_pFrontEndManager->Init(m_pFrontEndGraphicBank, m_pMeshBank, m_pInputManager, GetDefinitionManager());
    m_pFrontEndManager->BuildComponents(!WindowsMediaPlayerInstalled);
    g_pDisplayManager->SetDisplayMode(C2DExtentsI(1024, 768), 16, SURFACE_MULTISAMPLE_NONE, 60);
}