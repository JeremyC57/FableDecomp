// _STL::vector<pair<float,CCameraTag>>::vector(const vector& x) -- STLport vector copy constructor for the CSubtitledMessage camera-tag map's 28-byte pair<float, CSubtitledMessage::CCameraTag> elements: allocate exactly x.size() elements (none when empty), then __uninitialized_copy x's range into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D08A0, 104 bytes (+ int3 padding), `ret 4`.
// Callers: 0x004D1238, 0x004D1859, 0x004D2043 (vector-map copies in the same TU).
// Callees: CRT malloc 0x00BFEA0E (`add esp,4`); __uninitialized_copy(const pair*, const pair*, pair*, const __false_type&)
//   0x004CFAC0 (`__fastcall`, first ecx / last edx, result + tag on the stack: the key dword then the six CCameraTag
//   words per element; its `_Uninit_copy<...unsigned_int...>` label is propagated).
// Label correction: the catalog's `Vector_Copy_FixedStruct_0x1c` is descriptive; the element is the 28-byte pair of the
//   landed CCameraTagVector::_M_insert_overflow 0x004D1360 (same TU).
// Shape: the 0x004D0830 copy constructor with a 28-byte element: x.size() is the 0x92492493 reciprocal divide, the
//   zero register is loaded with `mov eax,0` so the `n != 0` flags of the divide survive for `allocate(n)`'s test;
//   the empty `__false_type()` tag is homed in the dead `x` argument slot (`lea ecx,[esp+10h]`).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

namespace _STL {

struct __false_type { __false_type() { } };

enum EConversationProtagonist { PROTAGONIST_NONE };
enum EConversationCameraPosition { CAMERA_POSITION_NONE };
enum EConversationCameraFocus { CAMERA_FOCUS_NONE };
enum EConversationCameraZoomType { CAMERA_ZOOM_NONE };
enum EConversationCameraView { CAMERA_VIEW_NONE };

struct CCameraTag {                                    // CSubtitledMessage::CCameraTag (24 bytes, Ego_r.pdb)
    EConversationProtagonist    CameraPositionPerson;  // +0x00
    EConversationCameraPosition CameraPosition;        // +0x04
    EConversationProtagonist    CameraFocusPerson;     // +0x08
    EConversationCameraFocus    CameraFocusPosition;   // +0x0c
    EConversationCameraZoomType CameraZoomType;        // +0x10
    EConversationCameraView     CameraView;            // +0x14
};

template <class T1, class T2> struct pair {     // STLport pair: memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<float, CCameraTag> SCameraTagEntry;     // 28 bytes

SCameraTagEntry* __fastcall __uninitialized_copy(const SCameraTagEntry* first, const SCameraTagEntry* last,
                                                 SCameraTagEntry* result, const __false_type&);   // 0x004CFAC0

template <class T> struct allocator {           // STLport allocator<T>: empty
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(size_t n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

template <class T> struct _Vector_base {
    T*                    _M_start;             // +0
    T*                    _M_finish;            // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;    // +8
    _Vector_base(size_t n, const allocator<T>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class T> struct vector : _Vector_base<T> {
    size_t size() const { return size_t(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start,
                                               __false_type());
    }
};

template vector<SCameraTagEntry>::vector(const vector<SCameraTagEntry>&);

}