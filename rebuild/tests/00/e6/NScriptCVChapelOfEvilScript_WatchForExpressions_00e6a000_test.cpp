// Behaviour test for 00e6a000: src/00e6a000.cpp copied verbatim; polls once per script frame while the thread is not
// terminating, asking for a performed expression only while the hero is inside the chapel.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CV_ChapelOfEvilScript::WatchForExpressions() -- every script frame, while the hero is inside the chapel, poll for a performed expression (the retail branch body only re-checks thread termination)
// Inline CCharString ==(""): the null-data arm is `if (strcmp(text, "") == 0) return true; return false;`, so VC7.1 branches straight off the repe cmpsb flags (`return strcmp(text, "") == 0;` materialises a sete al).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6A000, length 168 bytes (176 with padding).
// Callees: CCharString::CCharString() @ 0x0099E4B0, ~CCharString @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
// @ 0x00CB7940 (landed), Compare(const char*, const char*) @ 0x00411570 (landed, `__fastcall`, the inlined
// CBasicString<char>::operator==), and through the game script interface (+0x40): NewScriptFrame (slot 7, +0x1c),
// MsgOnExpressionPerformed(CCharString&) (slot 43, +0xac). Literal "" @ 0x0122D70E.
// Identity: the catalog label stands; InsideChapel is +0x59 (Ego_r.pdb CV_ChapelOfEvilScript +0x6d less 0x14).
// The real method name is from the manifest; what the original did with the expression is gone from the retail body:
// only `expression == ""` survives (null block: the strcmp intrinsic over the pooled empty literal; else the block's
// inlined operator== calling Compare and `? false : true`, whose result byte is homed in the frame).
// Shape: one CCharString local for the whole loop; `while (!terminating) { if (InsideChapel && Msg(expr)) {...}
// NewScriptFrame(); }`.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CGameScriptInterface { public:                       // CGameScriptInterface, retail vtable @ 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual bool MsgOnExpressionPerformed(CCharString& expression) const;   // slot 43 (+0xac) CGameScriptInterface::MsgOnExpressionPerformed @ 0x00893B00
};

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

struct CV_ChapelOfEvilScript : CQuestScriptBase {
    unsigned char _pad48[0x11];                             // +0x48 (donation members)
    bool InsideChapel;                                      // +0x59
    void WatchForExpressions();
};

void CV_ChapelOfEvilScript::WatchForExpressions()
{
    CCharString expression;
    while (!IsActiveThreadTerminating()) {
        if (InsideChapel && GSI->MsgOnExpressionPerformed(expression)) {
            if (IsActiveThreadTerminating())
                return;
            if (expression == "") {
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        GSI->NewScriptFrame();
    }
}
// ---- end source ----
static int g_frames, g_msgs, g_checks, g_limit, g_ctors, g_dtors; static bool g_reply; static CCharStringData* g_setTo;
CCharString::CCharString() : PStringData(0) { ++g_ctors; }
CCharString::~CCharString() { ++g_dtors; }
bool CScriptBase::IsActiveThreadTerminating() { return ++g_checks > g_limit; }
struct TestGSI : CGameScriptInterface {
    void v000() {} void v001() {} void v002() {} void v003() {} void v004() {} void v005() {} void v006() {}
    void NewScriptFrame() const { ++g_frames; }
    void v008() {} void v009() {} void v010() {} void v011() {} void v012() {} void v013() {} void v014() {} void v015() {} void v016() {} void v017() {}
    void v018() {} void v019() {} void v020() {} void v021() {} void v022() {} void v023() {} void v024() {} void v025() {} void v026() {} void v027() {}
    void v028() {} void v029() {} void v030() {} void v031() {} void v032() {} void v033() {} void v034() {} void v035() {} void v036() {} void v037() {}
    void v038() {} void v039() {} void v040() {} void v041() {} void v042() {}
    bool MsgOnExpressionPerformed(CCharString& e) const { ++g_msgs; e.PStringData = g_setTo; return g_reply; }
};
void CGameScriptInterface::v000() {} void CGameScriptInterface::v001() {} void CGameScriptInterface::v002() {} void CGameScriptInterface::v003() {}
void CGameScriptInterface::v004() {} void CGameScriptInterface::v005() {} void CGameScriptInterface::v006() {} void CGameScriptInterface::NewScriptFrame() const {}
void CGameScriptInterface::v008() {} void CGameScriptInterface::v009() {} void CGameScriptInterface::v010() {} void CGameScriptInterface::v011() {}
void CGameScriptInterface::v012() {} void CGameScriptInterface::v013() {} void CGameScriptInterface::v014() {} void CGameScriptInterface::v015() {}
void CGameScriptInterface::v016() {} void CGameScriptInterface::v017() {} void CGameScriptInterface::v018() {} void CGameScriptInterface::v019() {}
void CGameScriptInterface::v020() {} void CGameScriptInterface::v021() {} void CGameScriptInterface::v022() {} void CGameScriptInterface::v023() {}
void CGameScriptInterface::v024() {} void CGameScriptInterface::v025() {} void CGameScriptInterface::v026() {} void CGameScriptInterface::v027() {}
void CGameScriptInterface::v028() {} void CGameScriptInterface::v029() {} void CGameScriptInterface::v030() {} void CGameScriptInterface::v031() {}
void CGameScriptInterface::v032() {} void CGameScriptInterface::v033() {} void CGameScriptInterface::v034() {} void CGameScriptInterface::v035() {}
void CGameScriptInterface::v036() {} void CGameScriptInterface::v037() {} void CGameScriptInterface::v038() {} void CGameScriptInterface::v039() {}
void CGameScriptInterface::v040() {} void CGameScriptInterface::v041() {} void CGameScriptInterface::v042() {}
bool CGameScriptInterface::MsgOnExpressionPerformed(CCharString&) const { return false; }
static void Run(CV_ChapelOfEvilScript& s, int limit) { g_checks = 0; g_frames = 0; g_msgs = 0; g_limit = limit; s.WatchForExpressions(); }
int main()
{
    TestGSI gsi;
    static long raw[0x5c / 4]; memset(raw, 0, sizeof(raw));
    CV_ChapelOfEvilScript& s = *(CV_ChapelOfEvilScript*)raw;
    s.GSI = &gsi;
    // outside the chapel: 3 frames, no polls
    s.InsideChapel = false; Run(s, 3);
    CHECK(g_frames == 3 && g_msgs == 0 && g_checks == 4 && g_ctors == 1 && g_dtors == 1);
    // inside, no expression: poll each frame
    s.InsideChapel = true; g_reply = false; Run(s, 3);
    CHECK(g_frames == 3 && g_msgs == 3 && g_checks == 4);
    // inside, expression performed with a named expression: two termination checks per frame
    static char tWave[] = "Wave"; CCharStringData d; memset(&d, 0, sizeof(d)); d.Data.PData = tWave; d.Data.StringLength = 4;
    g_reply = true; g_setTo = &d; Run(s, 5);
    CHECK(g_msgs == 3 && g_frames == 2 && g_checks == 6);   // the third poll sees termination right after the message
    // expression with a null block counts as "": three checks per frame
    g_setTo = 0; Run(s, 6);
    CHECK(g_msgs == 2 && g_frames == 2 && g_checks == 7);
    // termination seen right after the message returns early (no frame)
    Run(s, 1);
    CHECK(g_msgs == 1 && g_frames == 0 && g_checks == 2);
    CHECK(g_ctors == 5 && g_dtors == 5);
    if (fails == 0) printf("00e6a000_TEST PASS\n"); else printf("00e6a000_TEST FAIL\n");
    return fails;
}