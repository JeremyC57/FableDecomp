// std::sort_heap(CSymbolInfo* first, CSymbolInfo* last, CCompareSymbolInfoName comp) -- SGI sort_heap over 12-byte (CCharString, CCharString, long) symbol records ordered by the first string: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A41C20, length 130 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>*,...>` is propagated
// over the byte-identical skeleton. Stride 12 (0x2AAAAAAB / sar 1); the callee 0x00A41630 (manifest `_Pop_heap<CTestQuest*,
// ...>`, propagated) is the SGI `__pop_heap` of a flat 12-byte record -- two CCharString::operator= and a dword in the
// assignment, two string copies in the value copy, `ret 18h` -- feeding 0x00A410A0 (manifest
// "Heap_AdjustDown_CCharStringTriple"), which orders on the first string (the inline CCharString operator< over
// 0x00429950). The record matches Ego_r.pdb CSymbolMap::CSymbolInfo (GroupName, EntryName, long Val) and sits next to
// "Introsort_Partition_CSymbolInfo" 0x00A40DF0; the comparator name is descriptive.
// Callees: __pop_heap @ 0x00A41630 (out of line, __fastcall), CCharString copy constructor @ 0x0099EC30 (x2).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy in the argument area is the record's implicit copy constructor: two string
// copies off the slot base (`lea ecx,[edi+4]`, a flat record -- a nested pair would re-base with `add edi,4`) and Val.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CSymbolInfo {                        // CSymbolMap::CSymbolInfo (Ego_r.pdb, 12 bytes)
    CCharString GroupName;                  // +0x00
    CCharString EntryName;                  // +0x04
    long        Val;                        // +0x08
};
struct CCompareSymbolInfoName { };          // empty, passed by value

void __fastcall __pop_heap(CSymbolInfo* first, CSymbolInfo* last, CSymbolInfo* result, CSymbolInfo value,
                           CCompareSymbolInfoName comp, int*);   // 0x00A41630

inline void __pop_heap_aux(CSymbolInfo* first, CSymbolInfo* last, CSymbolInfo*, CCompareSymbolInfoName comp)
{
    __pop_heap(first, last - 1, last - 1, CSymbolInfo(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(CSymbolInfo* first, CSymbolInfo* last, CCompareSymbolInfoName comp)
{
    __pop_heap_aux(first, last, (CSymbolInfo*)0, comp);
}

void __fastcall sort_heap(CSymbolInfo* first, CSymbolInfo* last, CCompareSymbolInfoName comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}