// NUISystem::CDraggable::ProcessEvent(EEvent) -- while dragged, follow the mouse: on the mouse-moved event (0x19) move the component to the mouse position divided by the UI scale; then the clickable handling
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055A510, length 157 bytes; slot 1 of the CDraggable CObserver vtable 0x0124BA70
// (rtti-vtables.tsv), so `this` is the CObserver subobject at +4 (members at -4 from their object offsets).
// Callees: NUISystem::CManager::GetInstance() @ 0x0041E5F2 (static, no arguments), CManager::GetUIScale() @ 0x0041CF47 (C2DVector by
// hidden pointer), the component's ChangePosition(const C2DVector&, float, bool) (primary slot 32, `call [eax+80h]` on this-4),
// NUISystem::CClickable::ProcessEvent(EEvent) @ 0x0055AD60.
// Layout: the retail NUISystem component model of the landed CDraggable constructor 0x0055A9C0 (primary interface at +0, CObserver
// at +4, event-sink interface at +0x18; WasDragged +0x19c). The front-end manager keeps the mouse position (C2DVector) at +0xb0.
// Shape: the event stays in edi for the base call; the mouse position is copied into a frame C2DVector (integer moves), the UI
// scale is fetched twice into two predeclared float locals in ONE comma expression (`scaleX = ...X, scaleY = ...Y;`), so the two
// GetUIScale result temporaries live to the same full-expression end and get separate 8-byte slots (`sub esp,18h`; separate
// statements share one slot) (X homed in the dead event slot, Y kept on the x87) BEFORE
// the two divisions (dividing right after each fetch moves the X division between the calls); ChangePosition gets
// (position, 0.0f, false).
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

namespace NUISystem { class CManager { public:
    unsigned char _pad[0xb0];
    C2DVector     MousePosition;                   // +0xb0
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
    C2DVector GetUIScale();                        // 0x0041CF47 (CManager::GetUIScale)
}; }

namespace NUISystem {

enum EEvent { EVENT_MOUSE_MOVED = 0x19 };

struct CComponentInterface {                       // primary vtable (CDraggable: 0x0124BA94)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void ChangePosition(const C2DVector& position, float time, bool immediate);   // slot 32 (+0x80)
};

struct CObserver_Retail {                          // vtable at +4 (CDraggable: 0x0124BA70)
    virtual void v00();
    virtual void ProcessEvent(EEvent event);       // slot 1
    unsigned char _body[0x10];                     // EventsToObserve (12-byte set), PreventObservation
};

struct CEventSinkInterface {                       // vtable at +0x18
    virtual void v00();
};

class CClickable : public CComponentInterface, public CObserver_Retail, public CEventSinkInterface { public:   // retail 0x194 bytes
    unsigned char _body[0x194 - 0x1c];
    virtual void ProcessEvent(EEvent event);       // 0x0055AD60
};

class CDraggable : public CClickable { public:                   // retail 0x1ac bytes
    C2DVector PositionBeforeDrag;                  // +0x194
    bool      WasDragged;                          // +0x19c
    virtual void ProcessEvent(EEvent event);
};

void CDraggable::ProcessEvent(EEvent event)
{
    if (event == EVENT_MOUSE_MOVED && WasDragged) {
        C2DVector position(NUISystem::CManager::GetInstance()->MousePosition);
        float scaleX, scaleY;
        scaleX = NUISystem::CManager::GetInstance()->GetUIScale().X, scaleY = NUISystem::CManager::GetInstance()->GetUIScale().Y;
        position.X /= scaleX;
        position.Y /= scaleY;
        ChangePosition(position, 0.0f, false);
    }
    CClickable::ProcessEvent(event);
}

}