#include <cstdio>
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
namespace NLocalisation {
CCharString __fastcall GetTextBankName();                // 0x00415260
}
struct CCPPointerInfo;
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
class CFrontEndManager {
public:
    unsigned char               _pad00[0x1c];
    CNewFrontendGameComponent*  PGameComponent;          // +0x1c
    unsigned char               _pad20[0xc0 - 0x20];
    bool                        m_bQuitFromGame;         // +0xc0
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

#include <stdio.h>
#include <string.h>
static int g_log[32]; static int g_n;
static void L(int x) { if (g_n < 32) g_log[g_n++] = x; }
static int g_tag;
CCharString::~CCharString() { L(90); }
CCharString __fastcall NLocalisation::GetTextBankName() { CCharString r; r.PStringData = &g_tag; return r; }
static const void* g_copied[4]; static int g_ncopy;
template <class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& o) : Object(o.Object), Info(o.Info) { g_copied[g_ncopy++] = o.Object; }
static void* g_resetTo;
template <class T> void CCountedPointer<T>::Reset(T* p) { L(7); g_resetTo = p; Object = p; }
static CDefinitionManager* g_defs = (CDefinitionManager*)0x5150;
CDefinitionManager* GetDefinitionManager() { L(9); return g_defs; }
static CGraphicDataBank* g_current;
void __fastcall SetCurrentGraphicBank(CGraphicDataBank* b) { L(6); g_current = b; }
NGameText::CDataBank::CDataBank() {}
NGameText::CDataBank::~CDataBank() {}
static const void* g_openName; static unsigned long g_openFlags;
bool NGameText::CDataBank::OpenReadOnly(const CCharString& n, unsigned long f) { L(8); g_openName = n.PStringData; g_openFlags = f; return true; }
static long g_dx, g_dy; static unsigned long g_bpp, g_rate; static int g_ms = -1;
CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetDisplayMode(const C2DExtentsI& s, unsigned long b, ESurfaceMultisampleType m, unsigned long r)
{ L(12); g_dx = s.X; g_dy = s.Y; g_bpp = b; g_ms = m; g_rate = r; return CREATE_DEVICE_0; }
static CDisplayManager g_dm;
CDisplayManager* g_pDisplayManager = &g_dm;
bool g_bFrontEndFlag_01375448;
void __fastcall NUISystem::CManager::Clear() { L(10); }
static NUISystem::CFrontEndManager g_fem;
NUISystem::CFrontEndManager* __fastcall NUISystem::CFrontEndManager::GetInstance_00595582() { L(1); return &g_fem; }
static CInputManager* g_initInput; static CDefinitionManager* g_initDefs; static void* g_initG; static void* g_initM;
void NUISystem::CFrontEndManager::Init(CCountedPointer<CGraphicDataBank> g, CCountedPointer<CMeshDataBank> m, CInputManager* i, CDefinitionManager* d)
{ L(11); g_initG = g.Object; g_initM = m.Object; g_initInput = i; g_initDefs = d; }
static int g_build = -1;
void NUISystem::CFrontEndManager::BuildComponents(bool f) { L(13); g_build = f ? 1 : 0; }
CNewFrontendGameComponent::~CNewFrontendGameComponent() {}
bool CNewFrontendGameComponent::InitialiseGraphics() { L(2); return true; }
void CNewFrontendGameComponent::InitialiseSounds() { L(3); }
void CNewFrontendGameComponent::InitialiseInput() { L(4); }
int main()
{
    static double raw[(sizeof(CNewFrontendGameComponent) + 7) / 8];
    CNewFrontendGameComponent& c = *(CNewFrontendGameComponent*)raw;   // no default ctor (CCountedPointer has only a copy ctor)
    c.m_pFrontEndGraphicBank.Object = (CGraphicDataBank*)0x1111; c.m_pMeshBank.Object = (CMeshDataBank*)0x2222;
    c.m_pInputManager = (CInputManager*)0x3333; c.WindowsMediaPlayerInstalled = false; g_bFrontEndFlag_01375448 = true;
    g_fem.m_bQuitFromGame = true;
    c.Init2();
    // the helper's own local CCharString may also log 90 (no NRVO in the test stub); compare without the 90s, then
    // require the temporary's destruction (90) right after OpenReadOnly (8)
    static const int want[] = { 1, 2, 3, 4, 6, 7, 8, 10, 9, 11, 13, 12 };
    int f[32]; int nf = 0; bool dtorAfterOpen = false;
    for (int i = 0; i < g_n; ++i) { if (g_log[i] == 90) { if (i > 0 && g_log[i - 1] == 8) dtorAfterOpen = true; } else f[nf++] = g_log[i]; }
    bool ok = nf == 12 && memcmp(f, want, sizeof(want)) == 0 && dtorAfterOpen;
    ok = ok && c.m_pFrontEndManager == &g_fem && g_fem.PGameComponent == &c && g_fem.m_bQuitFromGame == false
        && g_current == (CGraphicDataBank*)0x1111 && g_resetTo != 0 && c.m_pTextBank.Object == g_resetTo
        && g_openName == &g_tag && g_openFlags == 0x100 && g_initG == (void*)0x1111 && g_initM == (void*)0x2222
        && g_initInput == (CInputManager*)0x3333 && g_initDefs == g_defs && g_build == 1
        && g_dx == 1024 && g_dy == 768 && g_bpp == 16 && g_ms == 0 && g_rate == 60;
    if (!ok) { for (int i = 0; i < g_n; ++i) printf("%d ", g_log[i]); printf("q=%d cur=%p reset=%p obj=%p open=%p f=%lu G=%p M=%p I=%p D=%p b=%d dx=%ld %ld %lu %d %lu\n", g_fem.m_bQuitFromGame, g_current, g_resetTo, c.m_pTextBank.Object, g_openName, g_openFlags, g_initG, g_initM, g_initInput, g_initDefs, g_build, g_dx, g_dy, g_bpp, g_ms, g_rate); }
    printf(ok ? "0042e98f_TEST PASS\n" : "0042e98f_TEST FAIL\n");
    return ok ? 0 : 1;
}