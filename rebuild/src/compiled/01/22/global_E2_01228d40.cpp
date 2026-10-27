// _$E2() -- `dynamic atexit destructor for 'CConversationAttitude::ConversationAttitudeToAnimName'': the inlined STLport ~vector<CCharString> of the static (each element's ~CCharString, then free the storage)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01228D40, 54 bytes, in `.text$yd`; registered by the initializer 0x0121A4F0 (`push 0x01228D40 ; call _atexit`).
// Identity (proven): initializer 0x0121A4F0 builds the vector with 12 CCharString("") entries and registers this thunk; second of the three CConversationAttitude statics in the Ego_r.pdb/FableWin.pdb initializer and atexit lists (retail 0x0121A460/4F0/580, thunks 0x01228D00/40/80).
// Callees: CCharString::~CCharString 0x0099EAE0 (per element), free 0x00BFEA14 (STLport allocator<T>::deallocate).
// Shape (family M002, representative 0x012289B0): `std::vector<CCharString> CConversationAttitude::ConversationAttitudeToAnimName;` -- VC7.1 emits the initializer `_$E1`
//   and this atexit thunk `_$E2` with ~vector inlined: STLport _Destroy(_M_start, _M_finish) (the loop keeps start/finish
//   in esi/edi) then ~_Vector_base re-reads _M_start for the `if (_M_start) free(_M_start)`. The initializer here
//   default-constructs the vector (only the thunk is graded; retail's initializer is the separate row 0x0121A4F0).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CCharString {
public:
    CCharString();
    ~CCharString();                                     // 0x0099EAE0
    void* PStringData;
};
namespace std {
template <class _Tp>
class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~_Vector_base() { if (_M_start != 0) _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector() { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
class CConversationAttitude {                              // Ego_r.pdb: no data members
public:
    static std::vector<CCharString> ConversationAttitudeToAnimName;   // 0x013B8A38
};
std::vector<CCharString> CConversationAttitude::ConversationAttitudeToAnimName;