// NUISystem::CMouseCursor::ProcessEvent(EEvent event) -- on the mouse-move event apply the front end's mouse movement to the cursor, clamp the cursor to the render target (0 .. width / height), and hand the movement actually made back to the front end
// Grade: DIFFER(276v276) -- frame-slot packing only: VC7.1 (RTM and QFE) overlays the four GetRenderTargetDimensions result temporaries and the movement local in one 8-byte slot (sub esp,10h); retail gives each its own slot (sub esp,30h: old +4, movement +0xC, temporaries +0x14/+0x1C/+0x24/+0x2C); every instruction otherwise matches
// Leaf: ?ProcessEvent@CMouseCursor@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C6F0, 276 bytes (+ int3 padding), `ret 4`. Slot 1 of vtable 0x0124C208, the
//   CMouseCursor vtable of the event-observer base at +4 (rtti-vtables.tsv; Ego_r.pdb names
//   ?ProcessEvent@CMouseCursor@NUISystem@@UAEXW4EEvent@2@@Z), so `this` arrives adjusted (esi = object + 4, the primary
//   vtable at [esi-4]) as in the landed CZoomingComponent::ProcessEvent 0x005467F0.
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`, its MouseMovement C2DVector at +0xB0 as the
//   landed CInputProcess*::ProcessInput rows), the component's primary slot 32 (+0x80) ChangePositionDelta
//   (const C2DVector&, float, bool) @ 0x0052EA30 (slot 31 is ChangePosition 0x0052E9C0), GFGetSystemManager 0x009A4EC0
//   (`__fastcall`; the display manager at +0x60) and CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (C2DExtentsI
//   returned through a hidden pointer: one 8-byte frame temporary per call). 0x0122DEDC is the 0.0f pool constant.
// Layout: retail CComponent Position at +0x34 (the landed ChangePosition 0x0052E9C0 / CMouseCursor::Update 0x0055C580).
// Shape: the previous position is a POD C2DVector copy (integer moves) into a frame local; each clamp re-fetches the
//   dimensions (`fild` of the int extents, `test ah,41h ; jp` = `width <= x`, `test ah,5 ; jp` = `x < 0.0f`); the
//   movement made is built in a frame temporary before the second GetInstance call and copied through ecx / edx.
struct C2DVector {
    float X;
    float Y;
};

struct C2DExtentsI {
    long Width;
    long Height;
};

class CDisplayManager {
public:
    C2DExtentsI GetRenderTargetDimensions() const;      // 0x009BEDC0
};

class CSystemManager {
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;                    // +0x60
};
extern CSystemManager* __fastcall GFGetSystemManager(); // 0x009A4EC0

inline CDisplayManager* GFGetDisplayManager()
{
    return GFGetSystemManager()->DisplayManager;
}

namespace NUISystem {

enum EEvent { EVENT_MOUSE_MOVED = 0x19 };

class CFrontEndManager {
public:
    unsigned char _pad00[0xb0];
    C2DVector     MouseMovement;                        // +0xB0
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};

class CComponentInterface {                             // primary vtable at +0
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030();
    virtual void ChangePosition(const C2DVector& position, float time, bool immediate);      // slot 31 (+0x7C)
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool immediate);    // slot 32 (+0x80)
};

class CObserver_Retail {                                // vtable at +4
public:
    virtual void v0();
    virtual void ProcessEvent(EEvent event);            // slot 1
};

class CMouseCursor : public CComponentInterface, public CObserver_Retail {
public:
    unsigned char _pad08[0x34 - 8];
    C2DVector     Position;                             // +0x34
    virtual void ProcessEvent(EEvent event);
};

void CMouseCursor::ProcessEvent(EEvent event)
{
    if (event == EVENT_MOUSE_MOVED) {
        C2DVector oldPosition = Position;
        CFrontEndManager* frontEnd = CFrontEndManager::GetInstance();
        ChangePositionDelta(frontEnd->MouseMovement, 0.0f, false);
        if (GFGetDisplayManager()->GetRenderTargetDimensions().Width <= Position.X)
            Position.X = (float)GFGetDisplayManager()->GetRenderTargetDimensions().Width;
        if (Position.X < 0.0f)
            Position.X = 0.0f;
        if (GFGetDisplayManager()->GetRenderTargetDimensions().Height <= Position.Y)
            Position.Y = (float)GFGetDisplayManager()->GetRenderTargetDimensions().Height;
        if (Position.Y < 0.0f)
            Position.Y = 0.0f;
        C2DVector movement;
        movement.X = Position.X - oldPosition.X;
        movement.Y = Position.Y - oldPosition.Y;
        CFrontEndManager::GetInstance()->MouseMovement = movement;
    }
}
}
