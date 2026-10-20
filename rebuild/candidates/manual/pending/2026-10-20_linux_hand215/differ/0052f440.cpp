// CUIState::CUIState(const CUIStateDef& def) -- copy a UI state from its def: position, zoom, the float colour scaled to bytes, update time, change type/flag and the linear-change flag
// Grade: DIFFER(lane gap: inline-asm float->int) -- retail rounds each colour channel with a bare dword `fistp` after a float store/reload through a dead argument slot (the inline-asm FtoL helper, as the PackVertexNormal family in GOTCHAS); plain C++ can only produce __ftol2 calls
// Leaf: ??0CUIState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052F440, length 183 bytes (`ret 4`; 192 with padding). Constant: 0x01230014 = 255.0f.
// Layout: Ego_r.pdb CUIState (StateChangeFlag +0, StateChangeType +4, Position +8, Zoom +0x10, Colour +0x18, UpdateTime
// +0x1c, LinearChange +0x20); CUIStateDef retail = PDB - 4 (- 8 after its 12-byte vector): StateChangeFlag +0x40,
// StateChangeType +0x44, Position +0x48, Zoom +0x50, Colour (CRGBFloatColour) +0x58, UpdateTime +0x68, LinearChange +0x78.
// Shape: body assignments in retail store order; the colour is built B,G,R,A from the four rounded channels.
struct C2DVector { float x, y; };
struct CRGBFloatColour { float r, g, b, a; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) { }
};
struct CUIStateDef {
    unsigned char   _pad00[0x40];
    unsigned long   StateChangeFlag;        // +0x40
    long            StateChangeType;        // +0x44
    C2DVector       Position;               // +0x48
    C2DVector       Zoom;                   // +0x50
    CRGBFloatColour Colour;                 // +0x58
    float           UpdateTime;             // +0x68
    unsigned char   _pad6c[0x0c];           // ChildrenNotAffected
    bool            LinearChange;           // +0x78
};
struct CUIState {
    unsigned long StateChangeFlag;          // +0x00
    long          StateChangeType;          // +0x04
    C2DVector     Position;                 // +0x08
    C2DVector     Zoom;                     // +0x10
    CRGBColour    Colour;                   // +0x18
    float         UpdateTime;               // +0x1c
    bool          LinearChange;             // +0x20
    CUIState(const CUIStateDef& def);
};

CUIState::CUIState(const CUIStateDef& def)
{
    Position = def.Position;
    Zoom = def.Zoom;
    long r = (long)(def.Colour.r * 255.0f);
    long g = (long)(def.Colour.g * 255.0f);
    long b = (long)(def.Colour.b * 255.0f);
    long a = (long)(def.Colour.a * 255.0f);
    Colour = CRGBColour((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
    UpdateTime = def.UpdateTime;
    StateChangeType = def.StateChangeType;
    StateChangeFlag = def.StateChangeFlag;
    LinearChange = def.LinearChange;
}
