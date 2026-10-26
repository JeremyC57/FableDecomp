// NTCScriptedControl::CActionTalkToThing::CActionTalkToThing(CThing* target, const char* textName, const char* groupName,
// long selectionMethod, bool makeTargetListen, bool soundIn2D, bool displayOverScreenFade) -- constructor: the talk
// base (camera tags on), own vtable, the question state, the subtitled lines looked up by text name, and whether the
// talk starts during a cutscene.
// Leaf: ??0CActionTalkToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00717AA0, length 219 bytes, `ret 1Ch`.
// Callees: CActionTalkBase::CActionTalkBase @ 0x00717950, CCharString::CCharString() @ 0x0099E4B0 (three buttons),
// GetMessagesFromTextName @ 0x004CEBB0 (__fastcall, the local array first), CArray<CSubtitledMessage>::operator=
// @ 0x0071A160 (manifest "CCombatWheel::CopyAssign" propagated), ~CSubtitledMessage @ 0x004D1DB0 (0x44-byte
// elements), operator delete @ 0x00BFEA14; the player GUI @ 0x013B8790 (+0xf6 cutscene flag). Vtable 0x012659C8 =
// NTCScriptedControl::CActionTalkToThing.
// Layout: Ego_r.pdb CActionTalkToThing minus 8 (the retail CArray is 12 bytes): AddedDuringCutscene +0x90,
// AskingHeroQuestion +0x91, DisplayOverScreenFade +0x92, QuestionButton1..3 +0x94/+0x98/+0x9c.
// Shape: the local array's inline ctor/dtor (a by-value _Destroy(first, last) range helper -- the end pointer cached
// in ebp -- with `add edi,44h`, then free); the cutscene flag stored
// after the assignment, before the local's destructor.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CThing;

class CCharString { public:
    void* m_pData;
    CCharString();                                  // 0x0099E4B0
};

class CSubtitledMessage { public:                          // 0x44 bytes
    unsigned char _body[0x44];
    ~CSubtitledMessage();                           // 0x004D1DB0 (manifest "~CPatchTesselationEdgeStrip" propagated)
};

inline void DestroyRange(CSubtitledMessage* first, CSubtitledMessage* last)   // std::_Destroy(first, last)
{
    for (; first != last; ++first)
        first->~CSubtitledMessage();
}

struct CSubtitledMessageArray {                     // retail CArray<CSubtitledMessage>: first, last, end (12 bytes)
    CSubtitledMessage* m_first;
    CSubtitledMessage* m_last;
    CSubtitledMessage* m_end;
    CSubtitledMessageArray() : m_first(0), m_last(0), m_end(0) { }
    ~CSubtitledMessageArray()
    {
        DestroyRange(m_first, m_last);
        if (m_first)
            free(m_first);
    }
    CSubtitledMessageArray& operator=(const CSubtitledMessageArray& other);   // 0x0071A160
};

void __fastcall GetMessagesFromTextName(CSubtitledMessageArray* out, const char* textName, const char* groupName,
                                        long selectionMethod);   // 0x004CEBB0

class CPlayerGui { public:
    unsigned char _pad[0xf6];
    bool m_inCutscene;                              // +0xf6
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

struct CActionTalkBase {                            // NTCScriptedControl::CActionTalkBase (0x90 bytes in retail)
    virtual void v00();
    unsigned char _pad_0x04[0x2c];
    CSubtitledMessageArray Messages;                // +0x30
    unsigned char _pad_0x3c[0x90 - 0x3c];
    CActionTalkBase(CThing* target, bool makeTargetListen, bool useCameraTags, bool soundIn2D);   // 0x00717950
};

struct CActionTalkToThing : CActionTalkBase {       // vtable 0x012659C8
    bool        AddedDuringCutscene;                // +0x90
    bool        AskingHeroQuestion;                 // +0x91
    bool        DisplayOverScreenFade;              // +0x92
    CCharString QuestionButton1;                    // +0x94
    CCharString QuestionButton2;                    // +0x98
    CCharString QuestionButton3;                    // +0x9c
    virtual void v00();
    CActionTalkToThing(CThing* target, const char* textName, const char* groupName, long selectionMethod,
                       bool makeTargetListen, bool soundIn2D, bool displayOverScreenFade);
};

CActionTalkToThing::CActionTalkToThing(CThing* target, const char* textName, const char* groupName, long selectionMethod,
                                       bool makeTargetListen, bool soundIn2D, bool displayOverScreenFade)
    : CActionTalkBase(target, makeTargetListen, true, soundIn2D)
    , AskingHeroQuestion(false)
    , DisplayOverScreenFade(displayOverScreenFade)
{
    CSubtitledMessageArray messages;
    GetMessagesFromTextName(&messages, textName, groupName, selectionMethod);
    Messages = messages;
    AddedDuringCutscene = g_pPlayerGui->m_inCutscene;
}