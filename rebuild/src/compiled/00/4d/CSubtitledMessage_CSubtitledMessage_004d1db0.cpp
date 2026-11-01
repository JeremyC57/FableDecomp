// CSubtitledMessage::~CSubtitledMessage() -- implicit destructor: free the camera, animation (destroying each tag's CCharString) and attitude tag vector maps, then Narrator and Text
// Leaf: ??1CSubtitledMessage@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D1DB0, 98 bytes, thiscall, `jmp` tail; 22 call sites + 1 jmp.
// Identity (proven): the callers are CSubtitledMessage's: GetMessageFromTextIndex 0x004CEA60 and GetMessagesFromTextName
//   0x004CEBB0 (temporaries), the vector<CSubtitledMessage> helpers _Destroy_range 0x004D26F0, _M_clear 0x004D2720,
//   _M_insert_overflow 0x004D2760, erase 0x0071A110, operator= 0x0071A160, _M_fill_insert 0x0071A2F0, resize 0x0071A630
//   and the NTCScriptedControl::CActionTalkBase / CActionTalkToThing constructors and ~CActionTalkBase 0x0071A430 (Ego_r.pdb
//   CActionTalkBase::Messages, CArray<CSubtitledMessage>); the landed rows already bind it as CSubtitledMessage's dtor.
//   Ego_r.pdb ??1CSubtitledMessage@@QAE@XZ (donor 0x004F0770), non-virtual. The body tears down exactly Ego_r's
//   CSubtitledMessage members in reverse: CameraTags +0x34, AnimationTags +0x24 (8-byte pair<float, CCharString>, name at
//   +4), AttitudeTags +0x14, Narrator +0x10, Text +0x0C (the retail record is 0x44: 16-byte STLport CVectorMaps).
//   The manifest label (~CPatchTesselationEdgeStrip, a 44-byte CBaseClassNonCopyable with no string members) is
//   propagated over the shape and wrong.
// Callees: free 0x00BFEA14 (_free, STLport malloc allocator deallocate), CCharString::~CCharString 0x0099EAE0,
//   CWideString::~CWideString 0x0099B510 (??1CWideString@@QAE@XZ, tail).
// Shape: STLport 4.6 ~vector = _Destroy(_M_start, _M_finish) (the __false_type loop; empty for the trivially destructible
//   float/enum pairs) then ~_Vector_base's null-tested deallocate; every level is inline, the members' order gives the
//   +0x34 / +0x24 / +0x14 sequence and the string dtors close it. Matched on the first probe.
// Layout: no engine/CSubtitledMessage.h; the Ego_r.pdb member list with retail STLport sizes (vector 0xc, CVectorMap 0x10).
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14

namespace std {
// STLport 4.6 stl/_construct.h, stl/_alloc.h, stl/_vector.h, stl/_pair.h (the members this row needs)
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
}

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                             // 0x0099EAE0
    CCharStringData* PStringData;
};
class CWideString {
public:
    ~CWideString();                                             // 0x0099B510
    unsigned short* PData;
};

template <class T> class CArray : public std::vector<T> { };   // Lionhead CArray over STLport vector (0xc)
template <class K, class V> class CKeyPairCompareLess { };      // empty comparator
template <class K, class V, class C> class CVectorMap {         // 0x10 in retail
public:
    CArray<std::pair<K, V> > Vector;                            // +0x00
    C                        Compare;                           // +0x0c
    bool                     Dirty;                             // +0x0d
};

enum EConversationAttitude { };
enum EConversationProtagonist { };
enum EConversationCameraPosition { };
enum EConversationCameraFocus { };
enum EConversationCameraZoomType { };
enum EConversationCameraView { };
class CASoundBank;

class CSubtitledMessage {                                       // Ego_r.pdb members; 0x44 bytes in retail
public:
    class CCameraTag {                                          // Ego_r.pdb, 24 bytes
    public:
        EConversationProtagonist    CameraPositionPerson;       // +0x00
        EConversationCameraPosition CameraPosition;             // +0x04
        EConversationProtagonist    CameraFocusPerson;          // +0x08
        EConversationCameraFocus    CameraFocusPosition;        // +0x0c
        EConversationCameraZoomType CameraZoomType;             // +0x10
        EConversationCameraView     CameraView;                 // +0x14
    };
    CASoundBank* PSpeechBank;                                   // +0x00
    long         SpeechIndex;                                   // +0x04
    bool         Speech3D;                                      // +0x08
    CWideString  Text;                                          // +0x0c
    CCharString  Narrator;                                      // +0x10
    CVectorMap<float, EConversationAttitude, CKeyPairCompareLess<float, EConversationAttitude> > AttitudeTags;   // +0x14
    CVectorMap<float, CCharString, CKeyPairCompareLess<float, CCharString> >                     AnimationTags;  // +0x24
    CVectorMap<float, CCameraTag, CKeyPairCompareLess<float, CCameraTag> >                       CameraTags;     // +0x34
};

void EmitSubtitledMessageDestructor(CSubtitledMessage* message) // emits the implicit ??1; not a retail function
{
    message->CSubtitledMessage::~CSubtitledMessage();
}