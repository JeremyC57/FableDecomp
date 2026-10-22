// NTCScriptedControl::CActionTalkToThing::CActionTalkToThing(CThing* target, unsigned long textIndex, const char* groupName, long unused, bool makeTargetListen, bool soundIn2D, bool displayOverScreenFade) -- constructor: as the text-name overload (0x00717AA0) but the one subtitled line comes from a text index, pushed into a local array that is then assigned to Messages; records whether the talk starts during a cutscene
// Leaf: ??0CActionTalkToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00717C50, length 234 bytes (+ int3 padding), `ret 1Ch`.
// Callees: CActionTalkBase::CActionTalkBase @ 0x00717950, CCharString::CCharString() @ 0x0099E4B0 (x3),
// GetMessageFromTextIndex @ 0x004CEA60 (__fastcall, CSubtitledMessage by hidden pointer), the STLport
// vector<CSubtitledMessage>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x004D2760, ~CSubtitledMessage
// @ 0x004D1DB0, CArray<CSubtitledMessage>::operator= @ 0x0071A160, free @ 0x00BFEA14 (the MSVCR71 import thunk); the player GUI
// @ 0x013B8790 (+0xf6). Vtable 0x012659C8. Layout as the 0x00717AA0 sibling.
// Shape: STLport _Vector_base layout (alloc-proxy end pointer, base dtor frees); push_back inline with its capacity test,
// which folds on the empty local because the overflow and assignment instantiations have same-TU stand-in bodies that
// take the array words by value (retail compiled those COMDATs in this TU); the __false_type tag is an uninitialised
// named local homed in the dead displayOverScreenFade slot.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14 (MSVCR71 free import thunk)

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

struct SAllocProxy {                                // STLport _STLP_alloc_proxy: the end-of-storage pointer
    CSubtitledMessage* _M_data;
    SAllocProxy(CSubtitledMessage* p) : _M_data(p) { }
};
struct SVectorBase {                                // STLport _Vector_base<CSubtitledMessage>
    CSubtitledMessage* m_first;
    CSubtitledMessage* m_last;
    SAllocProxy        m_end;
    SVectorBase() : m_first(0), m_last(0), m_end(0) { }
    ~SVectorBase()
    {
        if (m_first)
            free(m_first);
    }
};
struct CSubtitledMessageArray : SVectorBase {       // retail CArray<CSubtitledMessage>: first, last, end (12 bytes)
    ~CSubtitledMessageArray() { DestroyRange(m_first, m_last); }
    CSubtitledMessageArray& operator=(const CSubtitledMessageArray& other);   // 0x0071A160
    void M_insert_overflow(CSubtitledMessage* pos, const CSubtitledMessage& x, const __false_type& isPOD,
                           unsigned long fillLength, bool atEnd);         // 0x004D2760
    void push_back(const CSubtitledMessage& x)      // STLport vector::push_back
    {
        if (m_last != m_end._M_data) {
            new(m_last) CSubtitledMessage(x);
            ++m_last;
        } else {
            __false_type isPOD;
            M_insert_overflow(m_last, x, isPOD, 1, true);
        }
    }
};

void InsertOverflow_004d2760(CSubtitledMessage** first, CSubtitledMessage** last, CSubtitledMessage** end,
                             CSubtitledMessage* pos, const CSubtitledMessage& x, unsigned long fillLength, bool atEnd);
void AssignWords_0071a160(CSubtitledMessageArray* to, CSubtitledMessage* first, CSubtitledMessage* last,
                          CSubtitledMessage* end);
// Same-TU stand-ins for the two out-of-line template instantiations (0x004D2760 and 0x0071A160; retail compiled their
// COMDAT bodies in this TU and kept another copy at link): the array words go by value, so VC7.1 knows the local array
// does not escape and folds the push_back capacity test on it.
#pragma auto_inline(off)
__declspec(noinline) void CSubtitledMessageArray::M_insert_overflow(CSubtitledMessage* pos, const CSubtitledMessage& x,
                                                                    const __false_type&, unsigned long fillLength,
                                                                    bool atEnd)
{
    CSubtitledMessage* first = m_first;
    CSubtitledMessage* last = m_last;
    CSubtitledMessage* end = m_end._M_data;
    InsertOverflow_004d2760(&first, &last, &end, pos, x, fillLength, atEnd);
    m_first = first;
    m_last = last;
    m_end._M_data = end;
}
__declspec(noinline) CSubtitledMessageArray& CSubtitledMessageArray::operator=(const CSubtitledMessageArray& other)
{
    AssignWords_0071a160(this, other.m_first, other.m_last, other.m_end._M_data);
    return *this;
}
#pragma auto_inline(on)

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