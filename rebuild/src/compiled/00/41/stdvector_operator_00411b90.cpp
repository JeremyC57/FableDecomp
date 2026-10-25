// std::vector::operator=(const vector& x) -- (vector<pair<float, CSubtitledMessage::CCameraTag>>) STLport copy assignment of the subtitle camera-tag vector: reallocate and copy, assign in place, or assign the overlap and copy-construct the rest
// Leaf: ??4?$vector@USCameraTagEntry@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00411B90, 321 bytes, thiscall `ret 4`, returns `this`; /O2.
// Identity: stride 0x1C (signed divide by 28: 0x92492493) with the landed out-of-line helpers __uninitialized_copy
//   0x0040FC50 (the placement-copy twin of 0x0040FF80) and __copy 0x0040E9D0 (the counted `rep movsd` assignment loop);
//   same element as _M_insert_overflow 0x004117C0. Eight callers (0x004086B2 .. 0x00431E95: the subtitle def copies).
//   The catalog label "std_vector_assign" is right.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x0040FC50 (__fastcall,
//   `ret 8`, twice), free 0x00BFEA14, __copy(first, last, result, const random_access_iterator_tag&, int*) 0x0040E9D0
//   (__fastcall, `ret 0Ch`, twice; the null `_Distance*` push is shared by both branches).
// Shape: STLport 4.x `vector::operator=` verbatim with `_M_allocate_and_copy`, `_M_clear` and `__copy_ptrs` inlined;
//   the trivial `_Destroy` vanishes. Every empty tag is homed on the live `x` argument slot ([esp+18h] / [esp+1Ch]),
//   which VC7.1 does only when the two helpers have same-TU template bodies with the tag unnamed (GOTCHAS 504/667).
// Model: the element as 0x004117C0 (pair<float, CCameraTag> flattened, member-wise user copy constructor, implicit
//   assignment); STLport's three vector words.
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

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)   // 0x0040FC50
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&,
                                                   _Distance*)                                                 // 0x0040E9D0
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter> inline void __destroy_aux(_ForwardIter, _ForwardIter, const __true_type&) { }

template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    __destroy_aux(__first, __last, __true_type());
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p) { if (__p != 0) free(__p); }
    void _M_clear() { _Destroy(_M_start, _M_finish); deallocate(_M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    vector<_Tp>& operator=(const vector<_Tp>& __x);
};

template <class _Tp>
vector<_Tp>& vector<_Tp>::operator=(const vector<_Tp>& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0,
                                      (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start,
                        __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0,
                                 this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<SCameraTagEntry>& vector<SCameraTagEntry>::operator=(const vector<SCameraTagEntry>&);

}