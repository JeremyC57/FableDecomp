// Grade: DIFFER(283v237) -- retail folds the inline STLport push_back capacity test on the fresh local array (straight to _M_insert_overflow with pos 0 from edi); ours keeps the test and the in-place copy path, which also shifts the zero register to ebx | tried: inline push_back on a default-constructed local | next: lane gap: dead-argument-slot temporary (the __false_type tag in the soundIn2D slot) plus the folded capacity test -- try the overflow call written directly
// NTCScriptedControl::CActionTalkToThing::CActionTalkToThing(CThing* target, unsigned long textIndex, const char*
// groupName, long unused, bool makeTargetListen, bool soundIn2D, bool displayOverScreenFade) -- constructor: as the
// text-name overload (0x00717AA0) but the one subtitled line comes from a text index (pushed into a local array).
// Leaf: ??0CActionTalkToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00717C50, length 234 bytes (+ int3 padding), `ret 1Ch`.
// Callees: CActionTalkBase::CActionTalkBase @ 0x00717950, CCharString::CCharString() @ 0x0099E4B0 (x3),
// GetMessageFromTextIndex @ 0x004CEA60 (__fastcall, CSubtitledMessage by hidden pointer), the STLport
// vector<CSubtitledMessage>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x004D2760 (push_back on the empty
// local: the capacity test folds away), ~CSubtitledMessage @ 0x004D1DB0, CArray<CSubtitledMessage>::operator=
// @ 0x0071A160, operator delete @ 0x00BFEA14; the player GUI @ 0x013B8790 (+0xf6). Vtable 0x012659C8.
// Shape: the __false_type tag temporary lives in the dead soundIn2D argument slot; the fourth argument is unused;
// the local array's destructor caches first/last in ebp/ebx. Layout as the 0x00717AA0 sibling.
void __cdecl operator_delete(void* p);             // 0x00BFEA14 (operator delete)

struct CThing;

struct CCharString {
    void* m_pData;
    CCharString();                                  // 0x0099E4B0
};

struct CSubtitledMessage {                          // 0x44 bytes
    unsigned char _body[0x44];
    CSubtitledMessage(const CSubtitledMessage& other);
    ~CSubtitledMessage();                           // 0x004D1DB0 (manifest "~CPatchTesselationEdgeStrip" propagated)
};

inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { };

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
            operator_delete(m_first);
    }
    CSubtitledMessageArray& operator=(const CSubtitledMessageArray& other);   // 0x0071A160
    void M_insert_overflow(CSubtitledMessage* pos, const CSubtitledMessage& x, const __false_type& isPOD,
                           unsigned long fillLength, bool atEnd);         // 0x004D2760
    void push_back(const CSubtitledMessage& x)      // STLport vector::push_back
    {
        if (m_last != m_end) {
            new(m_last) CSubtitledMessage(x);
            ++m_last;
        } else {
            M_insert_overflow(m_last, x, __false_type(), 1, true);
        }
    }
};

CSubtitledMessage __fastcall GetMessageFromTextIndex(unsigned long textIndex, const char* groupName);   // 0x004CEA60

struct CPlayerGui {
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
    CActionTalkToThing(CThing* target, unsigned long textIndex, const char* groupName, long unused,
                       bool makeTargetListen, bool soundIn2D, bool displayOverScreenFade);
};

CActionTalkToThing::CActionTalkToThing(CThing* target, unsigned long textIndex, const char* groupName, long unused,
                                       bool makeTargetListen, bool soundIn2D, bool displayOverScreenFade)
    : CActionTalkBase(target, makeTargetListen, true, soundIn2D)
    , AskingHeroQuestion(false)
    , DisplayOverScreenFade(displayOverScreenFade)
{
    CSubtitledMessageArray messages;
    messages.push_back(GetMessageFromTextIndex(textIndex, groupName));
    Messages = messages;
    AddedDuringCutscene = g_pPlayerGui->m_inCutscene;
}
