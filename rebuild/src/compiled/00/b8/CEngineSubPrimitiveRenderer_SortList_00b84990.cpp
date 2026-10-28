// CEngineSubPrimitiveRenderer::SortList(CArray<CSortElement>& list, bool stable) -- sort the sub-primitive list by SortValue: stable merge sort when asked, introsort under 256 elements, radix sort otherwise
// Leaf: ?SortList@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84990, 82 bytes, thiscall, `ret 8`, /Gr TU (the STLport callees are __fastcall).
// Flags: /Gr
// Identity (proven): Ego_r.pdb ?SortList@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@...@@_N@Z; its three
//   callees are the landed std::__stable_sort_aux<CSortElement*> 0x00B86710 and std::sort<CSortElement*> 0x00B868C0 and
//   the private CEngineSubPrimitiveRenderer::RadixSort 0x00B847D0 (thiscall, this kept in ecx).
// Shape: STLport stable_sort(first, last) inline wrapper -> __stable_sort_aux(first, last, (T*)0, (int*)0, less<T>());
//   the empty less<> is pushed from the dead `stable` argument slot.
class CEngineSubPrimitiveBase;

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
};
}

template <class T> class CArray : public std::vector<T> { };

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
private:
    void SortList(CArray<CSortElement>& list, bool stable);
    void RadixSort(CArray<CSortElement>& list);         // 0x00B847D0
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace std {
template <class _Tp> struct less {
    less() {}
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
void __fastcall __stable_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Distance*, _Compare __comp);  // 0x00B86710
template <class _RandomAccessIter>
inline void stable_sort(_RandomAccessIter __first, _RandomAccessIter __last)
{
    __stable_sort_aux(__first, __last, (CSortElement*)0, (int*)0, less<CSortElement>());
}
template <class _RandomAccessIter>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last);   // 0x00B868C0
}

void CEngineSubPrimitiveRenderer::SortList(CArray<CSortElement>& list, bool stable)
{
    if (list.empty())
        return;
    if (stable)
        std::stable_sort(list.begin(), list.end());
    else if (list.size() < 256)
        std::sort(list.begin(), list.end());
    else
        RadixSort(list);
}