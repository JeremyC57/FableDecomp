// Behaviour test for 0x00A9AE60 C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>&) (links the REAL row object, NF419).
// Fixtures: raw retail layouts (CCountedPointer: Data +0 / Info +4; CCPPointerInfo: RefCount +0, __fastcall DeleteFunc +4, Data +8;
// CChunkedFile 0x14 bytes, vptr +0). The base constructor and destructor are recording stubs bound by /alternatename; operator delete is
// replaced to record its argument. Each run checks the base constructor's argument (the converted temporary: same Data / Info, one more
// reference), that the temporary is released afterwards (DeleteFunc + delete of the block only when the count reaches zero), the
// vtable store (C3DMeshFile's vftable, after the base constructor ran) and the returned this.
#include <stdio.h>
#include <string.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("??0CChunkedFile@@QAE@ABV?$CCountedPointer@VCChunkedFileSuperChunk@@@@@Z", "@BaseCtor@12")
#pragma ALT("??1CChunkedFile@@UAE@XZ", "@BaseDtor@8")
#pragma ALT("@CallRow@12", "??0C3DMeshFile@@AAE@ABV?$CCountedPointer@VC3DMeshFileRootChunk@@@@@Z")
#pragma ALT("_g_vtable", "??_7C3DMeshFile@@6B@")

struct Info { unsigned long refs; void (__fastcall* del)(void*); void* data; };
struct CP { void* data; Info* info; };
extern "C" {
void* __fastcall CallRow(void* self, int, const CP* root);
extern void* g_vtable[];
}
static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

static void* g_deleted[8]; static int g_ndel = 0;
void operator delete(void* p) { if (g_ndel < 8) g_deleted[g_ndel] = p; ++g_ndel; }
static void* g_delData = 0; static int g_ndelData = 0;
static void __fastcall DelFunc(void* d) { g_delData = d; ++g_ndelData; }
static void* g_baseThis = 0; static const CP* g_baseArg = 0; static CP g_seen; static unsigned long g_seenRefs = 0; static int g_drop = 0;
static int g_baseDtor = 0;
extern "C" void* __fastcall BaseCtor(void* self, int, const CP* root)
{
    g_baseThis = self; g_baseArg = root; g_seen = *root; g_seenRefs = root->info ? root->info->refs : 0xFFFFFFFF;
    ((void**)self)[0] = (void*)0x11111111;            // the base's own vtable store, overwritten by the derived one
    ((unsigned long*)self)[3] = 0x2222;               // Version stand-in, must survive
    if (g_drop && root->info) root->info->refs -= g_drop;  // simulate other owners letting go while the base runs
    return self;
}
extern "C" void __fastcall BaseDtor(void*, int) { ++g_baseDtor; }

static void Run(void* data, Info* info, unsigned long refs, int drop, bool expectFree)
{
    ++g_run; g_ndel = 0; g_ndelData = 0; g_delData = 0; g_baseThis = 0; g_drop = drop; g_seenRefs = 0; g_baseDtor = 0;
    if (info) { info->refs = refs; info->del = DelFunc; info->data = data; }
    CP root = { data, info };
    unsigned char obj[0x14]; memset(obj, 0xCD, sizeof obj);
    void* r = CallRow(obj, 0, &root);
    CHECK(r == obj); CHECK(g_baseThis == obj);
    CHECK(g_baseArg != &root);                         // a converted temporary, not the caller's pointer
    CHECK(g_seen.data == data && g_seen.info == info);
    if (info) CHECK(g_seenRefs == refs + 1);
    CHECK(((void**)obj)[0] == (void*)g_vtable);
    CHECK(((unsigned long*)obj)[3] == 0x2222);
    CHECK(root.data == data && root.info == info);     // the caller's pointer is untouched
    if (info && !expectFree) { CHECK(info->refs == refs - drop); CHECK(g_ndel == 0); CHECK(g_ndelData == 0); }
    if (expectFree) { CHECK(g_ndelData == 1 && g_delData == data); CHECK(g_ndel == 1 && g_deleted[0] == info); }
    if (!info) { CHECK(g_ndel == 0); CHECK(g_ndelData == 0); }
    CHECK(g_baseDtor == 0);
}
int main()
{
    static Info a, b, c; static int chunkA, chunkB, chunkC;
    Run(&chunkA, &a, 1, 0, false);                     // caller keeps its reference
    Run(&chunkB, &b, 5, 0, false);
    Run(&chunkC, &c, 1, 1, true);                      // the temporary held the last reference: DeleteFunc(Data), then delete Info
    Run(0, 0, 0, 0, false);                            // null pointer: no count to touch
    if (g_fail) { printf("00a9ae60_TEST FAIL\n"); return 1; }
    printf("00a9ae60_TEST PASS\n");
    return 0;
}
