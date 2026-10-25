// NUISystem::CScrollingViewport::ProcessEvent(EEvent event) -- scroll the children down (event 6) or up (event 7) and flag the scroll; on the mouse-move event (0x19) make this viewport the active scrolling viewport when the mouse cursor lies inside its on-screen window, else clear the active viewport
// Grade: DIFFER(587v578) -- evaluation order + frame layout: retail fetches the GetPosition cursor (edi) before the GetSize cursor in each dead far-corner pair (ours fetches both cursors first, the GetSize one first), keeps mouse.X/Y in ebx/ebp (ours ebp + a spill), and lays the hidden-return temporaries out in first-use order in a 0x7C frame (ours 0x8C, scattered); same calls and arithmetic otherwise
// Leaf: ?ProcessEvent@CScrollingViewport@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053E9A0, 576 bytes (+ int3 padding), `ret 4`. Slot 1 of vtable 0x012468AC, the
//   CScrollingViewport vtable of the observer base at +4 (rtti-vtables.tsv; Ego_r.pdb
//   ?ProcessEvent@CScrollingViewport@NUISystem@@UAEXW4EEvent@2@@Z), so `this` arrives adjusted (esi = object + 4) as in
//   the parked CMouseCursor::ProcessEvent 0x0055C6F0.
// Callees: own primary slots 143 (+0x23c, ScrollChildren(bool), landed 0x0053EDA0), 122 (+0x1e8, GetScreenPosition) and
//   123 (+0x1ec, GetScale); NUISystem::CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`), its cursor word +0xB8
//   (as the landed CMouseCursor ctor / dtor) and GetUIScale 0x0041CF47 (C2DVector by hidden pointer); the cursor's
//   slots 16 (+0x40, GetPosition) and 23 (+0x5c, GetSize); __ftol2 0x00BFEA70. Global g_pActiveScrollingViewport
//   0x013B8AAC (as the landed CScrollingViewport rows).
// Layout: as the landed ScrollChildren 0x0053EDA0: Window C2DBoxF +0x178 (primary), the scrolled flag byte +0x190.
// Shape: the on-screen window box is ScrollChildren's float box converted to longs (the left edge spilled across the
//   GetUIScale call, the right / bottom edges add the converted left / top with `fiadd`); the mouse box is left
//   unset without a cursor, its far corner computed but unused (both cursor calls kept); the inside test is an inline
//   half-open box test.
namespace NUISystem {

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& other) const { return C2DVector(X + other.X, Y + other.Y); }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    bool IsInside(long x, long y) const { return x >= TLX && x < BRX && y >= TLY && y < BRY; }
};

enum EEvent { EVENT_SCROLL_DOWN = 6, EVENT_SCROLL_UP = 7, EVENT_MOUSE_MOVED = 0x19 };   // descriptive

class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual C2DVector GetPosition() const;                          // slot 16 (+0x40)
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual C2DVector GetSize();                                    // slot 23 (+0x5c)
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121();
    virtual C2DVector GetScreenPosition();                          // slot 122 (+0x1e8)
    virtual C2DVector GetScale();                                   // slot 123 (+0x1ec)
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142();
    virtual void ScrollChildren(bool up);                           // slot 143 (+0x23c)
};

struct CManager {
    unsigned char _pad00[0xb8];
    CComponent*   MouseCursor;                          // +0xB8
    C2DVector GetUIScale();                             // 0x0041CF47
};
struct CFrontEndManager {
    static CManager* __fastcall GetInstance();         // 0x0041E5F2
};

class CObserver_Retail {                                // vtable at +4
public:
    virtual void v0();
    virtual void ProcessEvent(EEvent event);            // slot 1
};

class CScrollingViewport : public CComponent, public CObserver_Retail {
public:
    unsigned char _pad08[0x178 - 8];
    C2DBoxF       Window;                               // +0x178
    unsigned char _pad188[0x190 - 0x188];
    bool          Scrolled;                             // +0x190 (descriptive)
    virtual void ProcessEvent(EEvent event);
};

extern CScrollingViewport* g_pActiveScrollingViewport;  // 0x013B8AAC

void CScrollingViewport::ProcessEvent(EEvent event)
{
    switch (event) {
    case EVENT_SCROLL_DOWN:
        ScrollChildren(false);
        Scrolled = true;
        break;
    case EVENT_SCROLL_UP:
        ScrollChildren(true);
        Scrolled = true;
        break;
    case EVENT_MOUSE_MOVED: {
        C2DVector screen = GetScreenPosition();
        C2DVector scale = GetScale();
        C2DBoxI mouse;
        if (CFrontEndManager::GetInstance()->MouseCursor) {
            mouse.TLX = (long)CFrontEndManager::GetInstance()->MouseCursor->GetPosition().X;
            mouse.TLY = (long)CFrontEndManager::GetInstance()->MouseCursor->GetPosition().Y;
            mouse.BRX = (long)(CFrontEndManager::GetInstance()->MouseCursor->GetPosition() + CFrontEndManager::GetInstance()->MouseCursor->GetSize()).X;
            mouse.BRY = (long)(CFrontEndManager::GetInstance()->MouseCursor->GetPosition() + CFrontEndManager::GetInstance()->MouseCursor->GetSize()).Y;
        }
        C2DBoxI box;
        float x = Window.TLX;
        box.TLX = (long)(x * CFrontEndManager::GetInstance()->GetUIScale().X + screen.X);
        box.TLY = (long)(Window.TLY * CFrontEndManager::GetInstance()->GetUIScale().Y + screen.Y);
        C2DVector size;
        size.X = Window.BRX - Window.TLX;
        box.BRX = (long)(size.X * CFrontEndManager::GetInstance()->GetUIScale().X + box.TLX);
        size.Y = Window.BRY - Window.TLY;
        box.BRY = (long)(size.Y * CFrontEndManager::GetInstance()->GetUIScale().Y + box.TLY);
        if (box.IsInside(mouse.TLX, mouse.TLY))
            g_pActiveScrollingViewport = this;
        else
            g_pActiveScrollingViewport = 0;
        break;
    }
    }
}

}
