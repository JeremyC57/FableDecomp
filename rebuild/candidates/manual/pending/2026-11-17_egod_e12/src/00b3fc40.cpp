// CEngineMap::PeekInterpolatedMapNormal(float xp, float yp) const -- the landscape normal at a fractional cell position: the four corner cells' map normals blended bilinearly, then normalised
// Leaf: ?PeekInterpolatedMapNormal@CEngineMap@@QBE?AVC3DVector@@MM@Z
// Flags: /d2inlT444
// Inline-budget: /d2inlT444 -- smallest N for the same-TU body CEngineMap::PeekMapNormal defined below: its four PeekLandscapeHeight sites
//   expand only at T>=444 (as landed row 0x00B3F5E0, same flag); with it that body grades RELOCATION_MATCH at 0x00B3F5E0 (1624 bytes).
//   Measured: no flag -- this row RELOCATION_MATCH but the defined helper DIFFER(298v1624); T443 -- the helper is expanded into this row
//   (DIFFER 1403v528); T444 -- both RELOCATION_MATCH.
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00B3FC40, 528 bytes, ebp frame, `ret 0Ch` (hidden C3DVector result + two floats).
// Identity: Ego_r.pdb / FableWin.pdb ?PeekInterpolatedMapNormal@CEngineMap@@QBE?AVC3DVector@@MM@Z; Ego_d.pdb (engine_map.obj, right after
//   PeekMapNormal) gives the parameter names xp / yp and the locals int_xp, int_yp, frac_x, frac_y, normal00, normal10, normal01, normal11,
//   normal (no named weights or partial sums: the blend is one expression).
// Callees: CEngineMap::PeekMapNormal 0x00B3F5E0 (four calls: (x,y) (x+1,y) (x,y+1) (x+1,y+1)), the shared GFFloatToLongFloor helper
//   (expanded twice) and the inline C3DVector::Normalise (GFFastOneOverSqrt table 0x013CE558).
// Shape: PeekMapNormal is the previous function of this object file (Ego_d order, retail 0x00B3F5E0 directly before this row); its
//   landed body is defined here as in retail's TU. With the body visible c2 knows the four results do not escape, which gives retail's
//   frame (0x60) and slot order: the old 29 frame hunks (0x54 frame) closed at the first grade.
// Retail-asm: 0x00B3FC5F..0x00B3FC79 `fld [ebp-10h] ; fsub [ebp-4] ; fistp dword ptr [ebp-8] ; fild [ebp-8] ; fadd [ebp-0Ch] ; fstp [ebp-14h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; inc dword ptr [ebp-8]` (and again 0x00B3FC90..0x00B3FCAA for y) -- the shared engine/GFFloatToLongFloor.h helper expanded twice.
// FITTED: FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_SQUARE_PAREN (no-code text of the Newton step; existing opt-in) -- the default text leaves
//   one x87 / epilogue interleave hunk at the tail (0x00B3FE3A..0x00B3FE44: retail `mov eax,[ebp+8] ; fxch st(2) ; pop edi ; fstp [eax]`);
//   NEWTON_GROUPED / NEWTON_PAREN leave 5 hunks. The defined PeekMapNormal body matches 0x00B3F5E0 under this text too (one text per TU).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_SQUARE_PAREN
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEKNORMAL
#define FABLE_RETAIL_CENGINEMAP_PEEK_BODIES
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#define FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

#include "engine/GFFloatToLongFloor.h"

C3DVector CEngineMap::PeekMapNormal(long x, long y) const
{
    float left = PeekLandscapeHeight(x - 1, y);
    float right = PeekLandscapeHeight(x + 1, y);
    float down = PeekLandscapeHeight(x, y - 1);
    float up = PeekLandscapeHeight(x, y + 1);
    C2DVector dx(left - right, 2.0f);
    C2DVector dy(down - up, 2.0f);
    dx.Normalise();
    dy.Normalise();
    C3DVector normal(dx.X, dy.X, dx.Y * dy.Y);
    normal.Normalise();
    return normal;
}
C3DVector CEngineMap::PeekInterpolatedMapNormal(float xp, float yp) const
{
    long int_xp = GFFloatToLongFloor(xp);
    long int_yp = GFFloatToLongFloor(yp);
    float frac_x = xp - int_xp;
    float frac_y = yp - int_yp;
    C3DVector normal00 = PeekMapNormal(int_xp, int_yp);
    C3DVector normal10 = PeekMapNormal(int_xp + 1, int_yp);
    C3DVector normal01 = PeekMapNormal(int_xp, int_yp + 1);
    C3DVector normal11 = PeekMapNormal(int_xp + 1, int_yp + 1);
    C3DVector normal = normal00 * ((1.0f - frac_x) * (1.0f - frac_y)) + normal10 * (frac_x * (1.0f - frac_y)) + normal01 * ((1.0f - frac_x) * frac_y) + normal11 * (frac_x * frac_y);
    normal.Normalise();
    return normal;
}
