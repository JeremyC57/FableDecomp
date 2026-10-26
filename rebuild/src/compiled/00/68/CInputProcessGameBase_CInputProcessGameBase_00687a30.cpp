// CInputProcessGameBase::CInputProcessGameBase(const CInputProcessGameBaseInit& init) -- the CAInputProcess base, this vtable, the four init pointers copied (World, MyPlayer, DefinitionManager, Component) and PGamePlayerInterface cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687A30, length 52 bytes (+ int3 padding). Vtable 0x0125BE98 (CInputProcessGameBase).
// Callees: CAInputProcess::CAInputProcess @ 0x00A0D290 (0x14 bytes: vptr, PPlayerInterface, the list position).
// Layout: engine/CInputProcessGameBase.h (World +0x14, MyPlayer +0x18, DefinitionManager +0x1c, Component +0x20, PGamePlayerInterface +0x24);
// CInputProcessGameBaseInit (PDB, 16 bytes) holds the same four pointers in order. The base is modelled locally for its constructor.
// Shape: the init pointer is loaded after the base call, the four copies alternate ecx/edx, the interface pointer is an immediate zero.
// Leaf: ??0CInputProcessGameBase
class CWorld; class CPlayer; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBaseInit { public:
    CWorld*                 World;                  // +0x00
    CPlayer*                MyPlayer;               // +0x04
    CGameDefinitionManager* DefinitionManager;      // +0x08
    CMainGameComponent*     Component;              // +0x0c
};
class CAInputProcess { public:                             // 0x14 bytes
    virtual void v00();
    void* PPlayerInterface;                         // +0x04
    void* InputProcessListPos_Data;                 // +0x08
    void* InputProcessListPos_Next;                 // +0x0c
    void* InputProcessListPos_Previous;             // +0x10
    CAInputProcess();                               // 0x00A0D290
};
struct CInputProcessGameBase : CAInputProcess {     // vtable 0x0125BE98
    virtual void v00();
    CWorld*                 World;                  // +0x14
    CPlayer*                MyPlayer;               // +0x18
    CGameDefinitionManager* DefinitionManager;      // +0x1c
    CMainGameComponent*     Component;              // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;   // +0x24
    CInputProcessGameBase(const CInputProcessGameBaseInit& init);
};

CInputProcessGameBase::CInputProcessGameBase(const CInputProcessGameBaseInit& init)
    : CAInputProcess()
{
    World = init.World;
    MyPlayer = init.MyPlayer;
    DefinitionManager = init.DefinitionManager;
    Component = init.Component;
    PGamePlayerInterface = 0;
}