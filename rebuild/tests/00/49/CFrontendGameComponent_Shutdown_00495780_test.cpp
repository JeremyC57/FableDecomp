// Test for 0x00495780 CFrontendGameComponent::Shutdown on the shared headers (NF234): the row's defines and includes, its body, and stubs
// for what retail keeps out of line (DeleteData<CGraphicDataBank> / <const CFontBank>, vector<CWideString>::erase 0x00412450).
// Case 1: MainFont's last reference is dropped (deleter + count block freed, Data / Info zeroed). Case 2: PTempGraphics still shared
// (count 2 -> 1, no delete, Data / Info zeroed). Case 3: SaveNames.clear() then WorldNames.clear(), each erase(begin, end) once, in order.
// Case 4: a second Shutdown with both pointers null deletes nothing more (the null-Info arm) and erases twice more.
// Test-only: private opened for Shutdown and the members.
#include <cstdio>
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RELEASEDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CGRAPHICDATABANK_RESET_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RESET_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SHUTDOWN
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CGameComponent.h"
#define private public
#include "engine/CFrontendGameComponent.h"
#undef private
static int g_deleted = 0, g_erase = 0; static void* g_eraseThis[8]; static CWideString* g_eraseArgs[16];
template<> void FABLE_FASTCALL CCountedPointer<CGraphicDataBank>::DeleteData(void*) { }
template<> void FABLE_FASTCALL CCountedPointer<const CFontBank>::DeleteData(void*) { }
static void __fastcall CountDelete(void*) { ++g_deleted; }
template<> std::vector<CWideString>::iterator std::vector<CWideString>::erase(iterator first, iterator last)
{
    g_eraseThis[g_erase] = this; g_eraseArgs[2 * g_erase] = first; g_eraseArgs[2 * g_erase + 1] = last; ++g_erase; return first;
}
void CFrontendGameComponent::Shutdown()
{
    MainFont.Reset(0);
    PTempGraphics.Reset(0);
    SaveNames.clear();
    WorldNames.clear();
}
int main()
{
    static unsigned char raw[sizeof(CFrontendGameComponent)];
    CFrontendGameComponent& c = *(CFrontendGameComponent*)raw;
    CCPPointerInfo* a = new CCPPointerInfo(1, CountDelete, (void*)8);
    CCPPointerInfo b(2, CountDelete, (void*)16);
    c.MainFont.Data = (const CFontBank*)8; c.MainFont.Info = a;
    c.PTempGraphics.Data = (CGraphicDataBank*)16; c.PTempGraphics.Info = &b;
    CWideString* s = (CWideString*)0x100; CWideString* w = (CWideString*)0x200;
    *(CWideString**)&c.SaveNames = s; ((CWideString**)&c.SaveNames)[1] = s + 2;
    *(CWideString**)&c.WorldNames = w; ((CWideString**)&c.WorldNames)[1] = w + 3;
    c.Shutdown();
    int ok = 1;
    if (g_deleted != 1 || c.MainFont.Data != 0 || c.MainFont.Info != 0) ok = 0;                       // case 1
    if (b.RefCount != 1 || c.PTempGraphics.Data != 0 || c.PTempGraphics.Info != 0) ok = 0;            // case 2
    if (g_erase != 2 || g_eraseThis[0] != (void*)&c.SaveNames || g_eraseThis[1] != (void*)&c.WorldNames
        || g_eraseArgs[0] != s || g_eraseArgs[1] != s + 2 || g_eraseArgs[2] != w || g_eraseArgs[3] != w + 3) ok = 0;   // case 3
    c.Shutdown();                                                                                       // case 4: both pointers already null
    if (g_deleted != 1 || b.RefCount != 1 || c.MainFont.Info != 0 || c.PTempGraphics.Info != 0 || g_erase != 4) ok = 0;
    std::printf(ok ? "00495780_TEST PASS\n" : "00495780_TEST FAIL\n");
    return ok ? 0 : 1;
}
