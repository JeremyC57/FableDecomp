// CGamePlayerInterface::~CGamePlayerInterface() -- close the input packet file if it was being saved or loaded, then destroy the button vectors, the controls map, the controls def pointer, the loaded input event and the packet file, then the CInputProcessManager base
// Leaf: ??1CGamePlayerInterface
// Retail: Fable.exe (TLC, VC7.1) @ 0x00446BE0, 296 bytes (+ int3 padding), plain `ret`.
// Label: the catalog's "CNewFrontendGameComponent::~CNewFrontendGameComponent" is wrong (that class is 0x150 bytes in
//   Ego_r.pdb); the vtable stored first, 0x01231BDC, is CGamePlayerInterface's (RTTI), its only caller is the deleting
//   destructor 0x00449420 (vtable slot 0), and the member offsets match CGamePlayerInterface less the 12-byte retail vectors.
// Callees: the packet file's virtual Close (slot 2, +8) and CDiskFileWin32::Close 0x0099A920 (inline file destructor),
//   ~CWideString 0x0099B510, ~CBaseClassNonCopyable 0x0099A300, the range destroys 0x00448860 (CCountedPointer<
//   CInputTypeBase> elements) and 0x004488B0 (pair<EGameAction, CCountedPointer<..>> elements) with a const tag
//   reference, free 0x00BFEA14, ~CInputEvent 0x00A03B30 (empty, out of line), ~CInputProcessManager 0x00A0D810.
// Layout (retail, Ego_r.pdb CGamePlayerInterface less 4 bytes per STLport vector): Component +0x6f8, PlayerManager
//   +0x6fc, PacketSave +0x700, PacketLoad +0x701, PacketFile +0x704 (CDiskFileWin32, 0x1c), InputEventIsLoaded +0x720,
//   LoadedInputEvent +0x724 (Frame, InputEvent +4, MousePos, Camera), Suspended +0x79c, PControlsDef +0x7a0,
//   ControlsDefIndex +0x7a4, Controls +0x7a8 (vector + Dirty), AssignableSpecialMoveButtons +0x7b8,
//   QuickAccessItemButtons +0x7c4, ContextSensitiveItemButtons +0x7d0. engine/CGamePlayerInterface.h carries the PDB
//   offsets, so the class is modelled locally.
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

struct CWideString {
    void* PStringData;
    ~CWideString();                                  // 0x0099B510
};
struct CBaseClassNonCopyable {                       // vtable 0x0129A7C4
    virtual void v00();
    ~CBaseClassNonCopyable();                        // 0x0099A300
};
struct CDiskFileWin32 : CBaseClassNonCopyable {      // vtable 0x0122D06C
    virtual void v00();
    virtual void Open();                             // slot 1
    virtual void Close();                            // slot 2 (+8), 0x0099A920
    long        AccessType;                          // +0x04
    long        Position;                            // +0x08
    long        Length;                              // +0x0c
    void*       WinFileHandle;                       // +0x10
    bool        Opened;                              // +0x14
    CWideString Pathname;                            // +0x18
    ~CDiskFileWin32() { Close(); }
};

struct CInputEvent {
    unsigned char Data[0x34];
    ~CInputEvent();                                  // 0x00A03B30
};
struct CSavedInputEvent {
    unsigned long Frame;                             // +0x00
    CInputEvent   InputEvent;                        // +0x04
    unsigned char MousePosAndCamera[0x78 - 0x38];    // +0x38 C2DVector MousePos, CCamera Camera
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +4
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CControlsDef : CDefBase { };
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

struct CInputTypeBase;
template <class T> struct CCountedPointer { T* Data; void* Info; };
struct SControlEntry { long Action; CCountedPointer<CInputTypeBase> Input; };   // pair<EGameAction, CCountedPointer<const CInputTypeBase>>

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
void __fastcall DestroyRange(CCountedPointer<CInputTypeBase>* first, CCountedPointer<CInputTypeBase>* last, const SNonscalarPtrIteratorTag& tag);   // 0x00448860
void __fastcall DestroyRange(SControlEntry* first, SControlEntry* last, const SNonscalarPtrIteratorTag& tag);                                     // 0x004488B0

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector()
    {
        DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start)
            free(_M_start);
    }
};
struct CControlsMap {                                // CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase>, ...>
    CRetailVector<SControlEntry> Entries;            // +0x00
    bool                         Dirty;              // +0x0c
};

struct CInputProcessManager {
    virtual void v00();
    unsigned char _body[0x6f8 - 0x04];
    ~CInputProcessManager();                         // 0x00A0D810
};

struct CMainGameComponent;
struct CPlayerManager;
struct CGamePlayerInterface : CInputProcessManager { // vtable 0x01231BDC
    virtual void v00();
    CMainGameComponent* Component;                   // +0x6f8
    CPlayerManager*     PlayerManager;               // +0x6fc
    bool                PacketSave;                  // +0x700
    bool                PacketLoad;                  // +0x701
    CDiskFileWin32      PacketFile;                  // +0x704
    bool                InputEventIsLoaded;          // +0x720
    CSavedInputEvent    LoadedInputEvent;            // +0x724
    bool                Suspended;                   // +0x79c
    CDefPointer<CControlsDef> PControlsDef;          // +0x7a0
    long                ControlsDefIndex;            // +0x7a4
    CControlsMap        Controls;                    // +0x7a8
    CRetailVector<CCountedPointer<CInputTypeBase> > AssignableSpecialMoveButtons;   // +0x7b8
    CRetailVector<CCountedPointer<CInputTypeBase> > QuickAccessItemButtons;         // +0x7c4
    CRetailVector<CCountedPointer<CInputTypeBase> > ContextSensitiveItemButtons;    // +0x7d0
    long                CurrentQueuedProcessedInputNo;   // +0x7dc
    ~CGamePlayerInterface();
};

CGamePlayerInterface::~CGamePlayerInterface()
{
    if (PacketSave || PacketLoad)
        PacketFile.Close();
}