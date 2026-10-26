// CUIState::CUIState(const CUIStateDef& def) -- copy a UI state from its def: position, zoom, the float colour scaled to bytes, update time, change type/flag and the linear-change flag
// Retail-asm: 0x0052F46A `fstp [esp+14h] ; fld [esp+14h] ; fistp dword ptr [esp]` (x4) -- the engine's inline-assembly ROUND helper: each scaled channel is homed in the dead `def` argument slot and reloaded for a bare dword fistp; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` without the reload.
// Leaf: ??0CUIState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052F440, length 183 bytes (`ret 4`; 192 with padding). Constant: 0x01230014 = 255.0f.
// Layout: Ego_r.pdb CUIState (StateChangeFlag +0, StateChangeType +4, Position +8, Zoom +0x10, Colour +0x18, UpdateTime
// +0x1c, LinearChange +0x20); CUIStateDef retail = PDB - 4 (- 8 after its 12-byte vector): StateChangeFlag +0x40,
// StateChangeType +0x44, Position +0x48, Zoom +0x50, Colour (CRGBFloatColour) +0x58, UpdateTime +0x68, LinearChange +0x78.
// Shape: body assignments in retail store order; the colour is built B,G,R,A from the four rounded channels.
class C2DVector { public: float x, y; };
class CRGBFloatColour { public: float r, g, b, a; };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
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
// the engine's inline-assembly float-to-long helper, expanded in place (the by-value parameter is homed in a dead slot)
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
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
    long r = ROUND(def.Colour.r * 255.0f);
    long g = ROUND(def.Colour.g * 255.0f);
    long b = ROUND(def.Colour.b * 255.0f);
    long a = ROUND(def.Colour.a * 255.0f);
    Colour = CRGBColour((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
    UpdateTime = def.UpdateTime;
    StateChangeType = def.StateChangeType;
    StateChangeFlag = def.StateChangeFlag;
    LinearChange = def.LinearChange;
}