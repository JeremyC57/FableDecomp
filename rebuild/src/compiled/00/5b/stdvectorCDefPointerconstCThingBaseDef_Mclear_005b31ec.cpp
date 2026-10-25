// std::vector<CDefPointer<const CThingBaseDef> >::_M_clear() -- STLport: destroy [start, finish), then release the block
// Leaf: ?_M_clear@?$vector@V?$CDefPointer@$$CBUCThingBaseDef@@@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B31EC, 35 bytes, thiscall `ret`: `__destroy_aux(start, finish, __false_type())`
//   (0x005B26E9, `__fastcall` -- ecx = first, edx = last, the empty tag by reference in the high byte of the `push ecx`
//   slot, `lea eax,[esp+7]`), then `if (start) free(start)` (0x00BFEA14).
// Identity (was the `__fastcall OnReadFinished(AutoTinyActiveFile*)` stand-in catalogued as StdList_Dtor_005b31ec): its
//   one caller is 0x005B330B in std::vector<CDefPointer<const CThingBaseDef> >::_M_insert_overflow 0x005B3275 (STLport
//   `_M_insert_overflow` ends with `_M_clear(); _M_set(...)`), whose element is the 4-byte CDefPointer (Ego_r.pdb
//   `CDefPointeeBase* Object`); the callee 0x005B26E9 is the element range destroy. Byte twin of the landed
//   std::vector<CAttackHistoryCombo>::_M_clear 0x004F963D (another size TU).
// Shape: STLport 4.x `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start,
//   _M_end_of_storage._M_data - _M_start); }` with _Destroy -> __destroy_aux(.., __false_type) and the null-tested
//   deallocate inline (`__forceinline` _Destroy: the size TU otherwise keeps it out of line). The tag has a user-declared
//   constructor (no zero-fill of the temporary).
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CThingBaseDef;
class CDefPointeeBase;

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                       // +0x00 (Ego_r.pdb)
};

namespace std {
struct __false_type { __false_type() { } };

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x005B26E9

template <class _ForwardIterator>
__forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, allocator<_Tp> > _M_end_of_storage; // +0x08
    void _M_clear();
};

template <class _Tp>
void vector<_Tp>::_M_clear()
{
    _Destroy(_M_start, _M_finish);
    _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
}

template void vector<CDefPointer<const CThingBaseDef> >::_M_clear();
}
