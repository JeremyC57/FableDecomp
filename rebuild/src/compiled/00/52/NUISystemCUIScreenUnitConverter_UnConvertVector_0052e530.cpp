// NUISystem::CUIScreenUnitConverter::UnConvertVector(C2DVector v) -- convert a back-buffer pixel vector back to UI-space (640x480 design) units when relative coordinates are on: x / back-buffer width * 640, y / back-buffer height * 480; otherwise unchanged
// Leaf: ?UnConvertVector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E530, 80 bytes, `ret 8` (static __fastcall: the hidden
//   C2DVector result in ecx, v by value on the stack).
// Callers: 0x0052EA6B, 0x0052EB2B (NUISystem::CComponent, same TU).
// Label correction: catalogued as "ConvertX" (PDB ConvertX is float -> float, `SIMM`); it is the inverse of
//   ConvertVector 0x0052E580 (the divisor and multiplier swapped), Ego_r.pdb
//   `?UnConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z`.
// Data: CUIScreenUnitConverter::IsUsingRelativeCoords 0x013B8768, BackBufferWidth / BackBufferHeight 0x013B876C /
//   0x013B8770 (landed SetIsUsingRelativeCoords 0x004299A8), ScreenWidth / ScreenHeight 0x01375CD4 / 0x01375CD8 (.data
//   640.0f / 480.0f; landed UpdateZoom 0x0052F5C0).
// Shape: the result starts as a copy of v (x stays in st(0), y in the frame word) and is overwritten in the flag arm;
//   each axis is a divide statement then a `*=` statement: the one-expression `v.X / BackBufferWidth * ScreenWidth` lets VC7.1
//   reassociate to `(ScreenWidth / BackBufferWidth) * v.X` (globals first), and /Op adds 2 bytes. Twin of ConvertVector 0x0052E580.
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
    static C2DVector __fastcall UnConvertVector(C2DVector v);
};

C2DVector __fastcall CUIScreenUnitConverter::UnConvertVector(C2DVector v)
{
    C2DVector result = v;
    if (IsUsingRelativeCoords) {
        result.X = v.X / BackBufferWidth;
        result.X *= ScreenWidth;
        result.Y = v.Y / BackBufferHeight;
        result.Y *= ScreenHeight;
    }
    return result;
}
}