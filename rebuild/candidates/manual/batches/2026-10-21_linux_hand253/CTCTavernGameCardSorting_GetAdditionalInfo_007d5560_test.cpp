#include <cstdio>
#include <cstring>
// Behaviour test for CTCTavernGameCardSorting::GetAdditionalInfo (0x007D5560): the body of src/ over stand-ins -- the
// "round n/total" text built from the def's label, the card-order count and the rounds left.
struct CWideString {
    char text[64];
    CWideString() { text[0] = 0; }
    CWideString(const char* t) { strcpy(text, t); }
};
struct CCharString {
    char text[16];
    CWideString ToWideString() const { return CWideString(text); }
};
static CCharString GFIntToCharString(long value) { CCharString s; sprintf(s.text, "%ld", value); return s; }
static CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r(a.text); strcat(r.text, b.text); return r; }
static CWideString operator+(const CWideString& a, const wchar_t* b)
{
    CWideString r(a.text);
    size_t n = strlen(r.text);
    while (*b) r.text[n++] = (char)*b++;
    r.text[n] = 0;
    return r;
}
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};
struct CCardPositionsDef { int dummy; };
static int g_releases = 0;
struct CTavernGameDef { long m_count; unsigned long AdditionalInfo; void ReleaseTemporary() { --m_count; ++g_releases; } };
struct CTavernGameCardBaseDef { long m_count; CRetailVector<CCardPositionsDef> CardOrders; void ReleaseTemporary() { --m_count; ++g_releases; } };
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    CTavernGameDef* operator->() const { return Object; }
};
struct CCardDefPointer {
    CTavernGameCardBaseDef* Object;
    CCardDefPointer() : Object(0) {}
    ~CCardDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};
static CCardPositionsDef g_orders[5];
static CTavernGameDef g_def = { 1, 42 };
static CTavernGameCardBaseDef g_cardDef = { 1, { g_orders, g_orders + 5, g_orders + 5 } };
static unsigned long g_textId = 0;
struct CTCTavernGame {
    long NumRounds;
    void GetTavernGameDef(CDefPointer* out) { out->Object = &g_def; ++g_def.m_count; }
    CWideString GetText(unsigned long id) { g_textId = id; return CWideString("Round "); }
};
struct CTCTavernGameCardBase : CTCTavernGame {
    void GetCardDef(CCardDefPointer* out) { out->Object = &g_cardDef; ++g_cardDef.m_count; }
};
struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    CWideString GetAdditionalInfo();
};

CWideString CTCTavernGameCardSorting::GetAdditionalInfo()
{
    CCardDefPointer cardDef;
    GetCardDef(&cardDef);
    CDefPointer def;
    GetTavernGameDef(&def);
    return GetText(def->AdditionalInfo) + GFIntToCharString(cardDef->CardOrders.size() - NumRounds + 1).ToWideString()
         + L"/" + GFIntToCharString(cardDef->CardOrders.size()).ToWideString();
}

static int fail(int line) { printf("007d5560_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CTCTavernGameCardSorting game;
    game.NumRounds = 5;
    CHECK(strcmp(game.GetAdditionalInfo().text, "Round 1/5") == 0);
    CHECK(g_textId == 42 && g_releases == 2 && g_def.m_count == 1 && g_cardDef.m_count == 1);
    game.NumRounds = 2;
    CHECK(strcmp(game.GetAdditionalInfo().text, "Round 4/5") == 0);
    printf("007d5560_TEST PASS\n");
    return 0;
}
