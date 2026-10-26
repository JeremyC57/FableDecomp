// CGamePlayerInterface::LoadInitialInfo() -- read the first saved input event from the packet file and restore its mouse position
// Leaf: ?LoadInitialInfo@CGamePlayerInterface_Methods@@QAEXXZ
// Retail: Fable.exe @ 0x00445B40, 55 bytes, plain `ret`, no direct reference; Ego_r.pdb
// ?LoadInitialInfo@CGamePlayerInterface@@AAEXXZ. PacketFile (+0x704, CDiskFileWin32) is read through its CAFile
// interface -- virtual slot 3, Read(void* buffer, long size, CASuspendableProcess* process) -- into LoadedInputEvent
// (+0x724, the 0x78-byte CSavedInputEvent); then the input manager (0x013B8388) -> PMouse (+0x24), when present, gets
// virtual slot 4 SetPos(const C2DVector*) (CMouseDX::SetPos 0x00AB4700 in vtable 0x0129EA14) with the event's MousePos
// (CSavedInputEvent +0x38, i.e. +0x75c).
// Layout: CGamePlayerInterface from engine/CGamePlayerInterface.h (PacketFile +0x704, LoadedInputEvent +0x724 match
// retail); it keeps both as opaque bytes, so inline getters give the CAFile view and the MousePos. CAFile and CMouse
// are local because their virtual calls need polymorphic declarations (engine/CMouse.h models the vptr as void*).
// CInputManager: retail PMouse +0x24 (Ego_r.pdb +0x2c, two 16-byte vectors before it); no engine/ header.
// Shape: /O2; PMouse read through an inline const getter (`mov edx,[ecx]` for the vtable) -- reading the member twice
// directly picks eax.
#include "engine/CGamePlayerInterface.h"
class CASuspendableProcess;
class C2DVector { public: float X; float Y; };
class CAFile {
public:
    virtual ~CAFile();
    virtual void _slot01();
    virtual void _slot02();
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);  // slot 3 (+0x0c)
};
class CMouse {
public:
    virtual ~CMouse();
    virtual void _slot01();
    virtual void _slot02();
    virtual void _slot03();
    virtual void SetPos(const C2DVector* pos);          // slot 4 (+0x10)
};
class CInputManager { public:
    unsigned char _pad00[0x24];
    CMouse*       PMouse;                                // +0x24 scoped_ptr<CMouse>
    CMouse* GetMouse() const { return PMouse; }
};
extern CInputManager* g_pInputManager;                  // 0x013B8388
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    CAFile& GetPacketFile() { return *reinterpret_cast<CAFile*>(PacketFile); }
    const C2DVector* GetLoadedMousePos() const { return reinterpret_cast<const C2DVector*>(LoadedInputEvent + 0x38); }
    void LoadInitialInfo();
};
void CGamePlayerInterface_Methods::LoadInitialInfo()
{
    GetPacketFile().Read(LoadedInputEvent, sizeof(LoadedInputEvent), 0);
    if (g_pInputManager->GetMouse())
        g_pInputManager->GetMouse()->SetPos(GetLoadedMousePos());
}