// CGamePlayerInterface::CGamePlayerInterface(CMainGameComponent* component, CPlayerManager* playerManager) -- build the input process manager base, the packet file, the saved input event, empty control containers and the processed-input block, then (outside front-end compilation) select the control scheme from the definitions
// Leaf: ??0CGamePlayerInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004473A0, length 324 bytes (`ret 8`). Vtable 0x01231BDC.
// Callees: CInputProcessManager ctor @ 0x00A0D4A0 (base), CBase ctor @ 0x0099A2F0 (the packet file's root; file vtable
//   0x0122D06C), CWideString ctor @ 0x0099AED0, CSavedInputEvent part ctor @ 0x00A04410, CProcessedInput part ctor
//   @ 0x00A0D300, CEngine::PeekDefinitionManager @ 0x00415FBC, CGamePlayerInterface::SetControlScheme(long) @ 0x00446EF0;
//   the front-end compile flag 0x013B8648.
// Shape: the packet file sets Open in its constructor body (after FileName); the processed-input block constructs its four
//   event slots inline (type -1, unused) and the outer body clears the flags and calls its Reset.
// Layout: Ego_r.pdb names at the retail offsets (Component +0x6F8, PlayerManager +0x6FC, PacketSave +0x700, PacketFile
//   +0x704, InputEventIsLoaded +0x720, Suspended +0x79C, Controls +0x7A8 .. CurrentQueuedProcessedInputNo +0x7DC,
//   CurrentProcessedInput +0x7E0); the engine header's PDB containers are larger, so a local model.
class CWideString { public: void* PStringData; CWideString(); };   // 0x0099AED0
struct CBase { CBase(); virtual void v00(); };          // 0x0099A2F0
class CDiskFileWin32 : public CBase { public:                         // vtable 0x0122D06C, 0x1C bytes
    unsigned char _pad04[0x10];
    bool          Open;                                 // +0x14
    CWideString   FileName;                             // +0x18
    CDiskFileWin32() { Open = false; }
    virtual void v00();
};
struct CSavedInputEventPart { CSavedInputEventPart(); unsigned char _body[0x60]; };   // 0x00A04410
struct CSavedInputEvent {                               // +0x724 in the interface
    unsigned char        _pad00[4];
    CSavedInputEventPart Part;                          // +0x04
    bool                 Flag64;                        // +0x64
    bool                 Flag65;                        // +0x65
    CSavedInputEvent() : Flag64(false), Flag65(false) {}
};
struct CGameEventSlot {                                 // 0x28 bytes
    unsigned char _pad00[4];
    signed char   Type;                                 // +0x04
    unsigned char _pad05[0x21];
    bool          Used;                                 // +0x26
    unsigned char _pad27;
    CGameEventSlot() : Type(-1), Used(false) {}
};
class CProcessedInput { public:                                // 0xB0 bytes
    unsigned char  _pad00[8];
    CGameEventSlot Events[4];                           // +0x08
    unsigned char  _padA8[8];
    void Reset();                                       // 0x00A0D300 (manifest "CGameEventPackage" ctor label propagated)
};
template<class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CInputPointer { void* Data; void* Info; };
struct CControlMap : CRetailVector<CInputPointer> {
    unsigned char Compare;
    bool          Dirty;
    CControlMap() : Dirty(false) {}
};
class CInputProcessManager { public:
    CInputProcessManager();                             // 0x00A0D4A0
    virtual ~CInputProcessManager();
    unsigned char _body[0x6f4];
};
class CMainGameComponent;
class CPlayerManager;
class CGameDefinitionManager { public: unsigned char _pad[0xd0]; struct CControlsSettings { unsigned char _p[0x3c]; long Scheme; }* Settings; };
class CMainGameComponent { public: public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC
};   // 0x00415FBC
extern bool g_bCompileFrontendDefinitions;              // 0x013B8648

class CGamePlayerInterface : public CInputProcessManager { public:
    CMainGameComponent*   Component;             // +0x6F8
    CPlayerManager*              PlayerManager;         // +0x6FC
    bool                         PacketSave;            // +0x700
    bool                         PacketLoad;            // +0x701
    CDiskFileWin32               PacketFile;            // +0x704
    bool                         InputEventIsLoaded;    // +0x720
    unsigned char                _pad721[3];
    CSavedInputEvent             LoadedInputEvent;      // +0x724
    unsigned char                _pad78a[0x12];
    bool                         Suspended;             // +0x79C
    void*                        PControlsDef;          // +0x7A0
    long                         ControlsDefIndex;      // +0x7A4
    CControlMap                  Controls;              // +0x7A8
    CRetailVector<CInputPointer> AssignableSpecialMoveButtons;   // +0x7B8
    CRetailVector<CInputPointer> QuickAccessItemButtons;         // +0x7C4
    CRetailVector<CInputPointer> ContextSensitiveItemButtons;    // +0x7D0
    long                         CurrentQueuedProcessedInputNo;  // +0x7DC
    CProcessedInput              CurrentProcessedInput; // +0x7E0
    long                         CurrentEventInProcessedInput;   // +0x890
    bool                         Unknown894;            // +0x894
    CGamePlayerInterface(CMainGameComponent* component, CPlayerManager* playerManager);
    void SetControlScheme(long scheme);                 // 0x00446EF0
};

CGamePlayerInterface::CGamePlayerInterface(CMainGameComponent* component, CPlayerManager* playerManager)
    : Component(component), PlayerManager(playerManager), PControlsDef(0)
{
    CurrentQueuedProcessedInputNo = 0;
    PacketSave = false;
    PacketLoad = false;
    InputEventIsLoaded = false;
    CurrentProcessedInput.Reset();
    Suspended = false;
    Unknown894 = false;
    ControlsDefIndex = 0;
    CurrentEventInProcessedInput = 0;
    if (!g_bCompileFrontendDefinitions)
        SetControlScheme(Component->PeekDefinitionManager().Settings->Scheme);
}