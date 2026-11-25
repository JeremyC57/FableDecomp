// Behaviour test for 0x00A99510 C3DAnimFileLoader::LoadAnimFileCompressed(const void*, unsigned long, CHandleBasedMemoryPool&) (links the REAL row object: the retail lib_3d_anim_file.obj unit's TU, NF422).
// Every external of the unit object is a recording stub bound by /alternatename (thiscall stubs are __fastcall with a dummy edx); operator
// new / delete are replaced and recorded; the row symbol and the compiler-generated helpers are reached through /alternatename aliases.
// Raw retail layouts: CCountedPointer Data +0 / Info +4; CCPPointerInfo RefCount +0, __fastcall DeleteFunc +4, Data +8;
// C3DAnimFile 0x14 bytes (vptr, RootChunk +4, Version +0xC, FileComment +0x10); loader RootChunkType at +0x10.
#include <stdio.h>
#include <string.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("??0CCharString@@QAE@PBDJ@Z", "@CCharStringCtor@16")
#pragma ALT("??1CCharString@@QAE@XZ", "@CCharStringDtor@8")
#pragma ALT("??4CCharString@@QAEABV0@ABV0@@Z", "@CCharStringAssign@12")
#pragma ALT("??0CChunkedFile@@QAE@ABV?$CCountedPointer@VCChunkedFileSuperChunk@@@@@Z", "@ChunkedFileCtor@12")
#pragma ALT("??1CChunkedFile@@UAE@XZ", "@ChunkedFileDtor@8")
#pragma ALT("??0CChunkedFileSuperChunk@@IAE@PBD@Z", "@SuperChunkCtor@12")
#pragma ALT("??1CChunkedFileSuperChunk@@UAE@XZ", "@SuperChunkDtor@8")
#pragma ALT("??0CMemoryDataInputStream@@QAE@PBXK@Z", "@MemStreamCtor@16")
#pragma ALT("??1CDataStream@@UAE@XZ", "@DataStreamDtor@8")
#pragma ALT("??0CTemporaryBuffer@@QAE@KPAVCHandleBasedMemoryPool@@PAVCScratchBuffer@@@Z", "@TempBufCtor@20")
#pragma ALT("??1CTemporaryBuffer@@UAE@XZ", "@TempBufDtor@8")
#pragma ALT("?LoadChunkedFile@CChunkedFileLoader@@IAEXAAVCChunkedFile@@AAVCDataInputStream@@@Z", "@LoadChunked@16")
#pragma ALT("?g_ScratchBuffer@@3VCScratchBuffer@@A", "_g_scratch")
#pragma ALT("?CreateNewInstance@C3DAnimFileRootChunk@@UBEPAVCChunkedFileChunk@@XZ", "_UnusedSlot")
#pragma ALT("?GetChunkDescription@C3DAnimFileRootChunk@@UBE?BVCCharString@@XZ", "_UnusedSlot")
#pragma ALT("?ReadFromFile@C3DAnimFileRootChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z", "_UnusedSlot")
#pragma ALT("?WriteToFile@C3DAnimFileRootChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z", "_UnusedSlot")
#pragma ALT("?GetChunkInfo@CChunkedFileChunk@@UBE?BVCCharString@@XZ", "_UnusedSlot")
#pragma ALT("?IsSuperChunk@CChunkedFileSuperChunk@@UBEHXZ", "_UnusedSlot")
#pragma ALT("?IsUnknownType@CChunkedFileChunk@@UBEHXZ", "_UnusedSlot")
#pragma ALT("?GetAsSuperChunk@CChunkedFileSuperChunk@@UBEPBV1@XZ", "_UnusedSlot")
#pragma ALT("?GetAsSuperChunk@CChunkedFileSuperChunk@@UAEPAV1@XZ", "_UnusedSlot")
#pragma ALT("?InitNewInstance@CChunkedFileSuperChunk@@MBEPAVCChunkedFileChunk@@PAV2@@Z", "_UnusedSlot")
#pragma ALT("_g_vtable", "??_7C3DAnimFile@@6B@")
#pragma ALT("_g_rootVtable", "??_7C3DAnimFileRootChunk@@6B@")
#pragma ALT("@DelSuper@4", "?DeleteData@?$CCountedPointer@VCChunkedFileSuperChunk@@@@SIXPAX@Z")
#pragma ALT("@DelRoot@4", "?DeleteData@?$CCountedPointer@VC3DAnimFileRootChunk@@@@SIXPAX@Z")
#pragma ALT("@DelFile@4", "?DeleteData@?$CCountedPointer@VC3DAnimFile@@@@SIXPAX@Z")
#pragma ALT("@RootDeleting@12", "??_GC3DAnimFileRootChunk@@UAEPAXI@Z")
#pragma ALT("@CallRow@24", "?LoadAnimFileCompressed@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VC3DAnimFile@@@@PBXKAAVCHandleBasedMemoryPool@@@Z")

struct Info { unsigned long refs; void (__fastcall* del)(void*); void* data; };
struct CP { void* data; Info* info; };
extern "C" {
extern void* g_vtable[]; extern void* g_rootVtable[]; char g_scratch[16];
void __fastcall DelSuper(void*); void __fastcall DelRoot(void*); void __fastcall DelFile(void*);
void* __fastcall RootDeleting(void*, int, unsigned int);
}
static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
// event log
static char g_log[64][24]; static void* g_logp[64]; static int g_nlog = 0;
static void Log(const char* e, void* p) { if (g_nlog < 64) { strncpy(g_log[g_nlog], e, 23); g_log[g_nlog][23] = 0; g_logp[g_nlog] = p; } ++g_nlog; }
static int Find(const char* e, void* p) { for (int i = 0; i < g_nlog && i < 64; ++i) if (!strcmp(g_log[i], e) && (p == (void*)-1 || g_logp[i] == p)) return i; return -1; }
static int Count(const char* e) { int n = 0; for (int i = 0; i < g_nlog && i < 64; ++i) if (!strcmp(g_log[i], e)) ++n; return n; }
static unsigned char g_heap[16][0x60]; static unsigned int g_newSize[16]; static int g_nnew = 0;
void* operator new(unsigned int n) { int k = g_nnew < 16 ? g_nnew : 15; g_newSize[k] = n; ++g_nnew; memset(g_heap[k], 0xEE, sizeof g_heap[k]); Log("new", g_heap[k]); return g_heap[k]; }
void operator delete(void* p) { Log("delete", p); }
static void Reset() { ++g_run; g_nlog = 0; g_nnew = 0; }
extern "C" {
// CCharString(const char*, long) / ~CCharString / operator=
static const char* g_strText = 0; static long g_strLen = 0; static void* g_assignSrc = 0;
void* __fastcall CCharStringCtor(void* self, int, const char* t, long n) { g_strText = t; g_strLen = n; *(const char**)self = t; Log("strctor", self); return self; }
void __fastcall CCharStringDtor(void* self, int) { Log("strdtor", self); }
void* __fastcall CCharStringAssign(void* self, int, void* other) { g_assignSrc = other; *(const char**)self = *(const char**)other; Log("strassign", self); return self; }
// CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&) / ~CChunkedFile: keeps a reference when g_keep
static int g_keep = 1; static CP g_baseSeen; static unsigned long g_baseSeenRefs = 0; static const CP* g_baseArg = 0;
void* __fastcall ChunkedFileCtor(void* self, int, const CP* root)
{
    g_baseArg = root; g_baseSeen = *root; g_baseSeenRefs = root->info ? root->info->refs : 0xFFFFFFFF;
    ((void**)self)[0] = (void*)0x11111111; ((CP*)((char*)self + 4))->data = 0; ((CP*)((char*)self + 4))->info = 0; ((unsigned long*)self)[3] = 7;
    if (g_keep) { *(CP*)((char*)self + 4) = *root; if (root->info) ++root->info->refs; }
    Log("filector", self); return self;
}
void __fastcall ChunkedFileDtor(void* self, int) { Log("filedtor", self); }
static const char* g_chunkType = 0;
void* __fastcall SuperChunkCtor(void* self, int, const char* type) { g_chunkType = type; ((void**)self)[0] = (void*)0x22222222; Log("superctor", self); return self; }
void __fastcall SuperChunkDtor(void* self, int) { Log("superdtor", self); }
// CMemoryDataInputStream(const void*, unsigned long) / ~CDataStream; the stream's vtable is the test's (GetPosition slot 2, SetPosition slot 1)
static unsigned long g_pos = 0x1234; static unsigned long g_setPos = 0xFFFFFFFF;
unsigned long __fastcall StreamGetPosition(void* self, int) { Log("getpos", self); return g_pos; }
void __fastcall StreamSetPosition(void* self, int, unsigned long p) { g_setPos = p; Log("setpos", self); }
void __cdecl UnusedSlot() { printf("unexpected call\n"); g_fail = 1; }
void* g_streamVtbl[4] = { (void*)UnusedSlot, (void*)StreamSetPosition, (void*)StreamGetPosition, (void*)UnusedSlot };
static const void* g_msData = 0; static unsigned long g_msSize = 0;
void* __fastcall MemStreamCtor(void* self, int, const void* data, unsigned long size) { g_msData = data; g_msSize = size; ((void**)self)[0] = g_streamVtbl; Log("msctor", self); return self; }
void __fastcall DataStreamDtor(void* self, int) { Log("msdtor", self); }
// CTemporaryBuffer(size, pool, scratch) / ~CTemporaryBuffer
static unsigned char g_buf[64]; static unsigned long g_tbSize = 0; static void* g_tbPool = 0; static void* g_tbScratch = 0;
void* __fastcall TempBufCtor(void* self, int, unsigned long size, void* pool, void* scratch) { g_tbSize = size; g_tbPool = pool; g_tbScratch = scratch; ((void**)self)[1] = g_buf; Log("tbctor", self); return self; }
void __fastcall TempBufDtor(void* self, int) { Log("tbdtor", self); }
static const unsigned char* g_lzSrc = 0; static unsigned long g_lzLen = 0, g_lzOut = 0; static unsigned char* g_lzDst = 0; static void* g_lzWrk = (void*)1;
int lzo1x_decompress_asm_fast(const unsigned char* src, unsigned long len, unsigned char* dst, unsigned long* dlen, void* wrk)
{ g_lzSrc = src; g_lzLen = len; g_lzDst = dst; g_lzOut = *dlen; g_lzWrk = wrk; memcpy(dst, "unpacked", 8); Log("lzo", dst); return 0; }
// CChunkedFileLoader::LoadChunkedFile(CChunkedFile&, CDataInputStream&)
static void* g_lcThis = 0; static void* g_lcFile = 0; static void* g_lcIn = 0; static const void* g_lcInData = 0;
void __fastcall LoadChunked(void* self, int, void* file, void* in) { g_lcThis = self; g_lcFile = file; g_lcIn = in; g_lcInData = g_msData; Log("loadchunked", file); }
}
// the root chunk prototype and the root chunks it creates (test vtables: slot 0 deleting destructor, slot 9 CreateNewInstance)
static unsigned char g_rootObj[4][0x40]; static int g_nroot = 0; static int g_protoNull = 0;
static void* g_fakeRootVtbl[11]; static void* g_protoVtbl[11];
extern "C" void* __fastcall FakeRootDeleting(void* self, int, unsigned int flags) { Log(flags == 1 ? "rootdel1" : "rootdelX", self); return self; }
extern "C" void* __fastcall ProtoCreate(void* self, int) { Log("create", self); if (g_protoNull) return 0; void* r = g_rootObj[g_nroot++ & 3]; *(void**)r = g_fakeRootVtbl; return r; }
struct Proto { void** vptr; }; static Proto g_proto;
static void InitFakes()
{
    for (int i = 0; i < 11; ++i) { g_fakeRootVtbl[i] = (void*)UnusedSlot; g_protoVtbl[i] = (void*)UnusedSlot; }
    g_fakeRootVtbl[0] = (void*)FakeRootDeleting; g_protoVtbl[9] = (void*)ProtoCreate; g_proto.vptr = g_protoVtbl;
    for (int k = 0; k < 4; ++k) memset(g_rootObj[k], 0, sizeof g_rootObj[k]);
}
static void* NewRoot(int k) { *(void**)g_rootObj[k] = g_fakeRootVtbl; return g_rootObj[k]; }

extern "C" CP* __fastcall CallRow(void* self, int, CP* result, const void* data, unsigned long size, void* pool);
static void Common(unsigned char* loader, CP& result, void* stream)
{
    // the image went through LoadFile: one root (count block), the file and its count block; the result owns the file
    CHECK(g_nnew == 3 && g_newSize[0] == 0xC && g_newSize[1] == 0x14 && g_newSize[2] == 0xC);
    void* file = g_heap[1]; Info* fileInfo = (Info*)g_heap[2];
    CHECK(result.data == file && result.info == fileInfo && fileInfo->refs == 1 && fileInfo->del == DelFile);
    CHECK(g_lcThis == loader && g_lcFile == file && g_lcIn == stream && Count("loadchunked") == 1);
    CHECK(Find("msctor", stream) >= 0 && Find("msctor", stream) < Find("loadchunked", file));
    CHECK(Find("msdtor", stream) > Find("loadchunked", file) && Count("msdtor") == 1 && Count("msctor") == 1);
    CHECK(Count("delete") == 0);
}
int main()
{
    InitFakes();
    unsigned char loader[0x5C]; memset(loader, 0, sizeof loader); *(void**)(loader + 0x10) = &g_proto;
    int pool = 0;
    {   // ">>>>": the image is read in place
        Reset(); g_keep = 1; NewRoot(g_nroot & 3);
        static const unsigned char img[12] = { '>', '>', '>', '>', 1, 2, 3, 4, 5, 6, 7, 8 };
        CP result = { 0, 0 };
        CP* r = CallRow(loader, 0, &result, img, sizeof img, &pool);
        CHECK(r == &result);
        CHECK(g_msData == img && g_msSize == sizeof img);
        CHECK(Count("tbctor") == 0 && Count("lzo") == 0 && Count("tbdtor") == 0);
        Common(loader, result, g_logp[Find("msctor", (void*)-1)]);
    }
    {   // compressed: the first dword is the unpacked size; a temporary buffer of size + 3, LZO into it, read from it
        Reset(); g_keep = 1; NewRoot(g_nroot & 3);
        static unsigned char img[20] = { 0x10, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };
        CP result = { 0, 0 };
        g_lzWrk = (void*)1;
        CP* r = CallRow(loader, 0, &result, img, sizeof img, &pool);
        CHECK(r == &result);
        CHECK(g_tbSize == 0x13 && g_tbPool == &pool && g_tbScratch == (void*)g_scratch);
        CHECK(g_lzSrc == img + 4 && g_lzLen == sizeof img - 4 && g_lzDst == g_buf && g_lzOut == 0x10 && g_lzWrk == 0);
        CHECK(g_msData == g_buf && g_msSize == 0x10 && g_lcInData == g_buf);
        CHECK(Find("tbctor", (void*)-1) < Find("lzo", g_buf) && Find("lzo", g_buf) < Find("msctor", (void*)-1));
        void* stream = g_logp[Find("msctor", (void*)-1)];
        CHECK(Find("tbdtor", (void*)-1) > Find("msdtor", stream) && Count("tbdtor") == 1);   // the stream dies before the buffer
        Common(loader, result, stream);
    }
    if (g_fail) { printf("00a99510_TEST FAIL\n"); return 1; }
    printf("00a99510_TEST PASS\n");
    return 0;
}
