// Behaviour test for 004b00c0: src/004b00c0.cpp copied verbatim; a section name is registered when it is "NULL" or its
// quest part (the text before any '%') is one of RegisteredQuestNames.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQuestManager::IsQuestRegistered(const CCharString& name) -- true for "NULL" or when the quest name (the part before any '%') is in RegisteredQuestNames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B00C0, length 154 bytes (padded to 160), `ret 4`.
// Callees: CCharString::FindCharPos(char) @ 0x0099E5A0, CCharString::GetSubString(unsigned long, unsigned long) const
// @ 0x0099EC70, CCharString::CCharString(const CCharString&) @ 0x0099EC30, CBasicString<char>::operator==(const char*)
// @ 0x004115A0 ("NULL" @ 0x012393E4), STLport `__find(first, last, val, random_access_iterator_tag)` over CCharString
// @ 0x004B8FF0 (`__fastcall`: first in ecx, last in edx), CCharString::~CCharString @ 0x0099EAE0.
// Layout: retail RegisteredQuestNames (STLport vector<CCharString>) at +0x2c/+0x30 (landed GetRegisteredQuestFromIndex).
// Shape: the quest name comes from an inline by-value helper returning either GetSubString(0, pos) or a copy, both
//   constructed straight into the local. VC keeps that local in the dead argument slot once __find has a same-TU
//   `auto_inline(off)` stand-in body that only reads `val`. The body's down-counting loop takes edx, so `last` is kept in
//   esi across the call as retail. The inline CCharString::operator==(const char*) null arm is
//   `if (strcmp(text, "") == 0) return true; return false;` (branches off repe cmpsb, the true exit tail-merged);
//   `find(...) != end` gives `setne bl`.
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)
struct CBasicStringChar {
    bool operator==(const char* text) const;       // 0x004115A0
};
class CCharString {
public:
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    long FindCharPos(char c) const;                // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return *PStringData == text;
    }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
#pragma auto_inline(off)
CCharString* __fastcall __find(CCharString* first, CCharString* last, const CCharString& val, const random_access_iterator_tag&)   // 0x004B8FF0 (same-TU stand-in: reads only)
{
    const char* key = val.PStringData ? *(const char* const*)val.PStringData : "";
    for (long n = last - first; n > 0; --n, ++first) {
        const char* text = first->PStringData ? *(const char* const*)first->PStringData : "";
        if (strcmp(text, key) == 0)
            return first;
    }
    return first;
}
#pragma auto_inline(on)
inline CCharString* find(CCharString* first, CCharString* last, const CCharString& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}
inline CCharString GetQuestNameFromSectionName(const CCharString& name)   // the part before any '%'
{
    long pos = name.FindCharPos('%');
    if (pos > -1)
        return name.GetSubString(0, pos);
    return name;
}
class CQuestManager { public:
    unsigned char _pad_0x00[0x2c];
    CCharString*  RegisteredQuestNames_start;      // +0x2c
    CCharString*  RegisteredQuestNames_finish;     // +0x30
    bool IsQuestRegistered(const CCharString& name) const;
};
bool CQuestManager::IsQuestRegistered(const CCharString& name) const
{
    CCharString questName = GetQuestNameFromSectionName(name);
    if (questName == "NULL")
        return true;
    return find(RegisteredQuestNames_start, RegisteredQuestNames_finish, questName) != RegisteredQuestNames_finish;
}
// ---- end source ----
struct Block { char* PData; char buf[64]; };
static Block g_pool[32]; static int g_used, g_copies, g_dtors;
static CBasicStringChar* NewBlock(const char* text, unsigned long count)
{
    Block* b = &g_pool[g_used++]; strncpy(b->buf, text, count); b->buf[count] = 0; b->PData = b->buf; return (CBasicStringChar*)b;
}
static const char* Text(const CCharString& s) { return s.PStringData ? *(const char* const*)s.PStringData : ""; }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
long CCharString::FindCharPos(char c) const { const char* t = Text(*this); const char* p = strchr(t, c); return p ? (long)(p - t) : -1; }
CCharString CCharString::GetSubString(unsigned long start, unsigned long count) const
{
    CBasicStringChar* block = NewBlock(Text(*this) + start, count);
    return *(const CCharString*)&block;
}
bool CBasicStringChar::operator==(const char* text) const { return strcmp(*(const char* const*)this, text) == 0; }
static const CCharString& Str(CBasicStringChar*& slot) { return *(const CCharString*)&slot; }
int main()
{
    static CBasicStringChar* names[3];
    names[0] = NewBlock("Q_Alpha", 7); names[1] = NewBlock("Q_Beta", 6); names[2] = 0;
    static long raw[0x34 / 4]; memset(raw, 0, sizeof(raw));
    CQuestManager& qm = *(CQuestManager*)raw;
    qm.RegisteredQuestNames_start = (CCharString*)&names[0]; qm.RegisteredQuestNames_finish = (CCharString*)&names[2];
    static CBasicStringChar* q[6];
    q[0] = NewBlock("Q_Beta", 6); q[1] = NewBlock("Q_Alpha%Section2", 16); q[2] = NewBlock("Q_Gamma", 7);
    q[3] = NewBlock("NULL", 4); q[4] = NewBlock("Q_Gamma%Q_Alpha", 15); q[5] = 0;
    CHECK(qm.IsQuestRegistered(Str(q[0])) == true);
    CHECK(qm.IsQuestRegistered(Str(q[1])) == true);
    CHECK(qm.IsQuestRegistered(Str(q[2])) == false);
    CHECK(qm.IsQuestRegistered(Str(q[3])) == true);
    CHECK(qm.IsQuestRegistered(Str(q[4])) == false);
    CHECK(qm.IsQuestRegistered(Str(q[5])) == false);      // unset name: compares as "" (not "NULL", not registered)
    CHECK(g_dtors == 6 && g_copies == 6);                   // one local per call (the stub GetSubString copies too)
    if (fails == 0) printf("004b00c0_TEST PASS\n"); else printf("004b00c0_TEST FAIL\n");
    return fails;
}