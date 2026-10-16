#pragma optimize("y", off)
#pragma optimize("s", on)
// CUIState::CUIState(const CUIState& other) -- copy constructor: the change flag/type, position and zoom words, the colour through CRGBColour's component-wise copy, the update time and the linear-change flag
// Leaf: ??0CUIState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00429E29, length 97 bytes, `ret 4`; frame-pointer size TU
// (`push ebp ; mov ebp,esp ; push ecx`): `optimize("y",off)` + `("s",on)`. No callees.
// Layout (Ego_r.pdb CUIState, 0x24 bytes): StateChangeFlag +0x00, StateChangeType +0x04, Position (C2DVector) +0x08,
// Zoom +0x10, Colour (CRGBColour, D3DCOLOR byte order B,G,R,A) +0x18, UpdateTime +0x1c, LinearChange +0x20.
// Shape: CRGBColour's user copy ctor forwards the four components to an inline Set(r, g, b, a): the arguments load
// right to left (A, B, G, R -- G is parked in the top byte of the consumed argument slot, B in the frame word) and
// the body stores R, G, B, A (+2/+1/+0/+3); both are `__forceinline` (the size TU emits them out of line otherwise).
struct C2DVector { float X, Y; };
struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& c) { Set(c.R, c.G, c.B, c.A); }
};
struct CUIState {
    unsigned long StateChangeFlag;                 // +0x00
    long          StateChangeType;                 // +0x04 EStateChangeType
    C2DVector     Position;                        // +0x08
    C2DVector     Zoom;                            // +0x10
    CRGBColour    Colour;                          // +0x18
    float         UpdateTime;                      // +0x1c
    bool          LinearChange;                    // +0x20
    CUIState(const CUIState& other);
};
CUIState::CUIState(const CUIState& other)
    : StateChangeFlag(other.StateChangeFlag), StateChangeType(other.StateChangeType), Position(other.Position),
      Zoom(other.Zoom), Colour(other.Colour), UpdateTime(other.UpdateTime), LinearChange(other.LinearChange)
{
}