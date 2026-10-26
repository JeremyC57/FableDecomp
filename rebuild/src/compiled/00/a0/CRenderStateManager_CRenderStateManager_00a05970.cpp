// CRenderStateManager::CRenderStateManager(unsigned long initFlags) -- construct the device-state table and the bookmark dummy state, empty the state lists and record the init flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A05970, length 108 bytes (+4 int3 padding).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (base), CDeviceState::CDeviceState @ 0x00A079B0
// (member at +0x2818). Vtable 0x0129C978 (CRenderStateManager, rtti-vtables.tsv).
// Shape: CStateInfo's inline ctor zeroes CurrentState/DesiredState/BookmarkMask and the two flag bytes of the
// BookmarkDummyState member (+0x3A24, the D3D state word +0x3A30 is left alone); the list sizes, InitFlags, the
// dummy's StateType = 4 and CurrentBookmarkMask = 1 are body assignments (they store after the member ctors, and
// InitFlags, an argument, stores after the zero words).
// Model: local polymorphic model; offsets from engine/CRenderStateManager.h (which holds the vptr as a plain word).
struct IDirect3DDevice9;

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

struct CStateInfo {                                  // 20 bytes (PDB)
    CStateInfo() : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false), StateType(0) {}
    unsigned long CurrentState;                      // +0x00
    unsigned long DesiredState;                      // +0x04
    unsigned long BookmarkMask;                      // +0x08
    long          State;                             // +0x0C
    bool          DirtyListFlag;                     // +0x10
    unsigned char StateType;                         // +0x11
    unsigned char TextureStage;                      // +0x12
};

struct CDeviceState {                                // 0x120C bytes
    CDeviceState();                                  // 0x00A079B0
    unsigned char _states[0x120C];
};

struct CStateStackElement {
    CStateInfo*   State;
    unsigned long Value;
};

class CRenderStateManager : public CBaseClassNonCopyable { public: // vtable 0x0129C978
    CRenderStateManager(unsigned long initFlags);
    virtual ~CRenderStateManager();

    IDirect3DDevice9*  PD3DDevice;                   // +0x04
    CStateStackElement StateStack[1024];             // +0x08
    CStateInfo*        StateUpdateList[256];         // +0x2008
    CStateInfo*        AllStatesList[256];           // +0x2408
    long               StateStackSize;               // +0x2808
    long               StateUpdateListSize;          // +0x280C
    long               AllStatesListSize;            // +0x2810
    unsigned long      CurrentBookmarkMask;          // +0x2814
    CDeviceState       DeviceState;                  // +0x2818
    CStateInfo         BookmarkDummyState;           // +0x3A24
    unsigned long      InitFlags;                    // +0x3A38
};

CRenderStateManager::CRenderStateManager(unsigned long initFlags)
{
    StateStackSize = 0;
    StateUpdateListSize = 0;
    AllStatesListSize = 0;
    InitFlags = initFlags;
    BookmarkDummyState.StateType = 4;
    CurrentBookmarkMask = 1;
}