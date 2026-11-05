// CSystemManager::~CSystemManager() -- destroy the system manager's members (strings and the counted manager pointers) and chain to CInitBaseClass
// Leaf: ??1CSystemManager
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
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
#include "engine/CCountedPointer.h"


class CCharString { public:
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                 // 0x0099B510
};

class CInitBaseClass { public:
    bool m_initialised;                             // +0x04
    virtual ~CInitBaseClass();                      // 0x0099A410
};

class CDebugManager; class CInputManager; class CDisplayManager; class CGraphicBankManager;
class CMeshBankManager; class CFontManager; struct CExtraManager; class CProfileManager;

class CSystemManager : public CInitBaseClass { public:
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