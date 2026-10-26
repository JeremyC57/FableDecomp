// std::vector::resize(size_type __new_size, const _Tp& __x) -- (vector<CDateAndTime >) STLport resize: erase the surplus [begin() + n, end()) when shrinking, else insert n - size() copies of x at the end
// Leaf: ?resize@?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@std@@QAEXIABVCDateAndTime@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004861F7, 72 bytes (size-optimised TU, odd entry), thiscall, `ret 8`.
// Identity (probable): std::vector<CDateAndTime >::resize(size_type, const CDateAndTime&); evidence: its only caller is resize(n) 0x0048623F, which default-constructs its 28-byte T through 0x009E9AB0 (seven dwords zeroed -- the Ego_r.pdb CDateAndTime {long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond}) and passes it on; that resize(n) is called by the two save-list builders 0x0047FE83 / 0x00597DF7 right after they resize their CWideString name vector (0x00412B70), the pair Ego_r.pdb CUserProfileManager::GetSortedSaveNamesAndTimes(vector<CWideString>&, vector<CDateAndTime>&) fills; both builders also construct CDateAndTime values through 0x009E9AB0 + GetFileModifiedTime 0x009992A0 and clear the same vector with this row's erase 0x00411B00
// Label: the manifest's "push_back" is a behavioural / propagated label; this row is vector<CDateAndTime >::resize(n, x) (family J069: five 72-byte bodies, each its own element).
// Callees: erase(iterator, iterator) 0x00411B00, _M_fill_insert(iterator, size_type, const CDateAndTime&) 0x0048608C (both out of line, thiscall).
// Shape: STLport 4.6 vector.h `resize` verbatim with insert(pos, n, x) inlined to _M_fill_insert; size() is the exact
//   pointer difference (`idiv 0x1C`, the divisor loaded by push/pop under /Os) and is recomputed on the insert path.
// Element: CDateAndTime (Ego_r.pdb, 28 bytes: Year, Month, Day, Hour, Minute, Second, Millisecond) -- probable: the type rests on the default constructor's shape and the save-list context, not on RTTI
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                          // 0x00411B00
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);         // 0x0048608C
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}   // namespace std
class CDateAndTime {                                     // Ego_r.pdb 28 bytes
public:
    long          Year;
    unsigned long Month;
    unsigned long Day;
    unsigned long Hour;
    unsigned long Minute;
    unsigned long Second;
    unsigned long Millisecond;
};
namespace std {
template void vector<CDateAndTime >::resize(unsigned int, const CDateAndTime&);
}