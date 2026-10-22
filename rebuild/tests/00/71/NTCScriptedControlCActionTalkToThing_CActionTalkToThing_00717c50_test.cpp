// Behaviour test for 00717c50: src/00717c50.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// NTCScriptedControl::CActionTalkToThing::CActionTalkToThing(...)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs ----
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
enum { L_BASE = 1, L_STR, L_MSG, L_OVF, L_MSGDTOR, L_ASSIGN, L_DELETE };
static void* g_baseThis; static CThing* g_baseTarget; static bool g_baseListen, g_baseCam, g_base2D;
CActionTalkBase::CActionTalkBase(CThing* target, bool makeTargetListen, bool useCameraTags, bool soundIn2D)
{
    Log(L_BASE); g_baseThis = this; g_baseTarget = target; g_baseListen = makeTargetListen; g_baseCam = useCameraTags;
    g_base2D = soundIn2D;
}
void CActionTalkBase::v00() { }
void CActionTalkToThing::v00() { }
static void* g_strThis[3]; static int g_strN = 0;
CCharString::CCharString() { Log(L_STR); if (g_strN < 3) g_strThis[g_strN] = this; ++g_strN; m_pData = 0; }
static unsigned long g_msgIndex; static const char* g_msgGroup; static void* g_msgTemp;
CSubtitledMessage __fastcall GetMessageFromTextIndex(unsigned long textIndex, const char* groupName)
{
    Log(L_MSG); g_msgIndex = textIndex; g_msgGroup = groupName;
    static unsigned char raw[sizeof(CSubtitledMessage)];
    memset(raw, 0x5a, sizeof(raw));
    return *(CSubtitledMessage*)raw;   // copy-constructed into the caller's temporary, no local to destroy
}
CSubtitledMessage::CSubtitledMessage(const CSubtitledMessage& other) { memcpy(_body, other._body, sizeof(_body)); g_msgTemp = this; }
static void* g_dtorThis[8]; static int g_dtorN = 0;
CSubtitledMessage::~CSubtitledMessage() { Log(L_MSGDTOR); if (g_dtorN < 8) g_dtorThis[g_dtorN] = this; ++g_dtorN; }
static CSubtitledMessage* g_ovfIn[3]; static CSubtitledMessage* g_ovfPos; static const CSubtitledMessage* g_ovfX;
static unsigned long g_ovfN; static bool g_ovfEnd; static CSubtitledMessage* g_buf;
void InsertOverflow_004d2760(CSubtitledMessage** first, CSubtitledMessage** last, CSubtitledMessage** end,
                             CSubtitledMessage* pos, const CSubtitledMessage& x, unsigned long fillLength, bool atEnd)
{
    Log(L_OVF); g_ovfIn[0] = *first; g_ovfIn[1] = *last; g_ovfIn[2] = *end; g_ovfPos = pos; g_ovfX = &x;
    g_ovfN = fillLength; g_ovfEnd = atEnd;
    g_buf = (CSubtitledMessage*)malloc(sizeof(CSubtitledMessage));
    memcpy(g_buf, &x, sizeof(CSubtitledMessage));
    *first = g_buf; *last = g_buf + 1; *end = g_buf + 1;
}
static CSubtitledMessageArray* g_assignTo; static CSubtitledMessage* g_assignW[3];
void AssignWords_0071a160(CSubtitledMessageArray* to, CSubtitledMessage* first, CSubtitledMessage* last, CSubtitledMessage* end)
{
    Log(L_ASSIGN); g_assignTo = to; g_assignW[0] = first; g_assignW[1] = last; g_assignW[2] = end;
}
static void* g_deleted; static int g_deleteN = 0;
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;

int main()
{
    static unsigned char buf[0xa0];
    memset(buf, 0xcd, sizeof(buf));
    g_gui.m_inCutscene = true;
    CThing* target = (CThing*)0x1234;
    CActionTalkToThing* p = new(buf) CActionTalkToThing(target, 77, "GRP", 99, true, false, true);
    CHECK(p == (CActionTalkToThing*)buf);
    CHECK(g_baseThis == buf && g_baseTarget == target && g_baseListen == true && g_baseCam == true && g_base2D == false);
    CHECK(buf[0x91] == 0 && buf[0x92] == 1 && buf[0x90] == 1);
    CHECK(g_strN == 3 && g_strThis[0] == buf + 0x94 && g_strThis[1] == buf + 0x98 && g_strThis[2] == buf + 0x9c);
    CHECK(g_msgIndex == 77 && strcmp(g_msgGroup, "GRP") == 0);
    CHECK(g_ovfIn[0] == 0 && g_ovfIn[1] == 0 && g_ovfIn[2] == 0 && g_ovfPos == 0 && g_ovfN == 1 && g_ovfEnd == true);
    CHECK(g_ovfX->_body[0] == 0x5a && g_ovfX->_body[0x43] == 0x5a);
    CHECK(g_assignTo == (CSubtitledMessageArray*)(buf + 0x30));
    CHECK(g_assignW[0] == g_buf && g_assignW[1] == g_buf + 1 && g_assignW[2] == g_buf + 1);
    // the temporary line is destroyed after the overflow insert, the local's element after the assignment, then freed
    CHECK(g_dtorN == 2 && g_dtorThis[0] == (void*)g_ovfX && g_dtorThis[1] == (void*)g_buf);
    int want[] = { L_BASE, L_STR, L_STR, L_STR, L_MSG, L_OVF, L_MSGDTOR, L_ASSIGN, L_MSGDTOR };
    CHECK(g_n == 9);
    for (int i = 0; i < 9 && i < g_n; ++i)
        CHECK(g_log[i] == want[i]);
    // not in a cutscene: the flag follows the GUI
    g_gui.m_inCutscene = false; g_n = 0; g_strN = 0; g_dtorN = 0; g_deleteN = 0;
    new(buf) CActionTalkToThing(target, 5, "X", 0, false, true, false);
    CHECK(buf[0x90] == 0 && buf[0x92] == 0 && g_base2D == true && g_baseListen == false);
    char out[64];
    _snprintf(out, sizeof(out), "00717c50_TEST %s", fails ? "FAIL" : "PASS");
    printf("%s\n", out);
    return fails ? 1 : 0;
}