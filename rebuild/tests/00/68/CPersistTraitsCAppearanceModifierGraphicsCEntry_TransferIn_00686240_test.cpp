// Behaviour test for 0x00686240 CPersistTraits<CAppearanceModifierGraphics::CEntry>::TransferIn: reads "(long,float,long,long,float,long)";
// its TU-mate float traits (0x0040F7E0's body) reports an unknown symbol as "Unable to find symbol " (retail 0x0122DFE8) + its name.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings, so the test asserts the six fields and the error message; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
template <class T> class CBasicString { public: char* PData; unsigned long StringLength; bool operator==(const char* text) const; };
class CCharString { public: CBasicString<char>* PStringData; ~CCharString(); bool operator==(const char* text) const; };
CCharString __fastcall operator+(const char* left, const CCharString& right);
class CStringParser { public: long PeekNextItemType(); CCharString ReadAsSymbol(); CCharString ReadAsString(); float ReadAsFloat(); };
class CSymbolMap { public: bool Lookup(const CCharString& name, long* value) const; };
class CPersistContext { public: void* PDefinitionManager; CSymbolMap* PSymbolMap; void PersistError(const CStringParser& parser, const CCharString& message) const; };
class CPersistTraitsSLONG { public: static void __fastcall TransferIn(CPersistContext& context, CStringParser& parser, long& value); };
class CAppearanceModifierGraphics { public: struct CEntry { long Long00; float Float04; long Long08; long Long0C; float Float10; long Long14; }; };
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CAppearanceModifierGraphics::CEntry> {
    void TransferIn(CPersistContext* context, CStringParser* parser, CAppearanceModifierGraphics::CEntry* value);
};
static char g_msg[128]; static char g_cat[128]; static int g_errors = 0;
static CBasicString<char> g_catStr;
CCharString::~CCharString() { }
CCharString __fastcall operator+(const char* left, const CCharString& right)
{ CCharString r; strcpy(g_cat, left); strcat(g_cat, right.PStringData->PData); g_catStr.PData = g_cat; r.PStringData = &g_catStr; return r; }
void CPersistContext::PersistError(const CStringParser& parser, const CCharString& message) const { strcpy(g_msg, message.PStringData->PData); ++g_errors; }
template <> bool CBasicString<char>::operator==(const char* text) const { return strcmp(PData, text) == 0; }
static const char* kSyms[] = { "(", ",", ",", ",", ",", ",", ")" }; static int g_sym = 0, g_float = 0; static long g_next = 0;
static CBasicString<char> g_symStr[8], g_nameStr; static char g_name[] = "MISSING";
CCharString CStringParser::ReadAsSymbol() { CCharString r; g_symStr[g_sym].PData = (char*)kSyms[g_sym]; r.PStringData = &g_symStr[g_sym]; ++g_sym; return r; }
long CStringParser::PeekNextItemType() { return g_float++ == 0 ? 2 : 0; }    // the first float is an unknown symbol
CCharString CStringParser::ReadAsString() { CCharString r; g_nameStr.PData = g_name; r.PStringData = &g_nameStr; return r; }
float CStringParser::ReadAsFloat() { return 1.5f; }
bool CSymbolMap::Lookup(const CCharString&, long*) const { return false; }
void __fastcall CPersistTraitsSLONG::TransferIn(CPersistContext&, CStringParser&, long& value) { value = ++g_next * 10; }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CSymbolMap map; CPersistContext ctx; ctx.PDefinitionManager = 0; ctx.PSymbolMap = &map; CStringParser parser;
    CPersistTraits<CAppearanceModifierGraphics::CEntry> traits; CAppearanceModifierGraphics::CEntry e; memset(&e, 0, sizeof(e));
    traits.TransferIn(&ctx, &parser, &e);
    bool ok = g_errors == 1 && strcmp(g_msg, "Unable to find symbol MISSING") == 0 && g_sym == 7
        && e.Long00 == 10 && e.Long08 == 20 && e.Long0C == 30 && e.Float10 == 1.5f && e.Long14 == 40;
    if (ok) printf("00686240_TEST PASS\n");
    else printf("FAIL errors=%d msg=%s syms=%d %ld %ld %ld %f %ld\n", g_errors, g_msg, g_sym, e.Long00, e.Long08, e.Long0C, e.Float10, e.Long14);
    return ok ? 0 : 1;
}
