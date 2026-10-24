// _global::_Push_heap(NGraphicAppearance::CMeshInfo*, long, long, NGraphicAppearance::CMeshInfo, CSortPrimaryMeshesLast) -- heap sift-up of 12-byte mesh infos with primary meshes ordered last
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C2000, 105 bytes, `ret 0x14`.  No callees.
// Called by the landed _Adjust_heap @ 0x004C2D10 (same instantiation; its model is reused here).
// Element: Ego_r.pdb NGraphicAppearance::CMeshInfo (0x0C) = long Index +0, float SizeX +4,
// uchar PrimaryMesh:1 +8.  CSortPrimaryMeshesLast reads only its FIRST argument:
// `mov cl,[parent+8] ; not cl ; test cl,1 ; je` = pred(a, b) = !a.PrimaryMesh.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 12-byte record
// by value and the empty comparator (4 + 12 + 4 = 0x14).  Element moves are three dword copies
// (implicit operator=: the bitfield word moves whole); the drop stores the three value words.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace N004c2000 {
struct CMeshInfo {
    long Index;                                 // +0x0
    float SizeX;                                // +0x4
    unsigned char PrimaryMesh : 1;              // +0x8
};
struct CSortPrimaryMeshesLast {
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return !a.PrimaryMesh; }
};

void __fastcall _Push_heap(CMeshInfo* first, long hole, long top, CMeshInfo value, CSortPrimaryMeshesLast comp)
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