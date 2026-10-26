// CompareQuickDrawTriDepth(const void*, const void*) -- qsort comparator over the 0x3C-byte quick-draw records: -1 when the first record's depth (+0x24) is greater, else 1 (back-to-front order)
// Leaf: ?CompareQuickDrawTriDepth@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8C30, length 32 bytes.
// Descriptive name: the only reference is the immediate `push 0x9d8c30` in the CRenderManager2D flush at 0x009DA607,
// which sorts the vector at +0x3E7C (element size 0x3C = CQuickDrawTriInfo, 60 bytes in the PDB) with CRT qsort
// (0x00BFEDCA). The PDB name of the comparator is not in either name set. The float at +0x24 sits in the record's
// 8-byte union at +0x20 (second word: the draw depth). Shape: `fld a ; fcomp b ; test ah,41h ; je` is the ordered
// `a > b`; the result is selected from two immediates (`mov eax,-1` before the branch, `mov eax,1` after).
class CQuickDrawTriInfo
{ public:
    unsigned char _pad00[0x24];
    float         m_depth;          // +0x24
    unsigned char _pad28[0x3C - 0x28];
};

int __cdecl CompareQuickDrawTriDepth(const void* left, const void* right)
{
    const CQuickDrawTriInfo* a = (const CQuickDrawTriInfo*)left;
    const CQuickDrawTriInfo* b = (const CQuickDrawTriInfo*)right;
    return a->m_depth > b->m_depth ? -1 : 1;
}