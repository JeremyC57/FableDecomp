// std::vector::_M_insert_overflow(pair* position, const pair& x, const __false_type&, unsigned int fillLength, bool atEnd) -- (vector<pair<float, CSubtitledMessage::CCameraTag>>) STLport growth path of the subtitle camera-tag vector: allocate size + max(size, n), copy the head, construct n copies of x, copy the tail unless at the end, free the old block
// Leaf: ?_M_insert_overflow@?$vector@USCameraTagEntry@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004117C0, 241 bytes, thiscall `ret 14h` (position, x, the tag, fillLength, atEnd); /O2.
// Identity: stride 0x1C with the landed placement-copy helpers __uninitialized_copy 0x0040FF80 and
//   __uninitialized_fill_n 0x0040EAC0 (the catalog's pair<float, CSubtitledMessage::CCameraTag> element); single caller
//   0x00411EF5 (push_back / insert). The catalog label "Std_Vector_Insert_Range" is wrong: this is _M_insert_overflow.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy(first, last, result, const __false_type&) 0x0040FF80 (__fastcall,
//   `ret 8`, twice), __uninitialized_fill_n(first, n, x, const __false_type&) 0x0040EAC0 (__fastcall, `ret 8`), free 0x00BFEA14.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)`: the old size is a signed divide by 28
//   (0x92492493 multiply) homed in the `push ecx` slot for the const-reference `max`; `allocate` = `n ? malloc : 0`; the
//   n == 1 `_Construct` is inlined (null test + the flat seven-word copy); `_M_clear` is the trivial-destructor free.
//   The empty tag the three helper calls take is homed on the live `atEnd` slot ([esp+28h]): VC7.1 does that only
//   when the two out-of-line helpers have same-TU template bodies with the tag unnamed (GOTCHAS 504/667).
// Model: the element as the landed 0x0040EAC0: pair<float, CCameraTag> flattened to Time + the six camera enums (Ego_r.pdb
//   CSubtitledMessage::CCameraTag) with a member-wise user copy constructor; STLport's three vector words.
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
                            bool __atend = false);
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

template void vector<SCameraTagEntry>::_M_insert_overflow(SCameraTagEntry*, const SCameraTagEntry&, const __false_type&,
                                                          unsigned int, bool);

}