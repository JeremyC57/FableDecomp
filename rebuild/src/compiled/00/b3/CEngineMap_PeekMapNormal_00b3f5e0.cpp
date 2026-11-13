// CEngineMap::PeekMapNormal(long x, long y) const -- the landscape normal at a cell from central differences of the quantised heights of its four neighbours: the x and y slopes over two cells, each a 2D vector (height difference, 2) normalised, combined into (dx.X, dy.X, dx.Y * dy.Y) and fast-normalised
// Leaf: ?PeekMapNormal@CEngineMap@@QBE?AVC3DVector@@JJ@Z
// Flags: /d2inlT444
// Inline-budget: /d2inlT444 -- the four expanded PeekLandscapeHeight sites (inline trace, c2.dll 0x1078D1AA: c1 345, f=1, score 443
//   each): retail expands all four (0x00B3F5E0.., 0x00B3F75B.., 0x00B3F8BF.., 0x00B3FA1C..), T<=443 calls them; straight-line sites
//   cannot rise above f=1. The same TU's 0x00B40BD0 needs T576 and this row matches at T576 too.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3F5E0, 1624 bytes (0x00B3F5E0..0x00B3FC37, `ret 0Ch` at 0x00B3FC35; the next function,
//   CEngineMap::PeekInterpolatedMapNormal 0x00B3FC40, follows after int3 padding), thiscall, hidden C3DVector result + two longs.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?PeekMapNormal@CEngineMap@@QBE?AVC3DVector@@JJ@Z (Ego_r donor order: after
//   ReadMapInfoBlock and before PeekInterpolatedMapNormal, as retail); its only caller is PeekInterpolatedMapNormal 0x00B3FC40
//   (four calls 0x00B3FCCD..0x00B3FCFE: the corner normals it blends).
// Callees: the header inline CEngineMap::PeekLandscapeHeight (Ego_r / FableWin ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z) expanded
//   four times, (x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1); its out-of-line COMDAT copy 0x00B439C0 (this TU's inline run) is
//   called only for a cell held by the neighbouring map (the inline self call). Inside it: g_pEngineWorldMap 0x01436E8C
//   (CEngineWorldMap::GetMap(x, y)), the game map's CIDrawMap::DrawBlockGetSizeZAt (slot 5, `call [edx+14h]`) and the private
//   inline QuantiseLandscapeHeight (x 128, round, / 128). Then ?Normalise@C2DVector@@QAEAAV1@XZ 0x00A14540 (out of line, twice)
//   and the inline C3DVector::Normalise (GFFastOneOverSqrt: exponent 0x17C, mantissa table GFastOneOverSqrtTable 0x013CE558,
//   one Newton step with 3.0f / 0.5f).
// Constants: 128.0f __real@43000000 (0x012571B0), 1/128 __real@3c000000 (0x012A1F34), 0.0f (0x0122DEDC), 3.0f (0x0122DED4),
//   0.5f (0x0122F59C); the 2.0f slope run is the immediate 0x40000000.
// Shape: matched on the first grade with the shared header's PeekLandscapeHeight body (hdr/engine/CEngineMap.h
//   FABLE_RETAIL_CENGINEMAP_PEEK_BODIES, plain inline, g_pEngineWorldMap read at each use: the first copy keeps the world map in
//   edx from the prologue, 0x00B3F5E7); 3 grades in all (forced-inline bodies, then plain inline at T444 and at T576). The fourth
//   height stays on the x87 stack for `fsub st(1)` (0x00B3FB79); the normal's X and Y are read straight from the two 2D vectors
//   and only its Z is stored (0x00B3FB9E).
// Retail-asm: 0x00B3F735 `fld dword ptr [esp+20h]` + 0x00B3F739 `fistp dword ptr [esp+1Ch]`, 0x00B3F899 / 0x00B3F89D (same),
//   0x00B3F9F6 `fld dword ptr [esp+4Ch]` + 0x00B3F9FA `fistp dword ptr [esp+20h]`, 0x00B3FB3B `fld dword ptr [esp+4Ch]` +
//   0x00B3FB3F `fistp dword ptr [esp+48h]`: the four expanded QuantiseLandscapeHeight roundings, the shared
//   engine/GFFloatToLongNear.h helper (its header proof rules out every plain C++ / intrinsic spelling at a bare dword fistp:
//   `(long)x` / `(int)x` give __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 gives cvttss2si; VC7.1 RTM has no x87 fistp
//   intrinsic). The out-of-line copy's site is 0x00B43B00 (row 0x00B439C0).
#define FABLE_RETAIL_C3DVECTOR
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
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"

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