// Behaviour test for 0x0040F7E0 CPersistTraits<float>::TransferIn: a symbol is looked up in the context's symbol map (an unknown one
// reports "Unable to find symbol " (retail 0x0122DFE8) + its name through PersistError), anything else is read as a float.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings, so the test asserts the error message the row builds; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CCharString { public: void* PStringData; ~CCharString(); };
CCharString __fastcall operator+(const char* left, const CCharString& right);
enum EParsedItemType { PARSED_ITEM_SYMBOL = 2 };
class CStringParser { public: CCharString ReadAsString(); EParsedItemType PeekNextItemType(); float ReadAsFloat(); };
class CSymbolMap { public: bool FindSymbol(const CCharString& name, long& value) const; };
class CPersistContext { public: void* PDefinitionManager; CSymbolMap* PSymbolMap; void PersistError(const CStringParser& parser, const CCharString& message) const; };
template <class T> struct CPersistTraits { T DefaultVal; void TransferIn(CPersistContext& context, CStringParser& parser, T* out) const; };
static char g_msg[128]; static char g_cat[128]; static int g_errors = 0; static const void* g_errParser = 0;
CCharString::~CCharString() { }
CCharString __fastcall operator+(const char* left, const CCharString& right)
{ CCharString r; strcpy(g_cat, left); strcat(g_cat, (const char*)right.PStringData); r.PStringData = g_cat; return r; }
void CPersistContext::PersistError(const CStringParser& parser, const CCharString& message) const
{ strcpy(g_msg, (const char*)message.PStringData); ++g_errors; g_errParser = &parser; }
static int g_type = 2; static bool g_known = false; static char g_name[] = "SYM_X";
CCharString CStringParser::ReadAsString() { CCharString r; r.PStringData = g_name; return r; }
EParsedItemType CStringParser::PeekNextItemType() { return (EParsedItemType)g_type; }
float CStringParser::ReadAsFloat() { return 2.5f; }
bool CSymbolMap::FindSymbol(const CCharString& name, long& value) const { if (strcmp((const char*)name.PStringData, "SYM_X") != 0) return false; if (g_known) value = 7; return g_known; }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CSymbolMap map; CPersistContext ctx; ctx.PDefinitionManager = 0; ctx.PSymbolMap = &map; CStringParser parser;
    CPersistTraits<float> traits; traits.DefaultVal = 0.0f; float out = -1.0f;
    traits.TransferIn(ctx, parser, &out);                       // unknown symbol
    bool ok = g_errors == 1 && strcmp(g_msg, "Unable to find symbol SYM_X") == 0 && g_errParser == &parser;
    g_known = true; traits.TransferIn(ctx, parser, &out);       // known symbol
    ok = ok && g_errors == 1 && out == 7.0f;
    g_type = 0; traits.TransferIn(ctx, parser, &out);           // a number
    ok = ok && g_errors == 1 && out == 2.5f;
    if (ok) printf("0040f7e0_TEST PASS\n");
    else printf("FAIL errors=%d msg=%s out=%f\n", g_errors, g_msg, out);
    return ok ? 0 : 1;
}
