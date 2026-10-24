// NUISystem::CUIScreenUnitConverter::ConvertVector(C2DVector v) -- convert a UI-space (640x480 design) vector to back-buffer pixels when relative coordinates are on: x / 640 * back-buffer width, y / 480 * back-buffer height; otherwise unchanged
// Leaf: ?ConvertVector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E580, 80 bytes, `ret 8` (static __fastcall: the hidden
//   C2DVector result in ecx, v by value on the stack).
// Callers: 20 call sites from 0x0052ED48 to 0x00620F0A, among them the landed NUISystem::CComponent::
//   GetTargetRelativeRenderPosition 0x0052EEC0 (call 0x0052EEF9) and UpdateZoom 0x0052F5C0 (call 0x0052F839).
// Label correction: catalogued as "ConvertX" (PDB ConvertX is float -> float, `SIMM`); this is Ego_r.pdb
//   `?ConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z`, as the landed callers 0x0052EEC0 /
//   0x0052F5C0 already declare at this address.
// Data: CUIScreenUnitConverter::IsUsingRelativeCoords 0x013B8768, BackBufferWidth / BackBufferHeight 0x013B876C /
//   0x013B8770 (landed SetIsUsingRelativeCoords 0x004299A8), ScreenWidth / ScreenHeight 0x01375CD4 / 0x01375CD8 (.data
//   640.0f / 480.0f; landed UpdateZoom 0x0052F5C0).
// Shape: the result starts as a copy of v (x stays in st(0), y in the frame word) and is overwritten in the flag arm;
//   each axis is a divide statement then a `*=` statement: the one-expression `v.X / ScreenWidth * BackBufferWidth` lets VC7.1
//   reassociate to `(BackBufferWidth / ScreenWidth) * v.X` (globals first), and /Op adds 2 bytes. Twin of UnConvertVector 0x0052E530.
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};

namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static bool      IsUsingRelativeCoords;             // 0x013B8768
    static float     BackBufferWidth;                   // 0x013B876C
    static float     BackBufferHeight;                  // 0x013B8770
    static float     ScreenWidth;                       // 0x01375CD4 (640)
    static float     ScreenHeight;                      // 0x01375CD8 (480)
    static C2DVector __fastcall ConvertVector(C2DVector v);
};

C2DVector __fastcall CUIScreenUnitConverter::ConvertVector(C2DVector v)
{
    C2DVector result = v;
    if (IsUsingRelativeCoords) {
        result.X = v.X / ScreenWidth;
        result.X *= BackBufferWidth;
        result.Y = v.Y / ScreenHeight;
        result.Y *= BackBufferHeight;
    }
    return result;
}
}