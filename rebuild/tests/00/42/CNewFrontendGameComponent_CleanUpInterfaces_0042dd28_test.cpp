// Behaviour test for 0042dd28: src/0042dd28.cpp copied verbatim; the counted pointers, bank managers and the front-end
// manager stubbed so each release step (delete + null, counted-pointer destruction + reset, the two bank closes with
// their by-value copies) is observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// Retry 10b: the mesh bank manager comes from an inline getter (`GFGetSystemManager()->GetMeshBankManager()`, B-4/B-5 accessor idiom): retail keeps the shared zero in ebp and the manager in ebx (a plain member read swaps them).
// CNewFrontendGameComponent::CleanUpInterfaces() -- tears down the front end's engine-side objects: deletes the front-end manager, drops the engine, closes and drops the front-end graphic bank and mesh bank, and drops the menu font
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042DD28, length 139 bytes.
// Label correction: the catalog calls this CEngineResourceManager::ChangeScene (propagated; that method is the landed
// 0x00B3B4A0). This body runs on a CNewFrontendGameComponent: its callers are CNewFrontendGameComponent::Shutdown
// (0x0042EBE6) and 0x0042F247, and every offset is a member of that class (PDB, retail vectors 4 bytes narrower):
// m_pMenuFont +0x10, m_pFrontEndGraphicBank +0x40, m_pMeshBank +0x48, m_pEngine +0x58, m_pFrontEndManager +0xb4
// (PDB +0xbc). The PDB name is the private `CleanUpInterfaces()` by elimination (the other private no-argument
// methods have their own roles); treat the name as probable.
// Callees: the front-end manager's virtual slot 0 (scalar deleting destructor, flag 1); the out-of-line counted-pointer
// destructors 0x0042F974 (CIEngine), 0x00419108 (CGraphicDataBank, landed as such by 0x0042A9B7), 0x0042918C
// (CMeshDataBank), 0x0042F948 (CFontBank); the out-of-line copy constructors 0x0041BA30 / 0x00428905 (the by-value
// arguments); 0x009F8290 CGraphicBankManager::CloseGraphicBank(CCountedPointer<CGraphicDataBank>) on the global
// @ 0x013B837C; 0x009A4EC0 GFGetSystemManager() and 0x00A09DC0 its mesh bank manager's (+0x74) close call.
// Shape: the safe-release idiom per member: `if (p) { delete p; p = 0; }` for the raw manager pointer, and for each
// counted pointer an explicit destructor call followed by a zero store of its object word (the destructor already
// clears both words; the store is the source's own reset); the mesh bank manager is fetched into a local before the
// argument copy; one zero register (ebp) serves every test and store.

#pragma optimize("s", on)

class CFrontEndManager {
public:
    virtual ~CFrontEndManager();                         // slot 0
};
class CCPPointerInfo;
template<class T> class CCountedPointer {
public:
    T*              Data;                                // +0
    CCPPointerInfo* Info;                                // +4
    CCountedPointer(const CCountedPointer& other);       // out of line (0x0041BA30 / 0x00428905)
    ~CCountedPointer();                                  // out of line (0x0042F974 / 0x00419108 / 0x0042918C / 0x0042F948)
};
class CIEngine;
class CGraphicDataBank;
class CMeshDataBank;
class CFontBank;
class CASoundBank;

class CGraphicBankManager {
public:
    void CloseGraphicBank(CCountedPointer<CGraphicDataBank> bank);   // 0x009F8290
};
extern CGraphicBankManager* g_pGraphicBankManager;       // 0x013B837C
class CMeshBankManager {
public:
    void CloseMeshBank(CCountedPointer<CMeshDataBank> bank);         // 0x00A09DC0
};
class CSystemManager { public:
    unsigned char     _pad[0x74];
    CMeshBankManager* MeshBankManager;                   // +0x74
    CMeshBankManager* GetMeshBankManager() const { return MeshBankManager; }
};
CSystemManager* __cdecl GFGetSystemManager();            // 0x009A4EC0

class CNewFrontendGameComponent {
public:
    unsigned char                     _pad00[0x10];
    CCountedPointer<const CFontBank>  m_pMenuFont;       // +0x10
    unsigned char                     _pad18[0x28];
    CCountedPointer<CGraphicDataBank> m_pFrontEndGraphicBank;   // +0x40
    CCountedPointer<CMeshDataBank>    m_pMeshBank;       // +0x48
    CCountedPointer<CASoundBank>      PSampleBank;       // +0x50
    CCountedPointer<CIEngine>         m_pEngine;         // +0x58
    unsigned char                     _pad60[0x54];
    CFrontEndManager*                 m_pFrontEndManager;   // +0xb4
    void CleanUpInterfaces();
};

void CNewFrontendGameComponent::CleanUpInterfaces()
{
    if (m_pFrontEndManager) {
        delete m_pFrontEndManager;
        m_pFrontEndManager = 0;
    }
    if (m_pEngine.Data) {
        m_pEngine.~CCountedPointer<CIEngine>();
        m_pEngine.Data = 0;
    }
    if (m_pFrontEndGraphicBank.Data) {
        g_pGraphicBankManager->CloseGraphicBank(m_pFrontEndGraphicBank);
        m_pFrontEndGraphicBank.~CCountedPointer<CGraphicDataBank>();
        m_pFrontEndGraphicBank.Data = 0;
    }
    if (m_pMeshBank.Data) {
        CMeshBankManager* meshBanks = GFGetSystemManager()->GetMeshBankManager();
        meshBanks->CloseMeshBank(m_pMeshBank);
        m_pMeshBank.~CCountedPointer<CMeshDataBank>();
        m_pMeshBank.Data = 0;
    }
    m_pMenuFont.~CCountedPointer<const CFontBank>();
    m_pMenuFont.Data = 0;
}
// ---- end source ----
#pragma optimize("", on)
static int g_copies = 0, g_dtors = 0, g_managerDtors = 0, g_graphicCloses = 0, g_meshCloses = 0;
static void* g_closedGraphic = 0; static void* g_closedMesh = 0;
template<class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { ++g_copies; }
template<class T> CCountedPointer<T>::~CCountedPointer() { ++g_dtors; Data = 0; Info = 0; }
CFrontEndManager::~CFrontEndManager() { ++g_managerDtors; }
void CGraphicBankManager::CloseGraphicBank(CCountedPointer<CGraphicDataBank> bank) { ++g_graphicCloses; g_closedGraphic = bank.Data; }
void CMeshBankManager::CloseMeshBank(CCountedPointer<CMeshDataBank> bank) { ++g_meshCloses; g_closedMesh = bank.Data; }
static CGraphicBankManager g_gbm;
CGraphicBankManager* g_pGraphicBankManager = &g_gbm;
static CMeshBankManager g_mbm;
static CSystemManager g_sys;
CSystemManager* __cdecl GFGetSystemManager() { return &g_sys; }
int main()
{
    g_sys.MeshBankManager = &g_mbm;
    static unsigned char raw[sizeof(CNewFrontendGameComponent)];
    memset(raw, 0, sizeof(raw));
    CNewFrontendGameComponent* c = (CNewFrontendGameComponent*)raw;
    int a, b, d, e;
    c->m_pFrontEndManager = new CFrontEndManager;
    c->m_pEngine.Data = (CIEngine*)&a;
    c->m_pFrontEndGraphicBank.Data = (CGraphicDataBank*)&b;
    c->m_pMeshBank.Data = (CMeshDataBank*)&d;
    c->m_pMenuFont.Data = (const CFontBank*)&e;
    c->CleanUpInterfaces();
    CHECK(g_managerDtors == 1 && c->m_pFrontEndManager == 0);
    CHECK(g_graphicCloses == 1 && g_closedGraphic == (void*)&b && g_meshCloses == 1 && g_closedMesh == (void*)&d);
    CHECK(c->m_pEngine.Data == 0 && c->m_pFrontEndGraphicBank.Data == 0 && c->m_pMeshBank.Data == 0 && c->m_pMenuFont.Data == 0);
    CHECK(g_copies == 2);
    int dtorsAfterFirst = g_dtors;
    CHECK(dtorsAfterFirst >= 6);                         // 4 members + the 2 by-value copies (destroyed by the callees)
    c->CleanUpInterfaces();                              // all empty: only the font is destroyed again
    CHECK(g_managerDtors == 1 && g_graphicCloses == 1 && g_meshCloses == 1 && g_dtors == dtorsAfterFirst + 1);
    if (fails == 0) printf("0042dd28_TEST PASS\n"); else printf("0042dd28_TEST FAIL\n");
    return fails;
}