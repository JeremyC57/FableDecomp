#include <cstdio>
#include <cstdarg>
// Behaviour test for 0x00406610 CUserProfileManager::GetManualSaveFileName(long) const.
// NF143 (2026-11-20, q4285): rewritten on the shared headers the row now uses (fable_string.h with FABLE_RETAIL_CWIDESTRING,
//   engine/GFIntToWideString.h, engine/CUserProfileManager.h SAVENAMES + GETMANUALSAVEBASEFILENAME_INLINE). The test defines the
//   out-of-line CWideString members the row calls (default / copy / literal constructors, destructor, Sprintf, operator+) as recording
//   fakes and checks retail's order: number string, Sprintf(L"%d", slot), copy into the right operand, its destructor, L"Save" for the
//   left operand, operator+(left, right), then both operands destroyed.
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_GETMANUALSAVEBASEFILENAME_INLINE
#include "engine/CUserProfileManager.h"

static int g_seq = 0, g_defaultCtor = 0, g_sprintf = 0, g_copy = 0, g_lit = 0, g_add = 0, g_dtor = 0, g_dtorAtAdd = -1;
static long g_slot = -1;
static bool g_fmtOk = false, g_litOk = false, g_argsOk = false, g_sprintfThisOk = false, g_copyFromOk = false;
static const CWideString* g_number = 0;
static const CWideString* g_right = 0;
static const CWideString* g_left = 0;

CWideString::CWideString() { if (!g_number) { g_number = this; g_defaultCtor = ++g_seq; } }
void __cdecl CWideString::Sprintf(const wchar_t* format, ...)
{
    va_list args; va_start(args, format); g_slot = va_arg(args, long); va_end(args);
    g_fmtOk = format[0] == '%' && format[1] == 'd' && format[2] == 0;
    g_sprintfThisOk = this == g_number; g_sprintf = ++g_seq;
}
CWideString::CWideString(const CWideString& other) { g_copyFromOk = &other == g_number; g_right = this; g_copy = ++g_seq; }
CWideString::CWideString(const wchar_t* text)
{
    g_litOk = text[0] == 'S' && text[1] == 'a' && text[2] == 'v' && text[3] == 'e' && text[4] == 0;
    g_left = this; g_lit = ++g_seq;
}
CWideString::~CWideString() { ++g_dtor; }
CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right)
{
    g_argsOk = &left == g_left && &right == g_right; g_dtorAtAdd = g_dtor; g_add = ++g_seq;
    return CWideString();
}

int main()
{
    static double storage[64];                    // the manager's bytes are never read by the method
    const CUserProfileManager* mgr = (const CUserProfileManager*)storage;
    {
        CWideString out = mgr->GetManualSaveFileName(7);
    }
    bool ok = g_slot == 7 && g_fmtOk && g_litOk && g_argsOk && g_sprintfThisOk && g_copyFromOk
           && g_defaultCtor == 1 && g_sprintf == 2 && g_copy == 3 && g_lit == 4 && g_add == 5
           && g_dtorAtAdd == 1            // the number string is destroyed before L"Save" is built
           && g_dtor == 4;                // number, both operands, and the result
    if (ok) printf("00406610_TEST PASS\n");
    else printf("FAIL slot=%ld ctor=%d spr=%d copy=%d lit=%d add=%d dtorAtAdd=%d dtor=%d fmt=%d lit=%d args=%d\n",
                g_slot, g_defaultCtor, g_sprintf, g_copy, g_lit, g_add, g_dtorAtAdd, g_dtor, g_fmtOk, g_litOk, g_argsOk);
    return 0;
}
