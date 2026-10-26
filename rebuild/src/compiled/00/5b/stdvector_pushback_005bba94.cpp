// std::vector::push_back(EClothingSuitPart const& x) -- (vector<EClothingSuitPart>) STLport append: construct x at the end when there is room, else grow through _M_insert_overflow(finish, x, __false_type(), 1, true)
// Leaf: ?push_back@?$vector@W4EClothingSuitPart@@V?$allocator@W4EClothingSuitPart@@@std@@@std@@QAEXABW4EClothingSuitPart@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBA94, 49 bytes, thiscall `ret 4`; frame-pointer size-optimised TU (`push ebp ; mov ebp,esp`, the empty tag homed in the dead argument slot `[ebp+0Bh]`).
// Identity (proven): vector<EClothingSuitPart>::push_back; evidence: CTCInventoryClothing (landed caller) pushes CAppearanceModifierDef::SuitPart -- Ego_r.pdb EClothingSuitPart -- onto its kept-parts vector through it; the enum element makes STLport's _IsPODType __false_type, and the overflow 0x005BB4E6 is the __false_type one.
// Label: the manifest's "std::vector_PushBack" is descriptive (family H061: six 4-byte-element push_backs, one per element).
// Callee: vector<EClothingSuitPart>::_M_insert_overflow(pointer, EClothingSuitPart const&, const __false_type&, size_type, bool) 0x005BB4E6 (__false_type: an enum element is not POD to STLport 4.6's type traits).
// Shape: STLport 4.6 `push_back(x) { if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true); }` -- placement new's null test, `this` kept in ecx for the call;
//   `optimize("y", off)` + `("s", on)` as the landed vector<long> push_back callers' TUs.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }
enum EClothingSuitPart { };                                 // enumerators not needed

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { public: allocator() { } };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08 (the _STLP_alloc_proxy's pointer)
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x005BB4E6
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

}

template void std::vector<EClothingSuitPart >::push_back(EClothingSuitPart const& __x);