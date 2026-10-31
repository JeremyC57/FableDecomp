// CTCGraphicAppearance::GetFocalPos(float time) const -- the "focal" helper point, else the second focal helper, else the parent thing's interpolated position
// Grade: DIFFER(126v126) sec1b: 8 difflines (was 10). The fallback copy now loads all three dwords before the stores (user operator= over long locals declared Z, Y, X) but VC7.1 loads X last into eax (retail X->ecx first, Z->eax); the second GetHelperPoint out-pointer lea is still ecx (retail eax). Tried: && vs ok-flag chain, early returns, Set(), memberwise / float-local / long-local operator= in 3 orders, reference and named temporaries. Next: a 3-dword struct copy through a by-value helper. Earlier: DIFFER(126v126) -- scheduling only: the second GetHelperPoint out-pointer lea uses ecx (retail eax), and the fallback copy of the __fastcall result interleaves load/store pairs where retail loads all three dwords first (tried nested-if, early-return, Set(x,y,z), copy-ctor temp) | retry batch 5: not retried: the untried idea needs the real GetHelperPoint body (0x4C0560, 128+ bytes, not landed) above the caller; blocked until that row lands
// Leaf: GetFocalPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C1060, length 126 bytes (+2 int3 padding), `ret 8` (hidden result + float).
// Catalog name "CTCGraphicAppearance::GetHelperPointOr" (descriptive); Ego_r.pdb / FableWin.pdb name the member
// ?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z, and GetCameraFocalPos (0x004C12D0) calls it.
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector&, float) const @ 0x004C0560
// (twice, with the helper-name globals 0x013BAC48 / 0x013BAC50), the `__fastcall` interpolated-position helper @ 0x004BBF00
// (manifest "Mesh_InterpolatePosition": ecx = hidden result, edx = thing, float on the stack; it reads the thing's +0x60 physics).
// Shape: the result local is plain (empty default ctor); the fallback is the implicit dword-copy assignment; the return copy
// is C3DVector's user memberwise copy ctor (`fld/fstp` for X, integer moves for Y/Z).
// Layout: CTCBase ParentThing +0x04.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector& operator=(const C3DVector& o) { long z = *(long*)&o.Z, y = *(long*)&o.Y, x = *(long*)&o.X; *(long*)&X = x; *(long*)&Y = y; *(long*)&Z = z; return *this; }
};
class CCharString { public: void* PStringData; };
class CThing;
extern const CCharString g_helperFocal_013bac48;      // 0x013BAC48
extern const CCharString g_helperFocal2_013bac50;     // 0x013BAC50
C3DVector __fastcall GetThingInterpolatedPosition(const CThing* thing, float time);   // 0x004BBF00

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& out, float time) const;   // 0x004C0560
    C3DVector GetFocalPos(float time) const;
};

C3DVector CTCGraphicAppearance::GetFocalPos(float time) const
{
    C3DVector pos;
    bool ok = GetHelperPoint(g_helperFocal_013bac48, 0, 0, pos, time);
    if (!ok) ok = GetHelperPoint(g_helperFocal2_013bac50, 0, 0, pos, time);
    if (!ok) pos = GetThingInterpolatedPosition(ParentThing, time);
    return pos;
}
}
