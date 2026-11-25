// Behaviour test for 0x00520570 CThingManager::LoadHeroStateScript: skips past "NewThing" (retail 0x01245A88), reads the hero's state up
// to "END_THING_LEVEL" (0x01245A78), parses it with a fresh CStringParser (filename "") and loads one thing (mode 1) from it.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings and log the text they receive, so the test asserts both literals; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CCharString { public: void* m_pData; CCharString(const char* text, long length = -1); ~CCharString(); };
class CBaseClass { public: virtual ~CBaseClass(); };
class CStringParser { public:
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset);
    CCharString ReadAsStringUntilPastString(const CCharString& terminator);
    CCharString ReadAsStringUntilString(const CCharString& terminator);
    long SkipWhitespace();
};
class CDefinitionManager; class CSymbolMap; class CThing;
class CPersistContext { public:
    CPersistContext(CStringParser& parser, void (__fastcall* errorCallback)(const CCharString&), bool reportErrors,
                    const CDefinitionManager* definitions, const CSymbolMap* symbols, CCharString* sourceText);
};
class CThingManager { private: CThing* LoadSingleThing(CPersistContext& context, long mode); public: void LoadHeroStateScript(CStringParser& parser); };
static char g_log[512]; static void* g_heroParser = 0; static void* g_context = 0; static int g_bad = 0;
static void Log(const char* tag, const void* s) { strcat(g_log, tag); strcat(g_log, "["); strcat(g_log, s ? (const char*)s : "(null)"); strcat(g_log, "]"); }
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; if (length != -1) g_bad = 1; }
CCharString::~CCharString() { }
CBaseClass::~CBaseClass() { }
CStringParser::CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset)
{ Log("parser", text.m_pData); Log("file", filename.m_pData); g_heroParser = this; if (errorCallback || baseLineOffset) g_bad = 1; }
CCharString CStringParser::ReadAsStringUntilPastString(const CCharString& terminator) { Log("past", terminator.m_pData); return CCharString("ignored", -1); }
CCharString CStringParser::ReadAsStringUntilString(const CCharString& terminator) { Log("until", terminator.m_pData); return CCharString("HERO_STATE", -1); }
long CStringParser::SkipWhitespace() { if (this != g_heroParser) g_bad = 1; strcat(g_log, "ws"); return 0; }
CPersistContext::CPersistContext(CStringParser& parser, void (__fastcall* errorCallback)(const CCharString&), bool reportErrors,
                                 const CDefinitionManager* definitions, const CSymbolMap* symbols, CCharString* sourceText)
{ if (&parser != g_heroParser || errorCallback || !reportErrors || definitions || symbols || sourceText) g_bad = 1; g_context = this; strcat(g_log, "ctx"); }
CThing* CThingManager::LoadSingleThing(CPersistContext& context, long mode) { if (&context != g_context || mode != 1) g_bad = 1; strcat(g_log, "load"); return 0; }
// The parser's comment list (STLport _List_base, out of line in the row's model): a thiscall-compatible __fastcall stand-in.
extern "C" void __fastcall NF420_ListBaseDtor(void*, int) { }
#pragma comment(linker, "/alternatename:??1?$_List_base@VCStringParserCommentBoundaries@@V?$allocator@VCStringParserCommentBoundaries@@@std@@@std@@QAE@XZ=@NF420_ListBaseDtor@8")
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static double raw[16]; CStringParser& parser = *(CStringParser*)raw; CThingManager mgr;
    mgr.LoadHeroStateScript(parser);
    bool ok = !g_bad && strcmp(g_log, "past[NewThing]until[END_THING_LEVEL]parser[HERO_STATE]file[]wsctxload") == 0;
    if (ok) printf("00520570_TEST PASS\n");
    else printf("FAIL bad=%d log=%s\n", g_bad, g_log);
    return ok ? 0 : 1;
}
