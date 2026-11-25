// Behaviour test for 0x00CB8690 CScriptBase::LoadGameState: reads the text between "START_SCRIPT_DATA" / "END_SCRIPT_DATA" (retail
// 0x012C16C8 / 0x012C16B8) into the script-data persist, and between "START_ENTITY_SCRIPTS" / "END_ENTITY_SCRIPTS" (0x012C16A0 /
// 0x012C168C) into the runtime entity-script persist.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings and log the text they receive, so the test asserts all four literals; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CCharString { public: void* m_pData; CCharString(const char* text, long length); ~CCharString(); };
class CBaseClass { public: virtual ~CBaseClass(); };
class CStringParser { public:
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset);
    bool SkipPastString(const CCharString& item);
    CCharString ReadAsStringUntilString(const CCharString& end);
};
namespace NScript { struct CRuntimeScriptedEntityPersist { void LoadFromGameStateScript(const CCharString& text); }; }
class CScriptBase { protected: void DoScriptDataPersist(CCharString& data, bool saving); public: void LoadGameState(const CCharString& state); };
static char g_log[512]; static int g_bad = 0;
static void Log(const char* tag, const void* s) { strcat(g_log, tag); strcat(g_log, "["); strcat(g_log, s ? (const char*)s : "(null)"); strcat(g_log, "]"); }
static unsigned char g_script[0x48];
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; if (length != -1) g_bad = 1; }
CCharString::~CCharString() { }
CBaseClass::~CBaseClass() { }
CStringParser::CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset)
{ Log("parser", text.m_pData); Log("file", filename.m_pData); if (errorCallback || baseLineOffset) g_bad = 1; }
bool CStringParser::SkipPastString(const CCharString& item) { Log("next", item.m_pData); return true; }
CCharString CStringParser::ReadAsStringUntilString(const CCharString& end) { Log("until", end.m_pData); return CCharString("BODY", -1); }
void NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript(const CCharString& text) { if ((unsigned char*)this != g_script + 0x30) g_bad = 1; Log("entities", text.m_pData); }
void CScriptBase::DoScriptDataPersist(CCharString& data, bool saving) { if (saving) g_bad = 1; Log("data", data.m_pData); }
extern "C" void __fastcall NF420_ListBaseDtor(void*, int) { }
#pragma comment(linker, "/alternatename:??1?$_List_base@VCStringParserCommentBoundaries@@V?$allocator@VCStringParserCommentBoundaries@@@std@@@std@@QAE@XZ=@NF420_ListBaseDtor@8")
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CCharString state("STATE", -1);
    ((CScriptBase*)g_script)->LoadGameState(state);
    const char* want = "parser[STATE]file[]next[START_SCRIPT_DATA]until[END_SCRIPT_DATA]data[BODY]next[END_SCRIPT_DATA]"
                       "next[START_ENTITY_SCRIPTS]until[END_ENTITY_SCRIPTS]entities[BODY]next[END_ENTITY_SCRIPTS]";
    bool ok = !g_bad && strcmp(g_log, want) == 0;
    if (ok) printf("00cb8690_TEST PASS\n");
    else printf("FAIL bad=%d log=%s\n", g_bad, g_log);
    return ok ? 0 : 1;
}
