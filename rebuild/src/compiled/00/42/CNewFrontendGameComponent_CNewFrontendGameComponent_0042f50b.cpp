// CNewFrontendGameComponent::~CNewFrontendGameComponent() -- shut the front end down if that has not happened yet, then destroy the members (save name, file lists, banks, engine, def, init data, menu font) and the game-component base
// Leaf: ??1CNewFrontendGameComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F50B, 158 bytes. Size TU (`#pragma optimize("s", on)`: `and dword ptr [esi+3Ch],0`).
// Callees: CNewFrontendGameComponent::Shutdown @ 0x0042EBB6, ~CWideString @ 0x0099B510, vector<CWideString>::~vector @ 0x00412130,
//   free @ 0x00BFEA14 (the POD vector's storage), the CCountedPointer destructors @ 0x00403750 / 0x0042F974 / 0x0042F9A0 /
//   0x0042918C / 0x00419108 / 0x0042F948 (labels "_Dest_val<...COptimisedPrimitive>" are propagated byte twins),
//   ~CMainGameComponentInit @ 0x004131D0, the def's virtual slot 1 (destroy), CBase::~CBase @ 0x0099A300 (tail jump).
//   Vtables 0x01230CA0 / 0x01230C94 (this class at +0 / +4), 0x0122ECB0 / 0x0122ECA4 (the game-component base).
// Layout = Ego_r.pdb up to +0x6c (m_pMenuFont +0x10, m_bInitMainGame +0x2c, m_pFrontEndDef +0x3c, banks/engine +0x40..+0x60,
//   FileNames +0x6c); the retail STLport vectors are 12 bytes, so FileTimes is +0x78 and SaveGame +0x84 (PDB +0x7c / +0x8c).
//   The shut-down flag at +8 belongs to the game-component base.
// Shape: members destroyed last to first; the def pointer's release is inline (local copy, `dec [ecx+4] ; jne`, slot 1, the zero
//   store inside the null test); the trivially destructible vector frees its storage inline; the base's inline destructor
//   stores its two vtables and tail-jumps to ~CBase. The size TU inlines the def release and the base destructor only when they
//   are `__forceinline`.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
namespace N_0099a300 { struct CBase {
    virtual ~CBase();                                      // 0x0099A300
}; } using N_0099a300::CBase;
struct CGameComponentInterface {
    virtual void v00();
};
class CGameComponent : public CBase, public CGameComponentInterface { public:   // vtables 0x0122ECB0 / 0x0122ECA4
    bool m_bShutDown;                                      // +0x08
    virtual __forceinline ~CGameComponent() { }
};
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    ~CCountedPointer();                                    // out of line per instantiation
};
class CFontBank; class CGraphicDataBank; class CMeshDataBank; class CASoundBank; class CIEngine; class CInputManager;
namespace NGameText { class CDataBank; }
class CFrontEndDef { public:
    virtual void v00();
    virtual void Destroy();                                // slot 1
    long m_count;                                          // +0x04
};
struct CDefPointer_CFrontEndDef {
    CFrontEndDef* Data;
    __forceinline ~CDefPointer_CFrontEndDef()
    {
        CFrontEndDef* def = Data;
        if (def) {
            if (--def->m_count == 0)
                def->Destroy();
            Data = 0;
        }
    }
};
class CMainGameComponentInit { public:
    unsigned long _data[4];
    ~CMainGameComponentInit();                             // 0x004131D0
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                        // 0x0099B510
};
struct CWideStringVector {                                 // STLport vector<CWideString>
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    ~CWideStringVector();                                  // 0x00412130
};
class CDateAndTime { public:
    unsigned char _data[0x10];
};
struct CDateAndTimeVector {                                // STLport vector<CDateAndTime> over the malloc allocator
    CDateAndTime* _M_start;
    CDateAndTime* _M_finish;
    CDateAndTime* _M_end_of_storage;
    __forceinline ~CDateAndTimeVector()
    {
        if (_M_start)
            free(_M_start);
    }
};
enum ECursorMovement { CURSOR_MOVEMENT_NONE = 0 };
class CNewFrontendGameComponent : public CGameComponent { public:        // vtables 0x01230CA0 / 0x01230C94
    unsigned long                          m_reserved0c;               // +0x0c (base padding in the PDB view)
    CCountedPointer<const CFontBank>       m_pMenuFont;                // +0x10
    ECursorMovement                        CurrentMovement;            // +0x18
    bool                                   StartedRepeatingMovement;   // +0x1c
    double                                 TimeLastMovementHappened;   // +0x20
    bool                                   m_bAllowInputs;             // +0x28
    bool                                   m_bCreateGame;              // +0x29
    bool                                   CreateDevFrontEnd;          // +0x2a
    CMainGameComponentInit                 m_bInitMainGame;            // +0x2c
    CDefPointer_CFrontEndDef               m_pFrontEndDef;             // +0x3c
    CCountedPointer<CGraphicDataBank>      m_pFrontEndGraphicBank;     // +0x40
    CCountedPointer<CMeshDataBank>         m_pMeshBank;                // +0x48
    CCountedPointer<CASoundBank>           PSampleBank;                // +0x50
    CCountedPointer<CIEngine>              m_pEngine;                  // +0x58
    CCountedPointer<NGameText::CDataBank>  m_pTextBank;                // +0x60
    CInputManager*                         m_pInputManager;            // +0x68
    CWideStringVector                      FileNames;                  // +0x6c
    CDateAndTimeVector                     FileTimes;                  // +0x78
    CWideString                            SaveGame;                   // +0x84
    void Shutdown();                                       // 0x0042EBB6
    virtual ~CNewFrontendGameComponent();
};

CNewFrontendGameComponent::~CNewFrontendGameComponent()
{
    if (!m_bShutDown)
        Shutdown();
}