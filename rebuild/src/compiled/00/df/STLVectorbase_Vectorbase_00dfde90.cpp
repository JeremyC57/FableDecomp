// std::_Vector_base::_Vector_base(unsigned int n, const allocator& a) -- (_Vector_base<CScriptThing>) STLport vector storage constructor: zero the three words, allocate n 12-byte CScriptThings, set start/finish/end
// Leaf: ??0?$_Vector_base@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAE@IABV?$allocator@VCScriptThing@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DFDE90, 58 bytes (+ int3 padding), `ret 8` (returns this).
// Callees: malloc @ 0x00BFEA0E (the STLport allocator's allocate: `n != 0 ? malloc(n * sizeof(T)) : 0`).
// Identity: its only caller is the vector<CScriptThing> copy constructor 0x00DFDE30 (element copy = the landed
//   `_Construct(CScriptThing*, const CScriptThing&)` 0x008AC120, stride 12, called on CQ_TraderConflictEvilScript's
//   AllCreatures at +0x48 from 0x00DFD616); the catalog's "Std_Vector_Initialize" is descriptive.
// Shape: STLport 4.6 `_Vector_base(size_t __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
//   { _M_start = _M_end_of_storage.allocate(__n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + __n; }`
//   (the landed out-of-line instantiation 0x00477530 is the size-TU twin): one zero register clears the three words and is
//   the null allocation; `add esp,4` after malloc = a speed TU (no pragma).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E

class CScriptThing {                                       // 12 bytes (vptr 0x01238C8C, counted-pointer Data/Info)
public:
    unsigned char _body[12];
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a);
};

template <class _Tp, class _Alloc>
_Vector_base<_Tp, _Alloc>::_Vector_base(unsigned int __n, const _Alloc& __a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
{
    _M_start = _M_end_of_storage.allocate(__n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + __n;
}

template class _Vector_base<CScriptThing, allocator<CScriptThing> >;

}