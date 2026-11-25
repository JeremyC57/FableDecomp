// Behaviour test for 0x0054ED90 NUISystem::CText::GetDataFromDef (?GetDataFromDef@CText@NUISystem@@QAEXXZ, Ego_r.pdb / FableWin.pdb).
// Links against the row object (verify_and_land's second link): the function under test is the row's own. The row is still on its HEAD
// local models (CText, CUIDef, its CDefPointer and the NUISystem-scope GFGetSystemManager stand-in), so this test defines no class at
// all: CFontManager::GetFont and ~CCharString are defined on their shared headers (engine/CFontManager.h, fable_string.h); the symbols no
// shared header declares (the row's call into the function under test, CDefStringTable::GetString, GDefStringTable and the stand-in
// GFGetSystemManager) are bound to plain __fastcall / C stubs with `/alternatename` (the linker maps an unresolved symbol to its stand-in;
// thiscall with ecx = this and callee-popped stack arguments is __fastcall with edx unused). The text component and its definition are
// byte buffers with hand-filled vtables (slots 108 LoadAndReturnDef, 144 SetValue; the definition's slot 1 Destroy).
// Cases: alignment LEFT / CENTRE / RIGHT set flag 0x08 / 0x10 / 0x20 of +0x12E (other bits kept), any other value sets none; the font
// comes from GetFont(GetString(def->Font)) into PFont +0x160 (the old one released); the window +0x178 and ScaleText +0x188 are copied;
// SetValue gets &def->TextValue, true and def->TextLineBreak; the returned definition's reference is dropped (Destroy at zero).
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "engine/CFontManager.h"

extern "C" void __fastcall CallGetDataFromDef(void* self);
extern "C" void* __fastcall StubGetString(void* self, int unused, void* result, long id);
extern "C" void* __fastcall StubGFGetSystemManager();
extern "C" unsigned char StubStringTable[0x40];
#pragma comment(linker, "/alternatename:@CallGetDataFromDef@4=?GetDataFromDef@CText@NUISystem@@QAEXXZ")
#pragma comment(linker, "/alternatename:?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z=@StubGetString@16")
// Naming (NF442, ruling q2): the row now calls the callee by its PDB spelling, so this double is respelt with it (callee only).
#pragma comment(linker, "/alternatename:?GFGetSystemManager@@YIPAVCSystemManager@@XZ=@StubGFGetSystemManager@0")
#pragma comment(linker, "/alternatename:?GDefStringTable@@3VCDefStringTable@@A=_StubStringTable")

unsigned char StubStringTable[0x40];
static unsigned char g_system[0x100], g_fontManager[0x10];
static unsigned char g_text[0x200] , g_def[0x200];
static void* g_textVtable[151]; static void* g_defVtable[4];
static long g_stringId = -1; static void* g_stringThis = 0; static int g_strDtors = 0;
static void* g_valueArg = 0; static int g_valueA = -1, g_valueB = -1, g_setValues = 0, g_defDestroyed = 0, g_fontDestroyed = 0;
static int g_fontNew, g_fontOld;
static CCPPointerInfo* g_fontInfo = 0;

static const char* const g_fontName = "FONT_NAME";
void* __fastcall StubGetString(void* self, int, void* result, long id)
{
    g_stringThis = self; g_stringId = id;
    *reinterpret_cast<const char**>(result) = g_fontName;      // the CCharString's string pointer (its only use here: GetFont's name)
    return result;
}
void* __fastcall StubGFGetSystemManager() { return g_system; }
CCharString::~CCharString() { ++g_strDtors; }
static const char* g_fontAsked = 0;
CCountedPointer<CFontBank> CFontManager::GetFont(const CCharString& name)
{
    g_fontAsked = (reinterpret_cast<unsigned char*>(this) == g_fontManager) ? *reinterpret_cast<const char* const*>(&name) : "wrong manager";
    CCountedPointer<CFontBank> result;
    result.Data = reinterpret_cast<CFontBank*>(&g_fontNew); result.Info = g_fontInfo; ++g_fontInfo->RefCount;
    return result;
}
static void* __fastcall FakeLoadAndReturnDef(void* self, int, void** result)
{
    *result = g_def; ++*reinterpret_cast<long*>(g_def + 4);
    return result;
}
static void __fastcall FakeSetValue(void* self, int, void* value, bool a, bool b)
{ ++g_setValues; g_valueArg = value; g_valueA = a; g_valueB = b; }
static void __fastcall FakeDestroy(void*) { ++g_defDestroyed; }
static void __fastcall DestroyFont(void*) { ++g_fontDestroyed; }

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("0054ed90 FAIL %s\n", what); g_fail = 1; } }
static void RunCase(long alignment, unsigned char expectFlag, bool lineBreak, bool scale)
{
    memset(g_text + 4, 0, sizeof(g_text) - 4);
    g_text[0x12e] = 0x41;                                         // other flag bits stay
    CCPPointerInfo* old = new CCPPointerInfo(1, DestroyFont, &g_fontOld);
    *reinterpret_cast<void**>(g_text + 0x160) = &g_fontOld; *reinterpret_cast<CCPPointerInfo**>(g_text + 0x164) = old;
    g_fontInfo = new CCPPointerInfo(0, DestroyFont, &g_fontNew);
    memset(g_def + 4, 0, sizeof(g_def) - 4);
    *reinterpret_cast<long*>(g_def + 4) = 1;                      // the definition's own reference
    *reinterpret_cast<long*>(g_def + 0x50) = 77;                  // Font (CDefString index)
    const float box[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; memcpy(g_def + 0xa4, box, sizeof(box));
    g_def[0xbd] = lineBreak; g_def[0xbe] = scale;
    *reinterpret_cast<long*>(g_def + 0x1fc) = alignment;
    g_fontDestroyed = 0; g_defDestroyed = 0; g_setValues = 0; g_strDtors = 0; g_stringId = -1; g_fontAsked = 0;
    CallGetDataFromDef(g_text);
    Check(g_text[0x12e] == (0x41 | expectFlag), "alignment flag");
    Check(g_stringThis == StubStringTable && g_stringId == 77 && g_fontAsked == g_fontName && g_strDtors == 1, "font name from the string table");
    Check(*reinterpret_cast<void**>(g_text + 0x160) == &g_fontNew && *reinterpret_cast<CCPPointerInfo**>(g_text + 0x164) == g_fontInfo &&
          g_fontInfo->RefCount == 1 && g_fontDestroyed == 1, "PFont replaced, the old font released");
    Check(memcmp(g_text + 0x178, box, sizeof(box)) == 0 && g_text[0x188] == (unsigned char)scale, "window and ScaleText copied");
    Check(g_setValues == 1 && g_valueArg == g_def + 0x54 && g_valueA == 1 && g_valueB == (int)lineBreak, "SetValue(TextValue, true, TextLineBreak)");
    Check(*reinterpret_cast<long*>(g_def + 4) == 1 && g_defDestroyed == 0, "the returned definition's reference dropped");
}
int main()
{
    *reinterpret_cast<void**>(g_system + 0x84) = g_fontManager;
    g_textVtable[108] = (void*)FakeLoadAndReturnDef; g_textVtable[144] = (void*)FakeSetValue;
    *reinterpret_cast<void***>(g_text) = g_textVtable;
    g_defVtable[1] = (void*)FakeDestroy; *reinterpret_cast<void***>(g_def) = g_defVtable;
    RunCase(0, 0x08, true, false);
    RunCase(1, 0x10, false, true);
    RunCase(2, 0x20, true, true);
    RunCase(3, 0x00, false, false);
    // the definition's last reference: Destroy runs once
    *reinterpret_cast<long*>(g_def + 4) = 0;                      // FakeLoadAndReturnDef adds the temporary's, which then drops to 0
    g_fontInfo = new CCPPointerInfo(0, DestroyFont, &g_fontNew);
    g_defDestroyed = 0;
    CallGetDataFromDef(g_text);
    Check(g_defDestroyed == 1, "definition destroyed at its last reference");
    if (g_fail) return 1;
    printf("0054ed90_TEST PASS\n");
    return 0;
}
