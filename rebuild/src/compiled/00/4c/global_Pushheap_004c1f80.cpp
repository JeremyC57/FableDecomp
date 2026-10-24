// _global::_Push_heap(NGraphicAppearance::CMeshInfo*, long, long, NGraphicAppearance::CMeshInfo, CSortPrimaryMeshesFirst) -- heap sift-up of 12-byte mesh infos with primary meshes ordered first
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C1F80, 100 bytes, `ret 0x14`.  No callees.
// Called by the landed _Adjust_heap @ 0x004C2C70 (same instantiation; its model is reused here).
// Element: Ego_r.pdb NGraphicAppearance::CMeshInfo (0x0C) = long Index +0, float SizeX +4,
// uchar PrimaryMesh:1 +8.  CSortPrimaryMeshesFirst reads only its FIRST argument:
// `test byte ptr [parent+8],1 ; je` = pred(a, b) = a.PrimaryMesh.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 12-byte record
// by value and the empty comparator (4 + 12 + 4 = 0x14).  Element moves are three dword copies
// (implicit operator=: the bitfield word moves whole); the drop stores the three value words.
// Label note: egor_pdb_names maps 0x004C1F80 to CTCGraphicAppearance::GetDummyObjectAssumingObjectAt,
// which does not fit these bytes (a debug-build address collision).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace N004c1f80 {
struct CMeshInfo {
    long Index;                                 // +0x0
    float SizeX;                                // +0x4
    unsigned char PrimaryMesh : 1;              // +0x8
};
struct CSortPrimaryMeshesFirst {
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};

void __fastcall _Push_heap(CMeshInfo* first, long hole, long top, CMeshInfo value, CSortPrimaryMeshesFirst comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}