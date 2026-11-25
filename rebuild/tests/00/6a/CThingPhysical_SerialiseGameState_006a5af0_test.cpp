// Behaviour test for 0x006A5AF0 CThingPhysical::SerialiseGameState: outside load modes it resets the physical state from the def; then
// it transfers "Health" (retail 0x0125D090, default MaxHealth, clamped unless the thing class is 2) and "Scale" (0x0125D088, default 1.0).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined; it is called through a
// __fastcall shim aliased to the row symbol). The callees are defined under the row's own spellings and log the names they receive,
// so the test asserts both literals; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class CBaseObject; class CBaseObjectPointer;
class CBaseIntelligentPointer { public:
    CBaseIntelligentPointer(); virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const; void SetPItem(CBaseObject* item);
    CBaseObjectPointer* PData;
};
class CPersistContext { public: unsigned char _pad00[0x18]; long Mode; template <class T> void Transfer(const char* name, T& value, const T& defaultValue); };
static char g_log[256]; static int g_live = 0, g_sets = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() { PData = 0; ++g_live; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_live; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = (CBaseObjectPointer*)item; ++g_sets; }
static unsigned char* g_obj = 0;
template <> void CPersistContext::Transfer<float>(const char* name, float& value, const float& defaultValue)
{
    char b[64]; sprintf(b, "%s@%x=%g;", name, (unsigned)((unsigned char*)&value - g_obj), defaultValue); strcat(g_log, b);
    if (strcmp(name, "Health") == 0) value = 500.0f;       // over MaxHealth: the row clamps it
}
extern "C" bool __fastcall NF420_Row(void* self, int edx, CPersistContext& context);
#pragma comment(linker, "/alternatename:@NF420_Row@12=?SerialiseGameState@CThingPhysical@@UAE_NAAVCPersistContext@@@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static double objRaw[0xc0 / 8]; g_obj = (unsigned char*)objRaw;
    static double defRaw[0x58 / 8]; unsigned char* def = (unsigned char*)defRaw;
    *(float*)(def + 0x50) = 100.0f; def[0x42] = 0x40;                  // def Health 100, Damageable
    *(unsigned char**)(g_obj + 0x70) = def; g_obj[0x10] = 1;              // PBaseDef, ThingClass 1
    *(float*)(g_obj + 0x9c) = 2.0f;
    CPersistContext ctx; memset(&ctx, 0, sizeof(ctx)); ctx.Mode = 0;
    bool r = NF420_Row(g_obj, 0, ctx);
    bool ok = r && strcmp(g_log, "Health@b4=100;Scale@9c=1;") == 0 && *(float*)(g_obj + 0xb0) == 100.0f && *(float*)(g_obj + 0xb4) == 100.0f
        && *(long*)(g_obj + 0xb8) == -1 && (g_obj[0x92] & 2) && (g_obj[0xbc] & 0x3f) == 0x27 && g_live == 0 && g_sets == 4;
    if (ok) printf("006a5af0_TEST PASS\n");
    else printf("FAIL r=%d log=%s max=%f health=%f bits=%02x live=%d sets=%d\n", r, g_log, *(float*)(g_obj + 0xb0), *(float*)(g_obj + 0xb4), g_obj[0xbc], g_live, g_sets);
    return ok ? 0 : 1;
}
