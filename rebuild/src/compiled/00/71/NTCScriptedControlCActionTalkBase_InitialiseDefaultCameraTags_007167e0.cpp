// NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags() -- fill the static default conversation camera-tag list
// Leaf: ?InitialiseDefaultCameraTags@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007167E0, 842 bytes, `?InitialiseDefaultCameraTags@CActionTalkBase@NTCScriptedControl@@CIXXZ`
//   (private static __fastcall, no arguments, plain `ret`).
// Label correction: catalog "CActionTalkBase_InitMorphs_Hardcoded" -> NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags
//   (Ego_r.pdb name at the donor address; static DefaultCameraTags @ 0x013BAD18 per the landed 0x007196D0 notes).
// Callees: vector<CSubtitledMessage::CCameraTag>::_M_insert_overflow 0x007196D0 (thiscall, the push_back overflow).
// Shape: seven inlined STLport push_back calls of a 24-byte camera tag built in one stack temporary: the six enum
//   values are loaded into registers, stored to the temporary and (when there is room) stored again through the
//   finish pointer (placement-new null check, then `_M_finish += 24` re-read from memory); otherwise
//   _M_insert_overflow(finish, tag, __false_type(), 1, true). The register-held fields need the growth path's body in
//   the TU (same-TU noinline _M_insert_overflow + its two algorithm callees, as retail compiled them here): with only a
//   declaration the temporary escapes and the finish copy is re-read from the frame (890v842; a const-reference or a
//   user copy constructor does not help).
// Enumerator values: installed-game misc_def_types.h (header_enums.csv); the CONVERSATION_* prefixes are descriptive.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

// misc_def_types.h (installed game headers)
enum EConversationProtagonist { CONVERSATION_PROTAGONIST_NONE = 0, CONVERSATION_PROTAGONIST_SPEAKER = 1, CONVERSATION_PROTAGONIST_SPEAKEE = 2 };
enum EConversationCameraPosition {
    CONVERSATION_CAMERA_IN_FRONT_OF_FACE = 0, CONVERSATION_CAMERA_LEFT_SHOULDER = 1, CONVERSATION_CAMERA_RIGHT_SHOULDER = 2,
    CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_NEAR = 3, CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_FAR = 4,
    CONVERSATION_CAMERA_BEHIND_RIGHT_SHOULDER_NEAR = 5, CONVERSATION_CAMERA_BEHIND_RIGHT_SHOULDER_FAR = 6,
    CONVERSATION_CAMERA_LEFT_OF_TORSO_NEAR = 7, CONVERSATION_CAMERA_LEFT_OF_TORSO_FAR = 8,
    CONVERSATION_CAMERA_RIGHT_OF_TORSO_NEAR = 9, CONVERSATION_CAMERA_RIGHT_OF_TORSO_FAR = 10,
    CONVERSATION_CAMERA_POS_LEFT = 11, CONVERSATION_CAMERA_POS_RIGHT = 12
};
enum EConversationCameraFocus {
    CONVERSATION_CAMERA_FOCUS_FACE = 0, CONVERSATION_CAMERA_FOCUS_10 = 1, CONVERSATION_CAMERA_FOCUS_25 = 2,
    CONVERSATION_CAMERA_FOCUS_33 = 3, CONVERSATION_CAMERA_FOCUS_50 = 4, CONVERSATION_CAMERA_FOCUS_HIGH = 5,
    CONVERSATION_CAMERA_FOCUS_LEVEL = 6, CONVERSATION_CAMERA_FOCUS_LOW = 7
};
enum EConversationCameraZoomType { CONVERSATION_CAMERA_NO_ZOOM = 0, CONVERSATION_CAMERA_SLOW_ZOOM = 1, CONVERSATION_CAMERA_FAST_ZOOM = 2 };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_WIDE = 0, CONVERSATION_CAMERA_VIEW_MEDIUM = 1, CONVERSATION_CAMERA_VIEW_CLOSE = 2 };

class CSubtitledMessage {
public:
    class CCameraTag {                                  // Ego_r.pdb, 24 bytes
    public:
        CCameraTag(EConversationProtagonist positionPerson, EConversationCameraPosition position,
                   EConversationProtagonist focusPerson, EConversationCameraFocus focus,
                   EConversationCameraZoomType zoomType, EConversationCameraView view)
            : CameraPositionPerson(positionPerson), CameraPosition(position), CameraFocusPerson(focusPerson),
              CameraFocusPosition(focus), CameraZoomType(zoomType), CameraView(view) {}
        EConversationProtagonist CameraPositionPerson;  // +0x00
        EConversationCameraPosition CameraPosition;     // +0x04
        EConversationProtagonist CameraFocusPerson;     // +0x08
        EConversationCameraFocus CameraFocusPosition;   // +0x0C
        EConversationCameraZoomType CameraZoomType;     // +0x10
        EConversationCameraView CameraView;             // +0x14
    };
};

namespace std {
struct __false_type { __false_type() {} };

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

// Same-TU bodies of this TU's out-of-line STLport members (landed: 0x00718390, 0x007183E0, 0x007196D0). With the
// growth path visible VC7.1 proves the push_back temporary only feeds reads, so the six tag fields stay in registers.
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
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x007196D0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};

#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
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
#pragma auto_inline(on)
}

namespace NTCScriptedControl {
class CActionTalkBase {
public:
    static std::vector<CSubtitledMessage::CCameraTag> DefaultCameraTags;    // 0x013BAD18
private:
    static void __fastcall InitialiseDefaultCameraTags();
};

void __fastcall CActionTalkBase::InitialiseDefaultCameraTags()
{
    typedef CSubtitledMessage::CCameraTag T;
    DefaultCameraTags.push_back(T(CONVERSATION_PROTAGONIST_SPEAKEE, CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_NEAR, CONVERSATION_PROTAGONIST_SPEAKER,
                                  CONVERSATION_CAMERA_FOCUS_FACE, CONVERSATION_CAMERA_NO_ZOOM, CONVERSATION_CAMERA_VIEW_MEDIUM));
    DefaultCameraTags.push_back(T(CONVERSATION_PROTAGONIST_SPEAKEE, CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_NEAR, CONVERSATION_PROTAGONIST_SPEAKER,
                                  CONVERSATION_CAMERA_FOCUS_FACE, CONVERSATION_CAMERA_NO_ZOOM, CONVERSATION_CAMERA_VIEW_MEDIUM));
    DefaultCameraTags.push_back(T(CONVERSATION_PROTAGONIST_SPEAKER, CONVERSATION_CAMERA_IN_FRONT_OF_FACE, CONVERSATION_PROTAGONIST_SPEAKER,
                                  CONVERSATION_CAMERA_FOCUS_FACE, CONVERSATION_CAMERA_NO_ZOOM, CONVERSATION_CAMERA_VIEW_MEDIUM));
    DefaultCameraTags.push_back(T(CONVERSATION_PROTAGONIST_SPEAKER, CONVERSATION_CAMERA_BEHIND_LEFT_SHOULDER_NEAR, CONVERSATION_PROTAGONIST_SPEAKEE,
                                  CONVERSATION_CAMERA_FOCUS_FACE, CONVERSATION_CAMERA_NO_ZOOM, CONVERSATION_CAMERA_VIEW_MEDIUM));
    DefaultCameraTags.push_back(T(CONVERSATION_PROTAGONIST_SPEAKER, CONVERSATION_CAMERA_BEHIND_RIGHT_SHOULDER_NEAR, CONVERSATION_PROTAGONIST_SPEAKEE,
                                  CONVERSATION_CAMERA_FOCUS_FACE, CONVERSATION_CAMERA_NO_ZOOM, CONVERSATION_CAMERA_VIEW_MEDIUM));
    DefaultCameraTags.push_back(T(CONVERSATION_PROTAGONIST_SPEAKER, CONVERSATION_CAMERA_IN_FRONT_OF_FACE, CONVERSATION_PROTAGONIST_SPEAKER,
                                  CONVERSATION_CAMERA_FOCUS_FACE, CONVERSATION_CAMERA_NO_ZOOM, CONVERSATION_CAMERA_VIEW_MEDIUM));
    DefaultCameraTags.push_back(T(CONVERSATION_PROTAGONIST_NONE, CONVERSATION_CAMERA_POS_LEFT, CONVERSATION_PROTAGONIST_NONE,
                                  CONVERSATION_CAMERA_FOCUS_LEVEL, CONVERSATION_CAMERA_NO_ZOOM, CONVERSATION_CAMERA_VIEW_MEDIUM));
}
}