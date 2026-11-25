// Behaviour test for 0x00A9A730 C3DAnimFile::`scalar deleting destructor' (links the REAL row object: the Option S owner's TU, NF419).
// ~CChunkedFile is a recording stub bound by /alternatename (as is the base constructor the owner's TU references); operator delete is
// replaced to record its argument. Each call checks the destructor ran once on this, operator delete ran on this exactly when bit 0 of
// the flags is set, and the returned pointer is this; the vtable slot 0 of C3DAnimFile's vftable is this function.
#include <stdio.h>
#include <string.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("??0CChunkedFile@@QAE@ABV?$CCountedPointer@VCChunkedFileSuperChunk@@@@@Z", "@BaseCtor@12")
#pragma ALT("??1CChunkedFile@@UAE@XZ", "@BaseDtor@8")
#pragma ALT("@CallRow@12", "??_GC3DAnimFile@@UAEPAXI@Z")
#pragma ALT("_g_vtable", "??_7C3DAnimFile@@6B@")
extern "C" {
void* __fastcall CallRow(void* self, int, unsigned int flags);
extern void* g_vtable[];
}
static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static void* g_deleted = 0; static int g_ndel = 0;
void operator delete(void* p) { g_deleted = p; ++g_ndel; }
static void* g_dtorThis = 0; static int g_ndtor = 0;
extern "C" void* __fastcall BaseCtor(void* self, int, const void*) { return self; }
extern "C" void __fastcall BaseDtor(void* self, int) { g_dtorThis = self; ++g_ndtor; }
static void Run(unsigned int flags, bool expectDelete)
{
    ++g_run; g_deleted = 0; g_ndel = 0; g_dtorThis = 0; g_ndtor = 0;
    unsigned char obj[0x14]; memset(obj, 0x5A, sizeof obj); ((void**)obj)[0] = (void*)g_vtable;
    void* r = CallRow(obj, 0, flags);
    CHECK(r == obj);
    CHECK(g_ndtor == 1 && g_dtorThis == obj);
    CHECK(g_ndel == (expectDelete ? 1 : 0));
    if (expectDelete) CHECK(g_deleted == obj);
    CHECK(((unsigned char*)obj)[4] == 0x5A);           // nothing else written
}
int main()
{
    Run(0, false); Run(1, true); Run(2, false); Run(3, true); Run(0x80000001u, true);
    // slot 0 holds ??_E, which VC7.1 defaults to this ??_G
    CHECK(g_vtable[0] == (void*)CallRow);
    if (g_fail) { printf("00a9a730_TEST FAIL\n"); return 1; }
    printf("PASS_00a9a730\n");
    return 0;
}
