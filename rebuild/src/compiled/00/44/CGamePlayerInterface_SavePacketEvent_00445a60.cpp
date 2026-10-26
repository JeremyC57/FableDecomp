// CGamePlayerInterface::SavePacketEvent(const CInputEvent& event) -- record an input event into the packet file: build a CSavedInputEvent holding the event, the game frame and (when the loader has a mouse device) the mouse position, and write its 0x78 bytes through the packet file
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00445A60, length 124 bytes (+ int3 padding). `ret 4`.
// Callees: CInputEvent::CInputEvent @ 0x00A04410 and ~CInputEvent @ 0x00A03B30 (as the landed CSavedInputEvent constructor 0x00447BB0 /
// CInputProcessTargetLockCycleTargets::Update), CMainGameComponent's frame getter @ 0x00416225 (`mov eax,[ecx+48h] ; ret`; label
// "CWorld::DrawGetEnvironment" is propagated), the loader's (g_Loader @ 0x013B8388) device at +0x24 virtual slot 1 (mouse position into
// the record), the packet file's (CDiskFileWin32 PacketFile +0x704) virtual slot 4 (+0x10, Write(buffer, size, 0)).
// Layout: engine/CGamePlayerInterface.h (Ego_r.pdb; retail agrees here): Component +0x6f8, PacketFile +0x704. CSavedInputEvent (0x78) as
// the landed constructor: Frame +0, InputEvent +4, MousePos +0x38, Camera +0x40 (inline constructor clears its two lock bytes).
// Shape: the record is a 0x78-byte frame local; the event copy is CInputEvent's implicit (bitwise) assignment (`rep movsd`, 13 dwords);
// the frame getter's result is stored straight into the record; the mouse device is a named local (the member chain twice swaps the
// eax/edx roles of the vtable and the argument); only the InputEvent member is destroyed at the end.
class CInputEvent { public:                                    // 0x34 bytes
    unsigned char _body[0x34];
    CInputEvent();                                      // 0x00A04410
    ~CInputEvent();                                     // 0x00A03B30
};

class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };

class CCamera { public:
    C3DVector       WorldPosition;                      // +0x00
    unsigned char   RHSet[0x18];                        // +0x0c CRightHandedSet (opaque here)
    bool            HeightLocked;                       // +0x24
    bool            ViewVecZLocked;                     // +0x25
    unsigned long   FOVFlags;                           // +0x28
    float           HorizontalFOV;                      // +0x2c
    float           VerticalFOV;                        // +0x30
    float           Zoom;                               // +0x34
    CCamera() : HeightLocked(false), ViewVecZLocked(false) {}
};

struct CSavedInputEvent {
    unsigned long Frame;                                // +0x00
    CInputEvent   InputEvent;                           // +0x04
    C2DVector     MousePos;                             // +0x38
    CCamera       Camera;                               // +0x40
};

struct CMouseDevice {
    virtual void v00();
    virtual void GetPosition(C2DVector* position);      // slot 1
};
struct CLoader {
    unsigned char _pad[0x24];
    CMouseDevice* m_pMouse;                             // +0x24
};
extern CLoader* g_Loader;                               // 0x013B8388

class CMainGameComponent { public:
    unsigned long GetFrame();                           // 0x00416225
};

class CDiskFileWin32 { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* buffer, unsigned long size, long flags);   // slot 4 (+0x10)
    unsigned char _body[0x18];
};

class CGamePlayerInterface { public:
    unsigned char       _pad[0x6f8];
    CMainGameComponent* Component;                      // +0x6f8
    void*               PlayerManager;                  // +0x6fc
    bool                PacketSave;                     // +0x700
    bool                PacketLoad;                     // +0x701
    unsigned char       _pad702[2];
    CDiskFileWin32      PacketFile;                     // +0x704
    void SavePacketEvent(const CInputEvent& event);
};

void CGamePlayerInterface::SavePacketEvent(const CInputEvent& event)
{
    CSavedInputEvent saved;
    saved.InputEvent = event;
    saved.Frame = Component->GetFrame();
    CMouseDevice* mouse = g_Loader->m_pMouse;
    if (mouse)
        mouse->GetPosition(&saved.MousePos);
    PacketFile.Write(&saved, sizeof(saved), 0);
}