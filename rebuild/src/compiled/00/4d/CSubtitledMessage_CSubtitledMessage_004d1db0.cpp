// CSubtitledMessage::~CSubtitledMessage() -- implicit destructor: free the camera, animation (destroying each tag's CCharString) and attitude tag vector maps, then Narrator and Text
// Leaf: ??1CSubtitledMessage@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
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

#include "engine/CArray.h"

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