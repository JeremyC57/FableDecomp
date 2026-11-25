// Behaviour test for 0x009E2590 CPersistTraitsSLONG::TransferIn: a symbol is looked up (an unknown one reports "Unable to find symbol "
// (retail 0x0122DFE8) + its name), a number is read as an integer, and a following | + - << >> combines a recursive right-hand side.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings, so the test asserts the error message and the operator texts the row tests; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CCharString { public: void* PStringData; CCharString(const char* text, long length); ~CCharString(); };
CCharString __fastcall operator+(const char* left, const CCharString& right);
enum EParsedItemType { PARSED_ITEM_SYMBOL = 2 };
class CStringParser { public: long ReadAsInteger(); EParsedItemType PeekNextItemType(); CCharString ReadAsString(); bool SkipPastNextItemIfIs(const CCharString& item); };
class CSymbolMap { public: void UpdateDirtyQuick() const; bool FindSymbol(const CCharString& name, long& value) const; };
class CPersistContext { public: void* PDefinitionManager; CSymbolMap* PSymbolMap; void PersistError(const CStringParser& parser, const CCharString& message) const; };
class CPersistTraitsSLONG { public: static void __fastcall TransferIn(CPersistContext& context, CStringParser& parser, long& value); };
static char g_msg[128]; static char g_cat[128]; static int g_errors = 0; static char g_log[256]; static int g_dirty = 0, g_peek = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; if (length != -1) strcat(g_log, "LEN!"); }
CCharString::~CCharString() { }
CCharString __fastcall operator+(const char* left, const CCharString& right)
{ strcpy(g_cat, left); strcat(g_cat, (const char*)right.PStringData); return CCharString(g_cat, -1); }
void CPersistContext::PersistError(const CStringParser&, const CCharString& message) const { strcpy(g_msg, (const char*)message.PStringData); ++g_errors; }
long CStringParser::ReadAsInteger() { return 5; }
EParsedItemType CStringParser::PeekNextItemType() { return (EParsedItemType)(g_peek++ == 0 ? 2 : 0); }
CCharString CStringParser::ReadAsString() { return CCharString("NOPE", -1); }
static int g_skips = 0;
bool CStringParser::SkipPastNextItemIfIs(const CCharString& item) { strcat(g_log, "["); strcat(g_log, (const char*)item.PStringData); strcat(g_log, "]"); return ++g_skips == 2; }
void CSymbolMap::UpdateDirtyQuick() const { ++g_dirty; }
bool CSymbolMap::FindSymbol(const CCharString&, long&) const { return false; }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CSymbolMap map; CPersistContext ctx; ctx.PDefinitionManager = 0; ctx.PSymbolMap = &map; CStringParser parser; long v = -1;
    CPersistTraitsSLONG::TransferIn(ctx, parser, v);     // unknown symbol (0), then "+" and a right-hand side 5
    bool ok = g_errors == 1 && strcmp(g_msg, "Unable to find symbol NOPE") == 0 && g_dirty == 1 && v == 5
        && strcmp(g_log, "[|][+][|][+][-][<<][>>]") == 0;
    if (ok) printf("009e2590_TEST PASS\n");
    else printf("FAIL errors=%d msg=%s v=%ld log=%s\n", g_errors, g_msg, v, g_log);
    return ok ? 0 : 1;
}
