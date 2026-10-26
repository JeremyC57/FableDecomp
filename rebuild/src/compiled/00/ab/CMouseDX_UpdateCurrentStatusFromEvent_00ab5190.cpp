// CMouseDX::UpdateCurrentStatusFromEvent(const CMouseEvent* event) -- apply one buffered mouse event: button down/up flags, relative X/Y movement clamped to the back buffer, or an absolute position
// Leaf: UpdateCurrentStatusFromEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB5190, length 347 bytes + an 11-entry jump table @ 0x00AB52F0, `ret 4`.
// Ego_r.pdb: `?UpdateCurrentStatusFromEvent@CMouseDX@@IAEXPBVCMouseEvent@@@Z`.
// Callees: GFGetSystemManager @ 0x009A4EC0 (display manager at +0x60, whose back-buffer CSurface sits at +0x2c),
//          CSurface::GetDimensions() const @ 0x009F2F90 (C2DExtentsI by hidden pointer).
// Globals: 0.0f @ 0x0122DEDC.
// Layout: CMouseEvent (PDB, 0x18): Time (double) +0, Type +8, Movement +0xc, PosX +0x10, PosY +0x14. CMouseDX retail sits 8 bytes
//   below the PDB (as in the landed PeekScreenPos 0x00AB4870): Pos +0x3414, LMBOn +0x4848, MMBOn +0x4849, RMBOn +0x484a.
// Shape: the surface extents are fetched before the switch for every event; the movement cases clamp through a float local
//   (`fst` into the local and the member), the upper bound is (extent - 1) converted with fild through the dead event slot.
class C2DVector { public:
    float X;
    float Y;
};

class C2DExtentsI { public:
    long Width;
    long Height;
};

class CSurface { public:
    C2DExtentsI GetDimensions() const;              // 0x009F2F90
};

class CDisplayManager { public:
    unsigned char _pad00[0x2c];
    CSurface      BackBuffer;                       // +0x2c
};

class CSystemManager { public:
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;                // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();    // 0x009A4EC0

enum EMouseEventType {
    MOUSE_EVENT_LMB_DOWN = 1,
    MOUSE_EVENT_RMB_DOWN = 2,
    MOUSE_EVENT_MMB_DOWN = 3,
    MOUSE_EVENT_LMB_UP = 4,
    MOUSE_EVENT_RMB_UP = 5,
    MOUSE_EVENT_MMB_UP = 6,
    MOUSE_EVENT_MOVE_X = 7,
    MOUSE_EVENT_MOVE_Y = 8,
    MOUSE_EVENT_WHEEL_UP = 9,
    MOUSE_EVENT_WHEEL_DOWN = 10,
    MOUSE_EVENT_SET_POSITION = 11
};

class CMouseEvent { public:
    double          Time;                           // +0x00
    EMouseEventType Type;                           // +0x08
    float           Movement;                       // +0x0c
    float           PosX;                           // +0x10
    float           PosY;                           // +0x14
};

class CMouseDX {
public:
    virtual ~CMouseDX();
    unsigned char _pad04[0x3410];
    C2DVector     Pos;                              // +0x3414
    unsigned char _pad341c[0x142c];
    bool          LMBOn;                            // +0x4848
    bool          MMBOn;                            // +0x4849
    bool          RMBOn;                            // +0x484a

    void UpdateCurrentStatusFromEvent(const CMouseEvent* event);
};

void CMouseDX::UpdateCurrentStatusFromEvent(const CMouseEvent* event)
{
    CSystemManager* system = GFGetSystemManager();
    C2DExtentsI extents = system->DisplayManager->BackBuffer.GetDimensions();
    switch (event->Type) {
    case MOUSE_EVENT_MOVE_X: {
        float x = event->Movement + Pos.X;
        Pos.X = x;
        if (x < 0.0f)
            Pos.X = 0.0f;
        else if (x > extents.Width - 1)
            Pos.X = (float)(extents.Width - 1);
        break;
    }
    case MOUSE_EVENT_MOVE_Y: {
        float y = event->Movement + Pos.Y;
        Pos.Y = y;
        if (y < 0.0f)
            Pos.Y = 0.0f;
        else if (y > extents.Height - 1)
            Pos.Y = (float)(extents.Height - 1);
        break;
    }
    case MOUSE_EVENT_SET_POSITION:
        Pos.X = event->PosX;
        Pos.Y = event->PosY;
        break;
    case MOUSE_EVENT_LMB_DOWN:
        LMBOn = true;
        break;
    case MOUSE_EVENT_LMB_UP:
        LMBOn = false;
        break;
    case MOUSE_EVENT_RMB_DOWN:
        RMBOn = true;
        break;
    case MOUSE_EVENT_RMB_UP:
        RMBOn = false;
        break;
    case MOUSE_EVENT_MMB_DOWN:
        MMBOn = true;
        break;
    case MOUSE_EVENT_MMB_UP:
        MMBOn = false;
        break;
    default:
        break;
    }
}