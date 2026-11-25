// Behaviour test for 0x00CB8440 NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript: clears the map, skips to
// "START_NEW_ENTITY_SCRIPT" (retail 0x012C1674), and for each entry reads the UID and the script up to "END_NEW_ENTITY_SCRIPT"
// (0x012C165C) into the map, then sorts it if anything was added.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings and log the text they receive, so the test asserts both literals and the stored entry; a mutated
// literal fails. The row's inline STLport vector / list code runs as compiled (CRT malloc / free).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CCharString { public: void* PStringData; CCharString(); CCharString(const char* text, long length); CCharString(const CCharString& other);
    ~CCharString(); const CCharString& operator=(const CCharString& other); };
class CBaseClass { public: virtual ~CBaseClass(); };
class CStringParser { public:
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset);
    bool SkipPastNextItemIfIs(const CCharString& item); bool SkipPastString(const CCharString& item);
    CCharString ReadAsStringUntilString(const CCharString& end);
    long SkipUntilWhitespace(); long SkipWhitespace(); unsigned __int64 ReadAsUHUGE();
};
namespace NScript { struct CRuntimeScriptedEntityPersist {
    class CPersistInfo { public: unsigned __int64 UID; CCharString PersistScript; };
    struct CMap { void Clear(); }; void LoadFromGameStateScript(const CCharString& text); }; }
template <class K, class V> class CKeyPairCompareLess { };
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class _RandomAccessIter, class _Compare> void __fastcall sort(_RandomAccessIter first, _RandomAccessIter last, _Compare comp);
}
typedef std::pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> CEntry;
static char g_log[512]; static int g_bad = 0;
static void Log(const char* tag, const void* s) { strcat(g_log, tag); strcat(g_log, "["); strcat(g_log, s ? (const char*)s : "(null)"); strcat(g_log, "]"); }
static int g_entries = 0, g_sorts = 0, g_clears = 0, g_erases = 0;
CCharString::CCharString() { PStringData = 0; }
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; if (length != -1) g_bad = 1; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; }
CCharString::~CCharString() { }
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
CBaseClass::~CBaseClass() { }
CStringParser::CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset)
{
    Log("parser", text.PStringData); Log("file", filename.PStringData); if (errorCallback || baseLineOffset) g_bad = 1;
    void** sentinel = (void**)malloc(16); sentinel[0] = sentinel; sentinel[1] = sentinel;     // the empty comment list (+0x14) the row's
    memset(this, 0, 0x28); *(void***)((unsigned char*)this + 0x14) = sentinel;                // inline destructor walks and frees
}
bool CStringParser::SkipPastNextItemIfIs(const CCharString& item) { Log("skip", item.PStringData); return g_entries++ < 1; }
bool CStringParser::SkipPastString(const CCharString& item) { Log("past", item.PStringData); return true; }
CCharString CStringParser::ReadAsStringUntilString(const CCharString& end) { Log("until", end.PStringData); return CCharString("SCRIPT_BODY", -1); }
long CStringParser::SkipUntilWhitespace() { strcat(g_log, "nonws;"); return 0; }
long CStringParser::SkipWhitespace() { strcat(g_log, "ws;"); return 0; }
unsigned __int64 CStringParser::ReadAsUHUGE() { return 0x123456789ull; }
void NScript::CRuntimeScriptedEntityPersist::CMap::Clear() { ++g_clears; }
// the row model's __fastcall std::sort (declared there, defined here) and its out-of-line vector erase (a thiscall-compatible stand-in)
template <> void __fastcall std::sort(CEntry* first, CEntry* last, CKeyPairCompareLess<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>)
{ if (last - first != 1) g_bad = 1; ++g_sorts; }
extern "C" void* __fastcall NF420_Erase(void*, int, void* first, void* last) { if (first != last) g_bad = 1; ++g_erases; return first; }
#pragma comment(linker, "/alternatename:?erase@?$vector@U?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@V?$allocator@U?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@@2@@std@@QAEPAU?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@2@PAU32@0@Z=@NF420_Erase@16")
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static unsigned char obj[0x10];
    CCharString text("TEXT", -1);
    ((NScript::CRuntimeScriptedEntityPersist*)obj)->LoadFromGameStateScript(text);
    const char* want = "parser[TEXT]file[]until[START_NEW_ENTITY_SCRIPT]skip[START_NEW_ENTITY_SCRIPT]nonws;ws;until[END_NEW_ENTITY_SCRIPT]"
                       "past[END_NEW_ENTITY_SCRIPT]skip[START_NEW_ENTITY_SCRIPT]";
    unsigned char* first = *(unsigned char**)obj; unsigned char* last = *(unsigned char**)(obj + 4);
    bool ok = !g_bad && strcmp(g_log, want) == 0 && g_clears == 1 && g_erases == 1 && g_sorts == 1 && obj[0xd] == 0
        && last - first == 0x18 && *(unsigned __int64*)first == 0x123456789ull && *(unsigned __int64*)(first + 8) == 0x123456789ull
        && strcmp(*(const char**)(first + 0x10), "SCRIPT_BODY") == 0;
    if (ok) printf("00cb8440_TEST PASS\n");
    else printf("FAIL bad=%d log=%s n=%d\n", g_bad, g_log, (int)(last - first));
    return ok ? 0 : 1;
}
