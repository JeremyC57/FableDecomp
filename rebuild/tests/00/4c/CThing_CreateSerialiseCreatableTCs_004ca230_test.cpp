// Behaviour test for 0x004CA230 CThing::CreateSerialiseCreatableTCs: parses `CreateTC "<name>";` entries and adds each TC by name.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined; it is called through a
// __fastcall shim aliased to the row symbol). The callees are defined under the row's own spellings and log the text they receive,
// so the test asserts the literals the row passes ("CreateTC", "\"", ";", "" for the filename); a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
// ---- row declarations (copied from the row source; its function definition left out) ----
// CThing::CreateSerialiseCreatableTCs(const CCharString&) (protected) -- parse `CreateTC "<name>";` statements from a text block and add each named thing component (activated)
// Retail 0x004CA230, 349 bytes, ret 4. Label correction: the catalog's "CScriptBase::LoadGameState" is propagated; the
// only caller (0x004CAE62) is CThing code that passes the thing in ecx right after a CThing::AddTC call, and the body
// calls CThing::AddTC 0x004C9D60 on `this`; the method name is descriptive.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CStringParser ctor 0x00404720 and its
// implicit inline dtor (~_List_base 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0), CStringParser::
// NextItemIs 0x009B9BA0 (catalog "SerialiseGameState" propagated), SkipPastNextItemIfIs 0x009B9E00, ReadAsString
// 0x009BA6F0 (by value). Literals: "" 0x0122D70E, "CreateTC" 0x01239BFC, "\"" 0x0122DF24, ";" 0x0123542C.
// Shape: the parser model of the landed CScriptBase::DoScriptDataPersist 0x00CB7CB0 (filename an unnamed "" temporary,
// `push eax`); every keyword is a NAMED CCharString inside small inline helpers (NextItemIsText / SkipPastText) --
// retail passes them with `lea ; push` and destroys each right after its call; the while condition is duplicated at
// the loop bottom (rotated loop).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CStringParserCommentBoundaries;
// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (declaration-only _List_base ctor / dtor): the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"
class CStringParser : public CBaseClass { public:                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    std::list<CStringParserCommentBoundaries> Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
    bool SkipPastString(const CCharString& item);              // 0x009B9BA0 NF35: Ego_r ?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z (catalog CStringParser::SkipPastString)
    bool SkipPastNextItemIfIs(const CCharString& item);    // 0x009B9E00
    CCharString ReadAsString();                            // 0x009BA6F0
    bool NextItemIsText(const char* text)
    {
        CCharString item(text, -1);
        return SkipPastString(item);
    }
    void SkipPastText(const char* text)
    {
        CCharString item(text, -1);
        SkipPastNextItemIfIs(item);
    }
};
class CTCBase;
class CTCParamsBase;
class CThing { protected:
    void CreateSerialiseCreatableTCs(const CCharString& text);
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};

// ---- end of row declarations ----
static char g_log[512]; static int g_entries = 0; static int g_addCalls = 0; static bool g_addFlag = false; static const void* g_addParams = (void*)1;
static void Log(const char* tag, const char* s) { strcat(g_log, tag); strcat(g_log, "["); strcat(g_log, s ? s : "(null)"); strcat(g_log, "]"); }
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; if (length != -1) strcat(g_log, "LEN!"); }
CCharString::~CCharString() { }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CStringParser::CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename, long baseLineOffset)
    : Filename(filename), TempCharString(0, -1)
{ Log("parser", (const char*)text.m_pData); Log("file", (const char*)filename.m_pData); if (errorCallback || baseLineOffset) strcat(g_log, "ARGS!"); }
bool CStringParser::SkipPastString(const CCharString& item) { Log("next", (const char*)item.m_pData); return g_entries++ < 2; }
bool CStringParser::SkipPastNextItemIfIs(const CCharString& item) { Log("skip", (const char*)item.m_pData); return true; }
CCharString CStringParser::ReadAsString() { return CCharString(g_entries == 1 ? "CTCA" : "CTCB", -1); }
CTCBase* CThing::AddTC(const CCharString& name, bool flag, const CTCParamsBase* params) { Log("add", (const char*)name.m_pData); ++g_addCalls; g_addFlag = flag; g_addParams = params; return 0; }
// The parser's comment list (STLport _List_base, out of line in the row's model): thiscall-compatible __fastcall stand-ins.
extern "C" void* __fastcall NF420_ListBaseCtor(void* self, int, const void*) { *(void**)self = 0; return self; }
extern "C" void __fastcall NF420_ListBaseDtor(void*, int) { }
#pragma comment(linker, "/alternatename:??0?$_List_base@VCStringParserCommentBoundaries@@V?$allocator@VCStringParserCommentBoundaries@@@std@@@std@@QAE@ABV?$allocator@VCStringParserCommentBoundaries@@@1@@Z=@NF420_ListBaseCtor@12")
#pragma comment(linker, "/alternatename:??1?$_List_base@VCStringParserCommentBoundaries@@V?$allocator@VCStringParserCommentBoundaries@@@std@@@std@@QAE@XZ=@NF420_ListBaseDtor@8")
extern "C" void __fastcall NF420_Row(CThing* self, int edx, const CCharString& text);
#pragma comment(linker, "/alternatename:@NF420_Row@12=?CreateSerialiseCreatableTCs@CThing@@IAEXABVCCharString@@@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CThing thing; CCharString text("CreateTC \"CTCA\";", -1);
    NF420_Row(&thing, 0, text);
    const char* want = "parser[CreateTC \"CTCA\";]file[]next[CreateTC]skip[\"]skip[\"]add[CTCA]skip[;]next[CreateTC]skip[\"]skip[\"]add[CTCB]skip[;]next[CreateTC]";
    bool ok = strcmp(g_log, want) == 0 && g_addCalls == 2 && g_addFlag && g_addParams == 0;
    if (ok) printf("004ca230_TEST PASS\n");
    else printf("FAIL log=%s\n", g_log);
    return ok ? 0 : 1;
}
