// Behaviour test for 00d38aa0: the model below is a copy of src/00d38aa0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

namespace std {
template <class T> struct vector {                      // retail STLport vector, 12 bytes (vector<bool> is byte-based)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
}

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQ_FireHeartScript : CScriptBase {
    void*         GSI;                                  // +0x40
    void*         PMasterData;                          // +0x44
    unsigned char _pad048[0x24];                        // +0x48 .. +0x6c (mission flags, cutscene maps, prophet counts)
    long StartNumberOfSuns;                             // +0x6c
    long NumberOfRounds;                                // +0x70
    long NumberOfPatterns;                              // +0x74
    long NumberOfProphets;                              // +0x78
    long NumberOfPads;                                  // +0x7c
    long NumberOfSuns;                                  // +0x80
    unsigned char _pad084[0x74];                        // +0x84 .. +0xf8 (round state, pad effects, pattern flags)
    std::vector<bool> CommentGivenThisRound;            // +0xf8
    void TriggerProphetComment(const CCharString& comment);   // 0x00D38400
    void DecNumberOfSuns();
};
void CQ_FireHeartScript::DecNumberOfSuns()
{
    if (--NumberOfSuns == 2) {
        if (IsActiveThreadTerminating())
            return;
        if (!CommentGivenThisRound[3]) {
            if (IsActiveThreadTerminating())
                return;
            TriggerProphetComment("NEARLYDEAD");
            CommentGivenThisRound[3] = true;
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        if (StartNumberOfSuns - NumberOfSuns == 2 && NumberOfSuns > StartNumberOfSuns - NumberOfSuns) {
            if (IsActiveThreadTerminating())
                return;
            if (!CommentGivenThisRound[1]) {
                if (IsActiveThreadTerminating())
                    return;
                TriggerProphetComment("NEWMOONS");
                CommentGivenThisRound[1] = true;
            }
        }
    }
}
}

// ---- test doubles and driver ----
static int g_termCalls = 0, g_termAt = 1000, g_comments = 0, g_ctors = 0, g_dtors = 0;
static char g_last[32];
CCharString::CCharString(const char* text, long length) { ++g_ctors; PStringData = (void*)text; (void)length; }
CCharString::~CCharString() { ++g_dtors; }
bool NScript::CScriptBase::IsActiveThreadTerminating() { ++g_termCalls; return g_termCalls > g_termAt; }
void NScript::CQ_FireHeartScript::TriggerProphetComment(const CCharString& comment)
{
    ++g_comments; std::strcpy(g_last, (const char*)comment.PStringData);
}

static bool Run(long suns, long start, int termAt, const bool* given, int expectComments, const char* expectText,
                const bool* expectGiven)
{
    bool flags[4];
    std::memcpy(flags, given, 4);
    NScript::CQ_FireHeartScript s;
    s.GSI = 0; s.PMasterData = 0;
    s.NumberOfPads = 10; s.NumberOfSuns = suns; s.StartNumberOfSuns = start;
    s.CommentGivenThisRound._M_start = flags; s.CommentGivenThisRound._M_finish = flags + 4; s.CommentGivenThisRound._M_end_of_storage = flags + 4;
    g_termCalls = 0; g_termAt = termAt; g_comments = 0; g_ctors = g_dtors = 0; g_last[0] = 0;
    s.DecNumberOfSuns();
    if (s.NumberOfSuns != suns - 1) return false;
    if (g_comments != expectComments || g_ctors != g_dtors) return false;
    if (expectComments && std::strcmp(g_last, expectText) != 0) return false;
    return std::memcmp(flags, expectGiven, 4) == 0;
}

int main()
{
    const bool none[4] = { false, false, false, false };
    const bool f3[4] = { false, false, false, true };
    const bool f1[4] = { false, true, false, false };
    bool ok = true;
    ok = ok && Run(3, 5, 1000, none, 1, "NEARLYDEAD", f3);   // down to two suns
    ok = ok && Run(3, 5, 1000, f3, 0, "", f3);               // already said this round
    ok = ok && Run(3, 5, 1, none, 0, "", none);              // terminating before the comment
    ok = ok && Run(7, 8, 1000, none, 1, "NEWMOONS", f1);     // second new moon (8 -> 6), six suns left
    ok = ok && Run(7, 8, 2, none, 0, "", none);              // terminating at the second check
    ok = ok && Run(7, 8, 1000, f1, 0, "", f1);               // already said this round
    ok = ok && Run(5, 5, 1000, none, 0, "", none);           // first new moon: nothing
    ok = ok && Run(2, 3, 1000, none, 0, "", none);           // second new moon but only one sun left: nothing
    if (!ok) { std::printf("00d38aa0_TEST FAIL\n"); return 1; }
    std::printf("00d38aa0_TEST PASS\n");
    return 0;
}