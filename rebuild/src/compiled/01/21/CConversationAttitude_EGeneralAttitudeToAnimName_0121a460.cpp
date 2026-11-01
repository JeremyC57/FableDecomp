// CConversationAttitude::$E_GeneralAttitudeToAnimName() -- dynamic initializer of CConversationAttitude::GeneralAttitudeToAnimName: the inline STLport 4.6 vector(n, value) constructor (18 copies of CCharString("")), then its atexit destructor is registered
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121A460, 139 bytes, in `.text$yc`, reached from the .CRT$XCU table entry 0x013742E4, plain `ret`.
//   Registers the landed atexit destructor 0x01228D00 (`_$E2`, the inline ~vector<CCharString>) over the same vector at 0x013B8A2C.
// Identity (proven, two sources): (1) Ego_r.pdb and FableWin.pdb list exactly three CConversationAttitude static initializers --
//   `dynamic initializer for 'CConversationAttitude::GeneralAttitudeToAnimName'`, ...ConversationAttitudeToAnimName,
//   ...ConversationAttitudeToAnimGroupName -- in the order of retail's .CRT$XCU entries 0x0121A460 / 0x0121A4F0 / 0x0121A580 and
//   of their atexit thunks 0x01228D00 / 0x01228D40 / 0x01228D80; (2) 0x013B8A2C is the table CConversationAttitude::Init 0x004CD670
//   fills (18 stores) and the static accessor 0x004CD9A0 (`mov eax,[0x013B8A2C] ; lea eax,[eax+ecx*4]` = PDB
//   ?GetAnimName@CConversationAttitude@@SIABVCCharString@@W4EAttitude@@@Z) indexes; 18 entries = the EAttitude table.
//   The landed `_$E2` 0x01228D00 names the same vector. Catalog row "_gapscan::sub_0121a460" carried no name.
// Not a toolchain-library object: a compiler-generated initializer of a Fable static (no CRT/vendor .lib contains it).
// Model: STLport 4.6 stl/_vector.h vector(size_type, const _Tp&, const allocator_type&) over _Vector_base(size_t, const _Alloc&)
//   (zero the three words, allocate, finish = start, end = start + n) and stl/_uninitialized.h __uninitialized_fill_n(__false_type)
//   with stl/_construct.h _Construct (placement new: the `test esi,esi ; je` per element); the malloc-backed allocator of this
//   build (allocate = malloc 0x00BFEA0E, as the landed 0x0121A670).
// Shape: the value is the literal "" converted by the implicit CCharString(const char*, long = -1) constructor; an explicit
//   `CCharString("")` temporary binds the reference to the ctor's returned this (held in ebx, `push ebx`), whereas the implicit
//   conversion rematerialises `lea edx,[esp+8]` per element as retail does (probe 1 DIFFER, probe 2 match).
// Callees: CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z), malloc 0x00BFEA0E, CCharString(const CCharString&)
//   0x0099EC30 (??0CCharString@@QAE@ABV0@@Z), ~CCharString 0x0099EAE0, _atexit 0x004012BC (the `_$E2` thunk 0x01228D00).
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }
class CCharStringData;
class CCharString { public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    typedef unsigned int size_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    vector(size_type __n, const _Tp& __val, const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__n, __a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
    }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CConversationAttitude { public:
    static std::vector<CCharString> GeneralAttitudeToAnimName;            // 0x013B8A2C
};
std::vector<CCharString> CConversationAttitude::GeneralAttitudeToAnimName(18, "");