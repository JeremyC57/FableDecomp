// std::vector::_M_fill_insert(pair* position, unsigned int n, const pair& x) -- (vector<pair<float, CSubtitledMessage::CCameraTag>>) STLport insert of n copies of a subtitle camera tag: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@USCameraTagEntry@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00411DB0, 339 bytes, thiscall `ret 0Ch`; /O2.
// Identity: stride 0x1C (signed divide by 28) with the landed out-of-line helpers of this element; callers are
//   insert(position, n, x) (tail jump 0x00412850) and resize(n, x) 0x00412A41. The catalog label
//   "Std_Vector_Insert_Range_Dispatch" is wrong: this is _M_fill_insert.
// Callees: __uninitialized_copy 0x0040FF80 (twice), __copy_backward(first, last, result, const
//   random_access_iterator_tag&, int*) 0x0040EB60, fill(first, last, x) 0x0040E9A0 (twice), __uninitialized_fill_n
//   0x0040EAC0, _M_insert_overflow(position, x, const __false_type&, n, atend) 0x004117C0 (this chunk); all __fastcall
//   free functions take first/last in ecx/edx.
// Shape: STLport 4.x `_M_fill_insert` verbatim (x_copy is the member-wise copy into the 0x1C-byte frame). Every empty
//   tag is homed on the dead/live `x` argument slot ([esp+38h]); the first branch-independent tag push is hoisted above
//   the elems_after test. VC7.1 places the tags there only when each tag-taking callee has a same-TU template body
//   with the tag unnamed (GOTCHAS 504/667), so all four have one here (bodies as the landed helpers).
// Model: the element as 0x004117C0; STLport's three vector words.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

enum EConversationProtagonist { CONVERSATION_PROTAGONIST_NONE };
enum EConversationCameraPosition { CONVERSATION_CAMERA_POSITION_NONE };
enum EConversationCameraFocus { CONVERSATION_CAMERA_FOCUS_NONE };
enum EConversationCameraZoomType { CONVERSATION_CAMERA_ZOOM_NONE };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_NONE };

struct SCameraTagEntry {                                  // pair<float, CSubtitledMessage::CCameraTag> flattened, 0x1C
    float                       Time;                     // +0x00 (pair.first)
    EConversationProtagonist    CameraPositionPerson;     // +0x04
    EConversationCameraPosition CameraPosition;           // +0x08
    EConversationProtagonist    CameraFocusPerson;        // +0x0C
    EConversationCameraFocus    CameraFocusPosition;      // +0x10
    EConversationCameraZoomType CameraZoomType;           // +0x14
    EConversationCameraView     CameraView;               // +0x18
    SCameraTagEntry(const SCameraTagEntry& o)
        : Time(o.Time), CameraPositionPerson(o.CameraPositionPerson), CameraPosition(o.CameraPosition),
          CameraFocusPerson(o.CameraFocusPerson), CameraFocusPosition(o.CameraFocusPosition),
          CameraZoomType(o.CameraZoomType), CameraView(o.CameraView) { }
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x0040FF80
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)                     // 0x0040EAC0
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)   // 0x0040EB60
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_backward_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)        // 0x0040E9A0
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void _M_clear() { if (_M_start != 0) free(_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x004117C0
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                                     bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = allocate(__len);
    pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(this->_M_end_of_storage - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<SCameraTagEntry>::_M_fill_insert(SCameraTagEntry*, unsigned int, const SCameraTagEntry&);

}