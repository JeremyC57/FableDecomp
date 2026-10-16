// Grade: DIFFER(198v198) -- scheduling only: retail stores UpdateCounter (+0x18) after the two fld/fstp time copies and loads the useCameraTags byte between them; ours stores it first | tried: init-list order, UpdateCounter as a body assignment (moves it to the end)
// NTCScriptedControl::CActionTalkBase::CActionTalkBase(CThing* target, bool makeTargetListen, bool useCameraTags,
// bool soundIn2D) -- constructor: the common action members zeroed, own vtable, the target's script thing, the
// conversation state reset (times zero, tag indices -1, attitude 1), and the one-time morph table set-up.
// Leaf: ??0CActionTalkBase
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
struct CThing;

struct CBase {                                      // the action root
    virtual void v00();
    CBase();                                        // 0x0099A2F0
};

struct CScriptThing {                               // 12 bytes: vptr, PImp_Data, PImp_Info
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);        // non-POD: returned through a hidden pointer
};

struct CThing {
    CScriptThing GetScriptThing();                  // 0x004C7E50
};

struct C3DVector { float X, Y, Z; };

struct CSubtitledMessage;
struct CSubtitledMessageArray {                     // retail CArray<CSubtitledMessage>: first, last, end (12 bytes)
    CSubtitledMessage* m_first;
    CSubtitledMessage* m_last;
    CSubtitledMessage* m_end;
    CSubtitledMessageArray() : m_first(0), m_last(0), m_end(0) { }
};

struct CScriptConversation;
struct CConversationPointer {                       // CCountedPointer<CScriptConversation>
    CScriptConversation* Data;
    void* Info;
    CConversationPointer() : Data(0), Info(0) { }
};

extern const double g_zeroTime;                     // 0x0122ED70 (0.0)

struct __declspec(novtable) CAction : CBase {       // NTCScriptedControl::CActionBase: abstract, its own vtable store is dead
    void*   PTCScript;                              // +0x04 CTCScriptedControl*
    bool    Activated;                              // +0x08
    bool    Terminated;                             // +0x09
    CAction() : CBase(), PTCScript(0), Activated(false), Terminated(false) { }
};

struct CActionTalkBase : CAction {                  // vtable 0x012659A8
    CScriptThing           PTargetThing;            // +0x0c
    long                   UpdateCounter;           // +0x18
    double                 TimeCurrentLineStarted;  // +0x20
    double                 TimeCurrentLineLasts;    // +0x28
    CSubtitledMessageArray Messages;                // +0x30
    long                   CurrentMessageIndex;     // +0x3c
    CConversationPointer   Conversation;            // +0x40
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
    static bool s_MorphsNeedInit;                   // 0x01380FB9
    static void InitMorphs();                       // 0x007167E0
    virtual void v00();
    CActionTalkBase(CThing* target, bool makeTargetListen, bool useCameraTags, bool soundIn2D);
};

CActionTalkBase::CActionTalkBase(CThing* target, bool makeTargetListen, bool useCameraTags, bool soundIn2D)
    : CAction()
    , PTargetThing(target->GetScriptThing())
    , UpdateCounter(0)
    , TimeCurrentLineStarted(g_zeroTime)
    , TimeCurrentLineLasts(g_zeroTime)
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
    if (s_MorphsNeedInit) {
        s_MorphsNeedInit = false;
        InitMorphs();
    }
}
