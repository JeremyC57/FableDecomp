// std::vector::push_back(NPlayerGui::CDrawBase* const& x) -- (vector<NPlayerGui::CDrawBase*>) STLport append: construct x at the end when there is room, else grow through _M_insert_overflow(finish, x, __true_type(), 1, true)
// Leaf: ?push_back@?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@QAEXABQAVCDrawBase@NPlayerGui@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656806, 49 bytes, thiscall `ret 4`; frame-pointer size-optimised TU (`push ebp ; mov ebp,esp`, the empty tag homed in the dead argument slot `[ebp+0Bh]`).
// Identity (proven): vector<NPlayerGui::CDrawBase*>::push_back; evidence: its only caller is the CDrawBase constructor 0x0064CC65 (landed), which registers `this` in the draw list it is given (vector<CDrawBase*>); the overflow 0x0065625E is the POD one (out-of-line __copy_trivial 0x00405BA0).
// Label: the manifest's "CDrawBase_PushBackValue" is descriptive (family H061: six 4-byte-element push_backs, one per element).
// Callee: vector<NPlayerGui::CDrawBase*>::_M_insert_overflow(pointer, NPlayerGui::CDrawBase* const&, const __true_type&, size_type, bool) 0x0065625E (__true_type: the element is POD under STLport's type traits).
// Shape: STLport 4.6 `push_back(x) { if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true); }` -- placement new's null test, `this` kept in ecx for the call;
//   `optimize("y", off)` + `("s", on)` as the landed vector<long> push_back callers' TUs.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }
namespace NPlayerGui { class CDrawBase; }

namespace std {

struct __true_type { __true_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { public: allocator() { } };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08 (the _STLP_alloc_proxy's pointer)
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x0065625E
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};

}

template void std::vector<NPlayerGui::CDrawBase* >::push_back(NPlayerGui::CDrawBase* const& __x);