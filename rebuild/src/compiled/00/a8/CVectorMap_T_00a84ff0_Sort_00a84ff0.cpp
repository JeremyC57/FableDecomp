// CVectorMap<T_00a84ff0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_00a84ff0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a8/CGuiVarTransferBool_TransferToGui_00a84ff0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_00a84ff0::Sort  @ 0x00a84ff0
// Callee: 0x00A84CF0, STLport `sort(first, last, comp)` over 12-byte elements (__fastcall /Gr algorithm: first in ecx,
//   last in edx, the empty comparator by value on the stack; its body is `if (first != last) { __introsort_loop(first,
//   last, (T*)0, __lg(last - first) * 2, comp) @ 0x00A847C0; __final_insertion_sort(...) }`, the count divided by 12
//   through `imul 0x2AAAAAAB ; sar edx,1`). Its other callers are 0x00A7A650 (CNavQuadTree::LinkToNeighbouringMaps per the
//   manifest) and 0x00A85340; the element and comparator types are not pinned, so they keep descriptive names.
// (The earlier model named the three arguments m_pTarget / m_data / m_value and the callee CGuiVarTransferBool_Apply:
//   those are the vector's _M_start / _M_finish and the map's Compare.)
struct SElement12_00a84cf0 {                       // 12-byte element (opaque)
    unsigned char _body[12];
};

struct CCompare_00a84ff0 { };                      // the map's empty comparator (1 byte, copied by value)

void __fastcall Sort_00a84cf0(SElement12_00a84cf0* first, SElement12_00a84cf0* last, CCompare_00a84ff0 comp);   // 0x00A84CF0

struct CVectorMap_T_00a84ff0
{
    SElement12_00a84cf0* First;                    // +0x00 vector _M_start
    SElement12_00a84cf0* Last;                     // +0x04 vector _M_finish
    SElement12_00a84cf0* EndOfStorage;             // +0x08
    CCompare_00a84ff0    Compare;                  // +0x0C
    bool                 Dirty;                    // +0x0D

    void Sort();
};

void CVectorMap_T_00a84ff0::Sort()
{
    Sort_00a84cf0(First, Last, Compare);
    Dirty = false;
}
