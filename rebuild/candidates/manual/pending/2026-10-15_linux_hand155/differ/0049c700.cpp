// CThingSearchTools::GetMapRelativeBox(const CThingSearchMap& map, const C2DBoxI& box) const -- clip a cell box to the map's bounds and return it relative to the bounds' top-left corner
// Grade: DIFFER(109v107) -- loads now in retail order, but the named result local is copied out through `mov esi,eax` with edx/edi swapped, where retail stores the four differences straight into the hidden result (+4, +0xc, +0, +8); `return C2DBoxI(...)` built in place loads TLY before TLX. Tried: direct member reads, reference local, C2DVectorI offset local, named result.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049C700, length 107 bytes (`ret 0Ch`: the hidden result pointer plus two
// arguments; `this` is unused).
// Label correction: the catalog's "CRegionMinimap::GetRegionMapExtents" is a propagated name (the real one is the
// landed 0x0066C6A0). Every caller (over 100, e.g. 0x00436DCA / 0x0049C4FA) is a CThingSearchTools::GetPBestThingInRoughArea
// instantiation passing its own `this`, a map object and the search box; the method name is descriptive.
// Callees: C2DBoxI::ClipTo(const C2DBoxI&) @ 0x0049C810.
// Layout: the map object carries a C2DBoxI bounds at +0x20 (the rest is unused here).
// Shape: the box is copied into a local (all four words loaded first), clipped in place, and the result is built in
// the hidden return slot by the inline four-argument C2DBoxI constructor (stores scheduled +4, +0xc, +0, +8).
// Leaf: GetMapRelativeBox
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
    void ClipTo(const C2DBoxI& bounds);             // 0x0049C810
};

struct C2DVectorI {
    long X, Y;
    C2DVectorI(long x, long y) : X(x), Y(y) {}
};

struct CThingSearchMap {
    unsigned char _pad[0x20];
    C2DBoxI       Bounds;                           // +0x20
};

struct CThingSearchTools {
    C2DBoxI GetMapRelativeBox(const CThingSearchMap& map, const C2DBoxI& box) const;
};

C2DBoxI CThingSearchTools::GetMapRelativeBox(const CThingSearchMap& map, const C2DBoxI& box) const
{
    const C2DBoxI& bounds = map.Bounds;
    C2DBoxI clipped = box;
    clipped.ClipTo(bounds);
    C2DBoxI result(clipped.TLX - bounds.TLX, clipped.TLY - bounds.TLY, clipped.BRX - bounds.TLX, clipped.BRY - bounds.TLY);
    return result;
}
