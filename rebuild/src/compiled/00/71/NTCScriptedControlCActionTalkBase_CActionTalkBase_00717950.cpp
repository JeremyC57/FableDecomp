// NTCScriptedControl::CActionTalkBase::CActionTalkBase(CThing& target, bool makeTargetListen, bool useCameraTags,
// bool soundIn2D) -- constructor: the common action members zeroed, own vtable, the target's script thing, the
// conversation state reset (times zero, tag indices -1, attitude 1), and the one-time morph table set-up.
// Grade: RELOCATION_MATCH under /Oa (retry wave 2 sec2: the lander's flag sweep -- /Oa lets VC7.1 keep the UpdateCounter store after the two time copies as retail; was DIFFER(198v198) at plain /O2). Respelled to the PDB: NTCScriptedControl::CActionTalkBase protected ctor taking CThing& (Ego_r ??0CActionTalkBase@NTCScriptedControl@@IAE@AAVCThing@@_N11@Z), CThing::GetScriptThing const, InitialiseDefaultCameraTags (0x007167E0); shared CBaseClassNonCopyable / CScriptThing / CCountedPointer headers.
// Leaf: ??0CActionTalkBase@NTCScriptedControl@@IAE@AAVCThing@@_N11@Z
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00717950, length 198 bytes, `ret 10h`.
// Callees: CBase::CBase @ 0x0099A2F0, CThing::GetScriptThing @ 0x004C7E50 (constructed straight into PTargetThing
// through the hidden result pointer), CActionTalkBase::InitMorphs @ 0x007167E0 (static; manifest
// "CActionTalkBase_InitMorphs_Hardcoded") guarded by the static flag byte @ 0x01380FB9; the zero time @ 0x0122ED70
// (a double loaded with `fld qword`, so an extern constant rather than a 0.0 literal). Vtable 0x012659A8 =
// NTCScriptedControl::CActionTalkBase.
// Layout: Ego_r.pdb CActionTalkBase with the retail 12-byte CArray (every member from Messages on sits 4 lower):
// PTargetThing +0x0c, UpdateCounter +0x18, TimeCurrentLineStarted +0x20, TimeCurrentLineLasts +0x28, Messages +0x30,
// CurrentMessageIndex +0x3c, Conversation +0x40, GameTurnDuration +0x48, the six tag/gesture indices +0x4c..+0x60,
// CameraToBeMovedDueToMovedSetupPositionsCount +0x64, PreviousConversationAttitude +0x68, the eight flags +0x6c..+0x73,
// ThingLookingFromPos +0x74, ThingLookingAtPos +0x80, the four flags +0x8c..+0x8f.
// Shape: the landed NTCScriptedControl ctor model (novtable CAction intermediate, ebx-zero stores, -1 from one
// `or eax,-1`); the static flag cleared before the call.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CScriptThing.h"

class CThing {
public:
    CScriptThing GetScriptThing() const;            // 0x004C7E50 (Ego_r ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ)
};

class C3DVector { public: float X, Y, Z; };

class CSubtitledMessage;
template <class T> class CArray {                   // retail CArray<T>: first, last, end (12 bytes)
public:
    T* m_first;
    T* m_last;
    T* m_end;
    CArray() : m_first(0), m_last(0), m_end(0) { }
};

// The 0.0 time inits read the merged double constant __real@0000000000000000 @ 0x0122ED70 (three conflicting
//   descriptive bindings exist for that address; a literal needs no name).
class CTCScriptedControl;

namespace NTCScriptedControl {
class CScriptConversation;

class __declspec(novtable) CActionBase : public CBaseClassNonCopyable {   // abstract: its own vtable store is dead
public:
    CTCScriptedControl* PTCScript;                  // +0x04
    bool                Activated;                  // +0x08
    bool                Terminated;                 // +0x09
protected:
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) { }
};

class CActionTalkBase : public CActionBase {        // vtable 0x012659A8
public:
    CScriptThing                          PTargetThing;            // +0x0c
    long                                  UpdateCounter;           // +0x18
    double                                TimeCurrentLineStarted;  // +0x20
    double                                TimeCurrentLineLasts;    // +0x28
    CArray<CSubtitledMessage>             Messages;                // +0x30
    long                                  CurrentMessageIndex;     // +0x3c
    CCountedPointer<CScriptConversation>  Conversation;            // +0x40
    long                   GameTurnDuration;        // +0x48
    long                   NextAttitudeTagIndex;    // +0x4c
    long                   NextAnimationTagIndex;   // +0x50
    long                   NextCameraTagIndex;      // +0x54
    long                   LastActionPriority;      // +0x58
    long                   FramesUntilNextGesture;  // +0x5c
    long                   LastGestureAnimIndex;    // +0x60
    long                   CameraToBeMovedDueToMovedSetupPositionsCount;   // +0x64
    long                   PreviousConversationAttitude;   // +0x68 EConversationAttitude
    bool                   AddedConversationMode;   // +0x6c
    bool                   FarEnoughAwayForFacingCamera;   // +0x6d
    bool                   MakeTargetListen;        // +0x6e
    bool                   UseCameraTags;           // +0x6f
    bool                   CameraTagsWereUsed;      // +0x70
    bool                   LookingAtListener;       // +0x71
    bool                   SoundIn2D;               // +0x72
    bool                   SoundStartedPlaying;     // +0x73
    C3DVector              ThingLookingFromPos;     // +0x74
    C3DVector              ThingLookingAtPos;       // +0x80
    bool                   ThingLookingPosSet;      // +0x8c
    bool                   ParentIsDemonDoor;       // +0x8d
    bool                   GettingUp;               // +0x8e
    bool                   SkippedCurrentLine;      // +0x8f
    static bool DefaultCameraTagsNeedInitialising;  // 0x01380FB9 (descriptive: no PDB name)
    virtual ~CActionTalkBase();                     // slot 0 (Ego_r ??1CActionTalkBase@NTCScriptedControl@@UAE@XZ)
protected:
    CActionTalkBase(CThing& target, bool makeTargetListen, bool useCameraTags, bool soundIn2D);
private:
    static void __fastcall InitialiseDefaultCameraTags();   // 0x007167E0 (Ego_r ?InitialiseDefaultCameraTags@CActionTalkBase@NTCScriptedControl@@CIXXZ)
};

CActionTalkBase::CActionTalkBase(CThing& target, bool makeTargetListen, bool useCameraTags, bool soundIn2D)
    : PTargetThing(target.GetScriptThing())
    , UpdateCounter(0)
    , TimeCurrentLineStarted(0.0)
    , TimeCurrentLineLasts(0.0)
    , Messages()
    , CurrentMessageIndex(0)
    , Conversation()
    , GameTurnDuration(0)
    , NextAttitudeTagIndex(-1)
    , NextAnimationTagIndex(-1)
    , NextCameraTagIndex(-1)
    , LastActionPriority(-1)
    , FramesUntilNextGesture(-1)
    , LastGestureAnimIndex(-1)
    , CameraToBeMovedDueToMovedSetupPositionsCount(0)
    , PreviousConversationAttitude(1)
    , AddedConversationMode(false)
    , FarEnoughAwayForFacingCamera(false)
    , MakeTargetListen(makeTargetListen)
    , UseCameraTags(useCameraTags)
    , CameraTagsWereUsed(false)
    , LookingAtListener(false)
    , SoundIn2D(soundIn2D)
    , SoundStartedPlaying(false)
    , ThingLookingPosSet(false)
    , ParentIsDemonDoor(false)
    , GettingUp(false)
    , SkippedCurrentLine(false)
{
    if (DefaultCameraTagsNeedInitialising) {
        DefaultCameraTagsNeedInitialising = false;
        InitialiseDefaultCameraTags();
    }
}
}   // namespace NTCScriptedControl