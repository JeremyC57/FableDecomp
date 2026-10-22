// NUISystem::CComponent::UpdateZoom(float deltaTime) -- advance the zoom transition (eased from the initial to the target zoom over ZoomTime, snapping to the target at the end), then rebuild the render zoom and relative render zoom (own zoom, scaled by the parent zooms unless parent zoom is ignored, screen-converted for a screen-zoomed component) and apply the UI scale on a root or zoom-ignoring component
// Leaf: UpdateZoom
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F5C0, 826 bytes (832 with padding), `ret 4`.
// Callees: the component's virtual slots 103 (+0x19c, zoom ignored) and 116 (+0x1d0, zoom screen-converted; slot names
//   are descriptive), CUIScreenUnitConverter::ConvertVector 0x0052E580 (`__fastcall`, C2DVector by value, hidden result),
//   NUISystem::CManager::GetInstance 0x0041E5F2 (manifest "CFrontEndManager GetInstance") and GetUIScale 0x0041CF47
//   (landed; C2DVector by hidden pointer), the inline `pow(float, int)` of <math.h> (_Pow_int, twice).
//   Data: CUIScreenUnitConverter::IsUsingRelativeCoords 0x013B8768, BackBufferWidth/Height 0x013B876C/0x013B8770 (landed
//   0x004299A8 names), the 640 x 480 screen size 0x01375CD4/0x01375CD8; floats -2.0f 0x01246050, 1.0f 0x0122DED8,
//   0.5f 0x0122F59C.
// Layout: local model (no engine header) -- Ego_r.pdb NUISystem::CComponent less the 4-byte smaller retail States map up
//   to the vectors and 4 bytes per retail STLport vector after them: Zoom +0x5c, TargetZoom +0x64, InitialZoom +0x6c,
//   ParentZoom +0x74, RenderZoom +0x7c, ZoomTimeElapsed +0xa0, ZoomTime +0xa4, PParent +0xc8, RelativeRenderZoom +0x108,
//   RelativeParentZoom +0x110 (consistent with AddPositionChild 0x00531BA0 in this TU).
// Shape: the eased zoom is s = a t^2 / 2 + v t with a = -2 (target - initial) / T^2 and v = 2 (target - initial) / T;
//   acceleration and velocity are declared first and then assigned (the operator results are built in the shared
//   [esp+4] temporary and copied with integer moves), operator/(float) is `*this * (1.0f / f)`, and the sum is written
//   `a * 0.5f * pow(t, 2) + v * t + initial` (member operators evaluate their argument first: v t, then pow, then a/2).
//   The elapsed time stays on the x87 stack for the end test. The render zooms are written per component (no temporary)
//   with the zoom-ignored test inverted (`if (!IsZoomIgnored())` arm first); the screen conversion of the relative render
//   zoom is the converter's inline per-axis form there and the out-of-line ConvertVector in the zoom-ignored arm.
#include <math.h>
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    C2DVector& operator*=(const C2DVector& o) { X *= o.X; Y *= o.Y; return *this; }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
    C2DVector operator/(float f) const { return *this * (1.0f / f); }
};

namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static bool      IsUsingRelativeCoords;             // 0x013B8768
    static float     BackBufferWidth;                   // 0x013B876C
    static float     BackBufferHeight;                  // 0x013B8770
    static float     ScreenWidth;                       // 0x01375CD4 (640)
    static float     ScreenHeight;                      // 0x01375CD8 (480)
    static C2DVector __fastcall ConvertVector(C2DVector v);   // 0x0052E580
    static float ConvertWidth(float x) { if (IsUsingRelativeCoords) return x / ScreenWidth * BackBufferWidth; return x; }
    static float ConvertHeight(float y) { if (IsUsingRelativeCoords) return y / ScreenHeight * BackBufferHeight; return y; }
};
class CManager {
public:
    static CManager* GetInstance();                     // 0x0041E5F2
    C2DVector GetUIScale();                             // 0x0041CF47
};

class CComponent {
public:
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void SetParentPosition(const C2DVector& position);      // slot 18 (+0x48)
    virtual void VSlot19();
    virtual void SetParentZoom(const C2DVector& zoom);              // slot 20 (+0x50)
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual void VSlot65();
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual void VSlot82();
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual bool IsPositionParentRelative() const;                  // slot 102 (+0x198)
    virtual bool IsZoomIgnored() const;                             // slot 103 (+0x19c)
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual void VSlot108();
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void SetRelativeParentPosition(const C2DVector& position);   // slot 114 (+0x1c8)
    virtual void SetRelativeParentZoom(const C2DVector& zoom);      // slot 115 (+0x1cc)
    virtual bool IsZoomScreenConverted() const;                     // slot 116 (+0x1d0)
    virtual bool IsScreenConverted() const;                         // slot 117 (+0x1d4)
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void SetPositionParent(CComponent* parent);             // slot 125 (+0x1f4)
    unsigned char _pad04[0x5c - 0x04];                      // bases, Name, States, Time, the position vectors
    C2DVector   Zoom;                                       // +0x5c
    C2DVector   TargetZoom;                                 // +0x64
    C2DVector   InitialZoom;                                // +0x6c
    C2DVector   ParentZoom;                                 // +0x74
    C2DVector   RenderZoom;                                 // +0x7c
    unsigned char _pad84[0xa0 - 0x84];                      // colours, position times
    float       ZoomTimeElapsed;                            // +0xa0
    float       ZoomTime;                                   // +0xa4
    unsigned char _pada8[0xc8 - 0xa8];                      // colour times, Children, ChildrenToDelete
    CComponent* PParent;                                    // +0xc8
    unsigned char _padcc[0x108 - 0xcc];
    C2DVector   RelativeRenderZoom;                         // +0x108
    C2DVector   RelativeParentZoom;                         // +0x110
    unsigned char _pad118[0x134 - 0x118];
    void UpdateZoom(float deltaTime);
};

void CComponent::UpdateZoom(float deltaTime)
{
    if (ZoomTimeElapsed < ZoomTime) {
        ZoomTimeElapsed += deltaTime;
        C2DVector acceleration;
        C2DVector velocity;
        acceleration = (TargetZoom - InitialZoom) * -2.0f / pow(ZoomTime, 2);
        velocity = (TargetZoom - InitialZoom) * 2.0f / ZoomTime;
        Zoom = acceleration * 0.5f * pow(ZoomTimeElapsed, 2) + velocity * ZoomTimeElapsed + InitialZoom;
        if (ZoomTimeElapsed >= ZoomTime) {
            Zoom = TargetZoom;
            InitialZoom = Zoom;
        }
    }
    if (!IsZoomIgnored()) {
        RenderZoom.X = Zoom.X * ParentZoom.X;
        RenderZoom.Y = Zoom.Y * ParentZoom.Y;
        if (IsZoomScreenConverted()) {
            RelativeRenderZoom.X = CUIScreenUnitConverter::ConvertWidth(Zoom.X) * RelativeParentZoom.X;
            RelativeRenderZoom.Y = CUIScreenUnitConverter::ConvertHeight(Zoom.Y) * RelativeParentZoom.Y;
        } else {
            RelativeRenderZoom.X = Zoom.X * RelativeParentZoom.X;
            RelativeRenderZoom.Y = Zoom.Y * RelativeParentZoom.Y;
        }
    } else {
        RenderZoom = Zoom;
        if (IsZoomScreenConverted())
            RelativeRenderZoom = CUIScreenUnitConverter::ConvertVector(Zoom);
        else
            RelativeRenderZoom = Zoom;
    }
    if (!PParent || IsZoomIgnored()) {
        RelativeRenderZoom.X *= CManager::GetInstance()->GetUIScale().X;
        RelativeRenderZoom.Y *= CManager::GetInstance()->GetUIScale().Y;
        RenderZoom.X *= CManager::GetInstance()->GetUIScale().X;
        RenderZoom.Y *= CManager::GetInstance()->GetUIScale().Y;
    }
}
}