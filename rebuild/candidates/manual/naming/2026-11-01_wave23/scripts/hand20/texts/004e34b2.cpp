#pragma optimize("s", on)
// std::vector<std::pair<long, CCharString> >::~vector() -- STLport 4.6: _Destroy(start, finish) through the out-of-line __destroy_aux 0x004DD782, then
//   ~_Vector_base frees the storage
// Leaf: ??1?$vector@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E34B2, 35 bytes, thiscall (an optimize("s") TU whose free templates are __fastcall).
// Identity (chunk c678 queue row ??1?$vector@U?$pair@JVCCharString@@@std@@...): the bound ??1?$vector@U?$pair@JVCCharString@@... of the landed
//   0x00CE1AF0 row; body = STLport ~vector: _Destroy(_M_start, _M_finish) -> __destroy(first, last, first) -> __destroy_aux(first, last,
//   __false_type()) out of line at 0x004DD782 (its own row; the tag temporary is the byte at [esp+7]), then ~_Vector_base: free(_M_start) when set.
// Callees: __destroy_aux<pair<long,CCharString>*> 0x004DD782 (__fastcall, the tag by const reference), free 0x00BFEA14 (allocator::deallocate).
// Naming wave 19 (wave 18 recheck-fail re-run): landed as the free OnReadFinished(AutoTinyActiveFile*) stand-in (a CActiveFile label) calling AutoTinyFinishRead_004dd782;
//   re-authored on the STLport instantiation (name-only: the bytes are unchanged).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x004DD782 (its own row)
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
__forceinline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
template class std::vector<std::pair<long, CCharString> >;
