// _global::_Adjust_heap(NGraphicAppearance::CMeshInfo*, long, long, NGraphicAppearance::CMeshInfo, CSortPrimaryMeshesLast) -- Dinkumware heap sift-down of 12-byte mesh infos with primary meshes ordered last, then _Push_heap @ 0x004C2000
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004c2d10, 152 bytes.
// Callee: 0x004C2000, whose manifest label is
//   _Push_heap<NGraphicAppearance::CMeshInfo*,int,NGraphicAppearance::CMeshInfo,CSortPrimaryMeshesLast>
// and whose loop tests the same ``not ; test 1` on the bit`.  first in ecx, hole in edx (link-time
// __fastcall); bottom, the 12-byte element by value (built in its argument slot: `sub esp,0Ch ;
// mov eax,esp` + three whole-dword stores -- the IMPLICIT copy constructor; a user memberwise one
// re-merges the PrimaryMesh bit with xor/and) and the empty comparator follow (`ret 14h`).  Twin of 0x004C2C70 (CSortPrimaryMeshesFirst).
// Element: Ego_r.pdb NGraphicAppearance::CMeshInfo (0x0C) = long Index +0, float SizeX +4,
// uchar PrimaryMesh:1 +8; CSortPrimaryMeshesLast is an empty functor.  The comparator reads only
// its FIRST argument (`mov cl,[ecx+eax*4+8] ; not cl ; test cl,1`): pred(a, b) = !a.PrimaryMesh.
// Label correction: the propagated pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> heap
// has the same 12-byte size but no bitfield; the callee's label and the bit test identify CMeshInfo.
namespace FableHeap_004c2d10 {

struct CMeshInfo {
    long Index;
    float SizeX;
    unsigned char PrimaryMesh : 1;
};

struct CSortPrimaryMeshesLast {
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return !a.PrimaryMesh; }
};

void __fastcall _Push_heap(CMeshInfo* first, long hole, long top, CMeshInfo value, CSortPrimaryMeshesLast pred);   // 0x004C2000

void __fastcall _Adjust_heap(CMeshInfo* first, long hole, long bottom, CMeshInfo value, CSortPrimaryMeshesLast pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}