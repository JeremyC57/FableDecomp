// NTCScriptedControl::CActionTalkBase::~CActionTalkBase() -- scripted talk action dtor (compiler-generated): the member dtors in reverse order (the conversation counted pointer, the subtitled message array, the target script thing) and the CActionBase / CBaseClassNonCopyable chain
// Leaf: ??1CActionTalkBase@NTCScriptedControl@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A430, 161 bytes, tail jump to the root destructor.
// Identity: the callers are 0x0071A530, slot 0 (scalar deleting dtor) of vtable 0x012659A8 with RTTI
//   `.?AVCActionTalkBase@NTCScriptedControl@@`, the CActionTalkToThing dtor 0x0071A570 (vtable 0x012659C8; it destroys its own
//   three strings and tail-jumps here) and the CActionOverheadTalkToThing scalar deleting dtor 0x0071A5A0 (vtable 0x012659FC);
//   rtti_bases: CActionTalkToThing / CActionOverheadTalkToThing : CActionTalkBase : CTCScriptedControl::CActionBase :
//   CBaseClassNonCopyable. Ego_r.pdb has `??1CActionTalkBase@NTCScriptedControl@@UAE@XZ`, and its CActionTalkBase layout matches
//   every offset used here. No vtable store of its own: the destructor is implicit. The catalog label
//   `C3DMeshStripifier::~C3DMeshStripifier` is wrong (C3DMeshStripifier is 0x1C bytes: two counted pointers and two scalars).
// Callees: operator delete 0x00BFE9BC (the counted pointers' info blocks), CSubtitledMessage::~CSubtitledMessage 0x004D1DB0
//   (Ego_r `??1CSubtitledMessage@@QAE@XZ`, bound so; one call per 0x44-byte element), free 0x00BFEA14 (the array block),
//   ~CBaseClass 0x0099A2E0 (PTargetThing's base, out of line), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (tail
//   jump; CTCScriptedControl::CActionBase's dtor is implicit too). Virtual call: each count block's DeleteFunc.
// Layout: Ego_r.pdb CActionTalkBase (0x98) with retail sizes (CArray = STLport vector, 0xC; CSubtitledMessage 0x44 with
//   16-byte CVectorMaps): CActionBase (PTCScript +0x04, Activated +0x08, Terminated +0x09), PTargetThing +0x0C (CScriptThing:
//   vptr 0x01238C8C, PImp +0x10), UpdateCounter +0x18, TimeCurrentLineStarted +0x20, TimeCurrentLineLasts +0x28, Messages
//   +0x30, CurrentMessageIndex +0x3C, Conversation +0x40, GameTurnDuration +0x48 ... SkippedCurrentLine +0x8F. No engine header.
// Shape: the implicit destructor, emitted out of line through a qualified explicit destructor call in a one-line helper
//   (EmitImplicitDestructor, not a retail function); CCountedPointer's inline release (`ReleaseData(); Data = 0; Info = 0;`);
//   STLport ~vector (_Destroy the range, then ~_Vector_base frees the block); the inline ~CScriptThing (vptr store, then its
//   counted pointer and ~CBaseClass).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
class CSubtitledMessage { public:                           // retail 0x44 bytes, destroyed out of line
    unsigned char   _data[0x44];                            // PSpeechBank, SpeechIndex, Speech3D, Text, Narrator, three CVectorMaps (Ego_r)
    ~CSubtitledMessage();                                   // 0x004D1DB0
};
template <class T> class CArray : public std::vector<T> { public:   // Lionhead array over the STLport vector (retail 0xC)
};
enum EConversationAttitude { };                             // values not needed here
class C3DVector { public:
    float X;
    float Y;
    float Z;
};
class CTCScriptedControl { public:
    class CActionBase : public CBaseClassNonCopyable { public:
        CTCScriptedControl* PTCScript;                      // +0x04
        bool                Activated;                      // +0x08
        bool                Terminated;                     // +0x09
    };
};

namespace NTCScriptedControl {
class CScriptConversation;
class CActionTalkBase : public CTCScriptedControl::CActionBase { public:
    CScriptThing                            PTargetThing;                                   // +0x0C
    long                                    UpdateCounter;                                  // +0x18
    double                                  TimeCurrentLineStarted;                         // +0x20
    double                                  TimeCurrentLineLasts;                           // +0x28
    CArray<CSubtitledMessage>               Messages;                                       // +0x30
    long                                    CurrentMessageIndex;                            // +0x3C
    CCountedPointer<CScriptConversation>    Conversation;                                   // +0x40
    long                                    GameTurnDuration;                               // +0x48
    long                                    NextAttitudeTagIndex;                           // +0x4C
    long                                    NextAnimationTagIndex;                          // +0x50
    long                                    NextCameraTagIndex;                             // +0x54
    long                                    LastActionPriority;                             // +0x58
    long                                    FramesUntilNextGesture;                         // +0x5C
    long                                    LastGestureAnimIndex;                           // +0x60
    long                                    CameraToBeMovedDueToMovedSetupPositionsCount;   // +0x64
    EConversationAttitude                   PreviousConversationAttitude;                   // +0x68
    bool                                    AddedConversationMode;                          // +0x6C
    bool                                    FarEnoughAwayForFacingCamera;                   // +0x6D
    bool                                    MakeTargetListen;                               // +0x6E
    bool                                    UseCameraTags;                                  // +0x6F
    bool                                    CameraTagsWereUsed;                             // +0x70
    bool                                    LookingAtListener;                              // +0x71
    bool                                    SoundIn2D;                                      // +0x72
    bool                                    SoundStartedPlaying;                            // +0x73
    C3DVector                               ThingLookingFromPos;                            // +0x74
    C3DVector                               ThingLookingAtPos;                              // +0x80
    bool                                    ThingLookingPosSet;                             // +0x8C
    bool                                    ParentIsDemonDoor;                              // +0x8D
    bool                                    GettingUp;                                      // +0x8E
    bool                                    SkippedCurrentLine;                             // +0x8F
};
}

void EmitImplicitDestructor(NTCScriptedControl::CActionTalkBase* p) { p->NTCScriptedControl::CActionTalkBase::~CActionTalkBase(); }