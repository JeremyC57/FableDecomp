// CSystemManager::~CSystemManager() -- destroy the system manager's members (strings and the counted manager pointers) and chain to CInitBaseClass
// Leaf: ??1CSystemManager
// Retail 0x009A5800, 416 bytes (tail `jmp` to the base destructor). Callees: ~CCharString @ 0x0099EAE0,
// ~CWideString @ 0x0099B510, operator delete @ 0x00BFE9BC, CInitBaseClass::~CInitBaseClass @ 0x0099A410;
// each counted pointer's per-type destroy function is called through its count block (+4, object in ecx).
// Layout: Ego_r.pdb CSystemManager (0x178) has LibDebugManager +0x50, PInputManager +0x58, PDisplayManager +0x60,
// DebugManager +0x68, PGraphicBankManager +0x6c, PMeshBankManager +0x74, PFontManager +0x7c, then bools at +0x84.
// Retail destroys an extra counted pointer at +0x84 and every later member sits 8 bytes higher (WinAppName +0xa0,
// ProfileManager +0xf8, TempPath +0x164, ApplicationName +0x168): retail has one more CCountedPointer member
// there than the PDB (named PExtraManager here). Vtable 0x0129AFFC.
// Shape: the compiler-generated member destruction in reverse order; the counted pointer's inline destructor
// re-reads the block after the decrement (count touched through the block pointer) and clears both words.
struct SCountBlock {
    long  m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};

template<class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CCharString {
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                 // 0x0099B510
};

struct CInitBaseClass {
    bool m_initialised;                             // +0x04
    virtual ~CInitBaseClass();                      // 0x0099A410
};

struct CDebugManager; struct CInputManager; struct CDisplayManager; struct CGraphicBankManager;
struct CMeshBankManager; struct CFontManager; struct CExtraManager; struct CProfileManager;

struct CSystemManager : CInitBaseClass {
    unsigned char _pad08[0x50 - 0x08];
    CCountedPointer<CDebugManager>       LibDebugManager;      // +0x50
    CCountedPointer<CInputManager>       PInputManager;        // +0x58
    CCountedPointer<CDisplayManager>     PDisplayManager;      // +0x60
    CDebugManager*                       DebugManager;         // +0x68
    CCountedPointer<CGraphicBankManager> PGraphicBankManager;  // +0x6c
    CCountedPointer<CMeshBankManager>    PMeshBankManager;     // +0x74
    CCountedPointer<CFontManager>        PFontManager;         // +0x7c
    CCountedPointer<CExtraManager>       PExtraManager;        // +0x84 (retail only)
    unsigned char _pad8c[0xa0 - 0x8c];
    CWideString                          WinAppName;           // +0xa0
    unsigned char _pada4[0xf8 - 0xa4];
    CCountedPointer<CProfileManager>     ProfileManager;       // +0xf8
    unsigned char _pad100[0x164 - 0x100];
    CWideString                          TempPath;             // +0x164
    CCharString                          ApplicationName;      // +0x168
    unsigned char _pad16c[0x180 - 0x16c];
    virtual ~CSystemManager();
};

CSystemManager::~CSystemManager()
{
}
