// Behaviour test for 006238a0: src/006238a0.cpp copied verbatim; test quests order case-insensitively by display name,
// an empty (null block or "") display name falling back to the quest name.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTestQuestNameCompare::operator()(const CTestQuest* a, const CTestQuest* b) -- orders test quests by display name (falling back to the quest name when the display name is empty), case-insensitively
// Leaf: operator()
// Retail: Fable.exe (TLC, VC7.1) @ 0x006238A0, 225 bytes. `ret 8`, bool in al.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30; CBasicString<char>::operator==(const char*) 0x004115A0
//          (on the data block, inside the inline CCharString::operator==; "" @ 0x0122D70E); CCharString::operator= 0x0099EFB0; _stricmp 0x00BFEBA8
//          (`__cdecl`, `add esp,8`); CCharString::~CCharString 0x0099EAE0. The empty-text global @ 0x0129AAF4.
// Shape: each name is a CCharString copy of DisplayName (+0x0c) in a frame word (the second in the dead `a` slot); the
//        natural `if (name == "") name = quest->QuestName;` with the inline CCharString ==("") whose null arm is
//        `if (strcmp(text, "") == 0) return true; return false;`. 0x004115A0 and Compare get their landed-equivalent
//        bodies as same-TU stand-ins, so VC knows the compare writes no memory. nameB's block then stays in esi across it,
//        and its C string is threaded on every path as retail. The C strings are the null-first `PStringData ? Data : ""`
//        reads (b first, then a); the compare result rides in bl across the two destructors.
// Layout: Ego_r.pdb CTestQuest (0x1c): QuestName +0x00, HSPName +0x04, ColourIndex +0x08, DisplayName +0x0c.
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body, same-TU stand-in)
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
bool CBasicStringChar::operator==(const char* text) const   // 0x004115A0 (same-TU stand-in: reads only)
{
    return Compare_00411570(PData, text) ? false : true;
}
#pragma auto_inline(on)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
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
class CTestQuest { public:
    CCharString QuestName;                      // +0x00
    CCharString HSPName;                        // +0x04
    long        ColourIndex;                    // +0x08
    CCharString DisplayName;                    // +0x0c
};
class CTestQuestNameCompare { public:
    bool operator()(const CTestQuest* a, const CTestQuest* b) const;
};
bool CTestQuestNameCompare::operator()(const CTestQuest* a, const CTestQuest* b) const
{
    CCharString nameA(a->DisplayName);
    if (nameA == "")
        nameA = a->QuestName;
    CCharString nameB(b->DisplayName);
    if (nameB == "")
        nameB = b->QuestName;
    return _stricmp(nameA.GetPString(), nameB.GetPString()) < 0;
}
// ---- end source ----
static int g_copies, g_dtors, g_assigns;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; ++g_assigns; return *this; }
static CCharStringData MakeData(char* text) { CCharStringData d; memset(&d, 0, sizeof(d)); d.Data.PData = text; d.Data.StringLength = (unsigned long)strlen(text); return d; }
static CTestQuest* MakeQuest(long* raw, CCharStringData* quest, CCharStringData* display)
{
    memset(raw, 0, 0x1c); CTestQuest* q = (CTestQuest*)raw; q->QuestName.PStringData = quest; q->DisplayName.PStringData = display; return q;
}
int main()
{
    static char tA[] = "Q_Apple", tB[] = "Q_banana", dA[] = "zebra", dB[] = "Apple pie", tE[] = "";
    CCharStringData qa = MakeData(tA), qb = MakeData(tB), da = MakeData(dA), db = MakeData(dB), de = MakeData(tE);
    long r1[7], r2[7];
    CTestQuestNameCompare less;
    CTestQuest* a = MakeQuest(r1, &qa, &da); CTestQuest* b = MakeQuest(r2, &qb, &db);
    CHECK(less(a, b) == false && less(b, a) == true);        // "zebra" vs "Apple pie"
    a->DisplayName.PStringData = 0;                          // falls back to "Q_Apple" < "Apple pie"? no: 'Q' > 'A'
    CHECK(less(a, b) == false && less(b, a) == true);
    b->DisplayName.PStringData = &de;                        // "" -> "Q_banana"; "Q_Apple" < "Q_banana" (case-insensitive)
    CHECK(less(a, b) == true && less(b, a) == false);
    CHECK(less(a, a) == false);
    CHECK(g_copies == g_dtors && g_copies == 14 && g_assigns > 0);
    if (fails == 0) printf("006238a0_TEST PASS\n"); else printf("006238a0_TEST FAIL\n");
    return fails;
}