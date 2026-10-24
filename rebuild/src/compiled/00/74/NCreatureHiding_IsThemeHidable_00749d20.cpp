// NCreatureHiding::IsThemeHidable(const CWorldMap& map, const C3DVector& pos) -- a position is hidable when its world-map cell is not passable
// Leaf: ?IsThemeHidable@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749D20, 55 bytes (+9 int3), `ret` (__fastcall: map in ecx, pos in edx).
// Callees: __ftol2 @ 0x00BFEA70 (x2), CWorldMap::IsPosPassable(const C2DCoordI&) const @ 0x004FCBD0 (landed).
// Callers: none by direct call or absolute reference in the retail image (xref.py).
// Label: the manifest's "IsThemeHidable" is right: Ego_r.pdb names `?IsThemeHidable@NCreatureHiding@@YI_NABVCWorldMap@@
//   ABVC3DVector@@@Z` (bool __fastcall NCreatureHiding::IsThemeHidable(const CWorldMap&, const C3DVector&)).
// Shape: `return !map.IsPosPassable(C2DCoordI((long)pos.X, (long)pos.Y));` -- the coordinate is built in an 8-byte frame
//   (X first), and the negated bool comes back as `neg al ; sbb eax,eax ; inc eax`.
class C3DVector {
public:
    float X, Y, Z;
};

class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};

class CWorldMap {
public:
    bool IsPosPassable(const C2DCoordI& pos) const;          // 0x004FCBD0
};

namespace NCreatureHiding {

bool __fastcall IsThemeHidable(const CWorldMap& map, const C3DVector& pos)
{
    return !map.IsPosPassable(C2DCoordI((long)pos.X, (long)pos.Y));
}

}