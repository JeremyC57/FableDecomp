// CGamePlayerInterface::SaveInitialInfo() -- write one saved input event (frame, input event, current mouse position, camera) to the packet file
// Leaf: ?SaveInitialInfo@CGamePlayerInterface_Methods@@QAEXXZ
// Retail: Fable.exe @ 0x00445AE0, 85 bytes, plain `ret`; no direct callers in the xref index; Ego_r.pdb
// ?SaveInitialInfo@CGamePlayerInterface@@AAEXXZ.
// Body: a CSavedInputEvent local (0x78 bytes) with its implicit constructor inlined -- CInputEvent::CInputEvent on +4
// (0x00A04410), the CCamera member's HeightLocked / ViewVecZLocked (+0x40+0x24 / +0x25) = false; if the input manager's
// mouse (g_pInputManager 0x013B8388, PMouse +0x24) exists, its virtual slot 1 (+4) fills the MousePos member (+0x38);
// PacketFile (+0x704, CDiskFileWin32) .Write(&saved, 0x78, 0) (CAFile slot 4, +0x10); the implicit destructor runs
// CInputEvent::~CInputEvent on +4 (0x00A03B30).
// Layout: engine/CGamePlayerInterface.h PacketFile +0x704 (opaque; the CAFile virtual order as the landed 0x004160A6 row);
// Ego_r.pdb CSavedInputEvent {Frame, CInputEvent InputEvent +4 (52 bytes), C2DVector MousePos +0x38, CCamera Camera
// +0x40 (56 bytes: HeightLocked +0x24, ViewVecZLocked +0x25)}; CInputManager PMouse at retail +0x24 (Ego_r.pdb +0x2c: the
// two vectors ahead are 12-byte STLport vectors). The mouse's slot-1 name is not confirmed.
// Shape: /O2 (16-aligned entry); the mouse bound to a local (otherwise the vptr / argument registers swap).
#include "engine/CGamePlayerInterface.h"
class CWideString;
class CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CAFile {
public:
    virtual ~CAFile();
    virtual void Open(const CWideString& name, FileAccessType access, unsigned long flags) = 0;
    virtual void Close() = 0;                                                    // slot 2
    virtual void Read(void* dst, long size, CASuspendableProcess* process) = 0;  // slot 3
    virtual void Write(const void* src, long size, CASuspendableProcess* process) = 0;   // slot 4 (+0x10)
};
struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };
class CCamera {
public:
    C3DVector     WorldPosition;                         // +0x00
    unsigned char RHSet[0x18];                           // +0x0c CRightHandedSet
    bool          HeightLocked;                          // +0x24
    bool          ViewVecZLocked;                        // +0x25
    unsigned long FOVFlags;                              // +0x28
    float         HorizontalFOV;                         // +0x2c
    float         VerticalFOV;                           // +0x30
    float         Zoom;                                  // +0x34
    CCamera() : HeightLocked(false), ViewVecZLocked(false) {}
};
class CInputEvent {
public:
    unsigned char Body[0x34];
    CInputEvent();                                       // 0x00A04410
    ~CInputEvent();                                      // 0x00A03B30
};
class CSavedInputEvent {
public:
    unsigned long Frame;                                 // +0x00
    CInputEvent   InputEvent;                            // +0x04
    C2DVector     MousePos;                              // +0x38
    CCamera       Camera;                                // +0x40
};
class CMouse {
public:
    virtual ~CMouse();
    virtual void GetMousePosition(C2DVector& position);  // slot 1 (+4); name not confirmed
};
class CInputManager {
public:
    unsigned char _pad00[0x24];
    CMouse*       PMouse;                                // +0x24 scoped_ptr<CMouse>
};
extern CInputManager* g_pInputManager;                   // 0x013B8388
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    CAFile& GetPacketFile() { return *reinterpret_cast<CAFile*>(PacketFile); }
    void SaveInitialInfo();
};
void CGamePlayerInterface_Methods::SaveInitialInfo()
{
    CSavedInputEvent saved;
    CMouse* mouse = g_pInputManager->PMouse;
    if (mouse)
        mouse->GetMousePosition(saved.MousePos);
    GetPacketFile().Write(&saved, sizeof(saved), 0);
}