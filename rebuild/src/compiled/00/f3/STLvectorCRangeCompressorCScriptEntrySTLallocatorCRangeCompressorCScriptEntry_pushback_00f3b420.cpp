// std::vector<CScriptEntry>::push_back(const CScriptEntry& x) -- range-compressor script entries: copy the entry into place when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@UCScriptEntry@CRangeCompressor@@V?$allocator@UCScriptEntry@CRangeCompressor@@@std@@@std@@QAEXABUCScriptEntry@CRangeCompressor@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3B420, 82 bytes, thiscall `ret 4`, no frame: `_M_finish != end` -> six dword moves
//   into *_M_finish and `+= 0x18`, else `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` (0x00F3B330) with the
//   empty tag temporary homed in the dead argument slot.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 453 "landed ... 0xF3B420 (SRecord24) are CScriptEntry containers"; hand453
//   README: vector<CRangeCompressor::CScriptEntry>::push_back): the callee is the landed
//   std::vector<CScriptEntry>::_M_insert_overflow 0x00F3B330, whose only caller this is; its caller is
//   CRangeCompressor::CalcCompressionScript 0x00F3ABF0; element = Ego_r.pdb CRangeCompressor::CScriptEntry (24 bytes).
//   The landed CRetailVector_SRecord24 row named it by element size.
// Namespace: std as in this TU's landed rows (0x00F3B1D0, 0x00F3B330).
// Model: the 0x00F3B330 row's STLport pieces; STLport 4.6 push_back as written (_IsPODType() = __false_type for a type
//   without __type_traits).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                    // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class CRangeCompressor {
public:
    struct CScriptEntry {                        // 24 bytes (Ego_r.pdb), all scalars: implicit trivial copy
        unsigned long Flags;                     // +0x00
        long          BitCount;                  // +0x04
        unsigned long RangeBias;                 // +0x08
        long          RangeShift;                // +0x0c
        unsigned long ShuffleMask;               // +0x10
        unsigned long OrMask;                    // +0x14
    };
};
typedef CRangeCompressor::CScriptEntry CScriptEntry;
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    void push_back(const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);   // 0x00F3B330
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
}
template void vector<CScriptEntry>::push_back(const CScriptEntry&);
}   // namespace std
