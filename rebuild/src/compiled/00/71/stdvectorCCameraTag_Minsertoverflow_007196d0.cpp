// std::vector<CCameraTag>::_M_insert_overflow(pointer pos, const CSubtitledMessage::CCameraTag& x, const __false_type&, size_type fill_len, bool atend) -- STLport grow-and-insert of 24-byte camera tags: allocate old + max(old, fill_len), copy the head, construct the fill, copy the tail unless appending, free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007196D0, length 238 bytes (+ int3 padding). __fastcall, `ret 14h`.
// Callees: malloc / free (CRT import thunks) @ 0x00BFEA0E / 0x00BFEA14, __uninitialized_copy<CCameraTag*> @ 0x00718390
//          (twice), __uninitialized_fill_n<CCameraTag*> @ 0x007183E0.
// Identity: the manifest label "Vector_ReallocateAndCopyMorphs" is wrong (no morphs). The push_back overflow of
//   vector<CSubtitledMessage::CCameraTag>: seven calls from CActionTalkBase::InitialiseDefaultCameraTags 0x007167E0
//   (the static DefaultCameraTags) and one from 0x00719ADA; `imul 2AAAAAABh ; sar edx,2` = divide by 24.
// Shape: STLport 4.x `_M_insert_overflow(pos, x, __false_type, fill_len, atend)` verbatim (the 0x0070B240 shape):
//   `(max)` by const reference, `allocate(len)` = `len != 0 ? malloc(len * 24) : 0`, the single-element case as the
//   inline `_Construct`, the empty tag temporaries homed in the dead `atend` slot, then `_M_clear()` (the trivial
//   `_Destroy` loop folds away, `if (p != 0) free(p)`) and `_M_set`. The two algorithm callees are same-TU stand-in
//   bodies under auto_inline(off) (the lesson of 0x0070B240).
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

enum EConversationProtagonist { CONVERSATION_PROTAGONIST_DUMMY };
enum EConversationCameraPosition { CONVERSATION_CAMERA_POSITION_DUMMY };
enum EConversationCameraFocus { CONVERSATION_CAMERA_FOCUS_DUMMY };
enum EConversationCameraZoomType { CONVERSATION_CAMERA_ZOOM_TYPE_DUMMY };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_DUMMY };

class CSubtitledMessage {
public:
    class CCameraTag {                                  // Ego_r.pdb, 24 bytes
    public:
        EConversationProtagonist CameraPositionPerson;  // +0x00
        EConversationCameraPosition CameraPosition;     // +0x04
        EConversationProtagonist CameraFocusPerson;     // +0x08
        EConversationCameraFocus CameraFocusPosition;   // +0x0C
        EConversationCameraZoomType CameraZoomType;     // +0x10
        EConversationCameraView CameraView;             // +0x14
    };
};

namespace std {
template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

// Same-TU stand-in bodies (the real ones, 0x00718390 / 0x007183E0, are out of line in this TU): VC7.1 uses what it learns
// from them.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x00718390
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x007183E0
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {   // the end-of-storage pointer
public:
    _Value _M_data;
};

template <class _Tp> class vector {                     // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    _Tp* __new_start = _M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<CSubtitledMessage::CCameraTag>;
}