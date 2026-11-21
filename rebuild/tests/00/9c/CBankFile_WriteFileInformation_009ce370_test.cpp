// Behaviour test for 0x009CE370 CBankFile::WriteFileInformation(bool) (links the REAL row object). Raw native-layout storage only:
//   CBankFile  vptr +0x00 (slot 8 GetDataFormatVersion), RuntimeData +0x20 (12-byte records: DataOffset +0, DataSize +4, Valid +9),
//              UpdateData +0x2C (pointers; RequiresUpdate +0x14), PackedDataOffset +0x38 / PackedDataSize +0x48 (Size +4), BankFile +0x7C
//              (Data, Info), Filename +0x84, RetailMode +0x8C, WrittenToFile +0x8E, Alignment +0x98, BankFormatVersion +0x9C, HeaderSize +0xA0
//   CDiskFileWin32  vtable slots 1 Open(const CWideString&, FileAccessType, unsigned long), 2 Close, 5 SetPosition(long), 7 GetPosition
// Every callee logs a letter and its arguments; the test compares the whole call sequence and every argument and value with a model:
// MakeWritable when the file is not yet writable (position kept across the read/write reopen), the stream on the bank file, "BBBB" at 0,
// the seven header words (format version, data version, 1 or the random validation value, alignment, highest initialised index,
// initialised count, the aligned end of entry data, starting from the old HeaderSize), HeaderSize = the stream position, the padding to the
// alignment, then the entry list at the aligned end (is_valid) or the recovery file (not retail mode), and the stream's destructor.
// An entry counts when RuntimeData[i].Valid, RuntimeData[i].DataOffset != 0 and !UpdateData[i]->RequiresUpdate; its end is the
// offset + size from the packed arrays (index - 1) when they are in use, else from RuntimeData[i].
#include <stdio.h>
#include <string.h>
#pragma comment(linker, "/alternatename:@CallRow@12=?WriteFileInformation@CBankFile@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z=@OsCtor@12")
#pragma comment(linker, "/alternatename:??1CFileDataOutputStream@@UAE@XZ=@OsDtor@4")
#pragma comment(linker, "/alternatename:?SetPosition@CFileDataOutputStream@@UAEXK@Z=@OsSetPos@12")
#pragma comment(linker, "/alternatename:?GetPosition@CFileDataOutputStream@@UBEKXZ=@OsGetPos@4")
#pragma comment(linker, "/alternatename:?Write@CFileDataOutputStream@@UAEXPBXJ@Z=@OsWrite@16")
#pragma comment(linker, "/alternatename:?WriteULONG@CDataOutputStream@@QAEXK@Z=@OsULong@12")
#pragma comment(linker, "/alternatename:??ACPackedUIntArray@@QBEKK@Z=@PackedGet@12")
#pragma comment(linker, "/alternatename:?GetRandomValidationValue@CBankFile@@AAEKXZ=@Rand@4")
#pragma comment(linker, "/alternatename:?WritePadding@CBankFile@@AAEXK@Z=@Pad@12")
#pragma comment(linker, "/alternatename:?WriteRecoveryFile@CBankFile@@AAEXKKK@Z=@Recovery@20")
#pragma comment(linker, "/alternatename:?InternalWriteEntryList@CBankFile@@QAEKAAVCDataOutputStream@@@Z=@EntryList@12")
extern "C" void __fastcall CallRow(void* self, int edx, int is_valid);
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall OnFault(void*) { ExitProcess(3); return 0; }

static int g_fail = 0, g_reports = 0;
static void Check(bool ok, int line, int scene) { if (!ok) { if (g_reports++ < 20) printf("check failed at line %d (scene %d)\n", line, scene); g_fail = 1; } }
static char g_log[128]; static int g_logn; static unsigned long g_arg[128]; static int g_argn;
static void Log(char c) { if (g_logn < 120) g_log[g_logn++] = c; g_log[g_logn] = 0; }
static void Arg(unsigned long v) { if (g_argn < 120) g_arg[g_argn++] = v; }

// ---- the scene ----
struct Rt { unsigned long DataOffset, DataSize; unsigned char DataType; bool Valid; unsigned char pad[2]; };
struct Upd { unsigned char pad[0x14]; bool RequiresUpdate; bool Exists; unsigned char pad2[2]; };
static unsigned char g_bank[0x110];
static Rt g_rt[40]; static Upd g_upd[40]; static Upd* g_updp[40];
static unsigned long g_poff[40], g_psize[40];
static const void* g_bankvt[16]; static const void* g_filevt[16];
static struct { const void* vptr; } g_file;
static unsigned long g_filepos, g_dfv, g_rand, g_streampos; static void* g_os;
template<class T> static T& At(int off) { return *(T*)(g_bank + off); }

extern "C" unsigned long __fastcall Dfv(void* self) { Log('V'); Arg((unsigned long)self); return g_dfv; }
extern "C" long __fastcall FileGetPos(void* self) { Log('p'); Arg((unsigned long)self); return (long)g_filepos; }
extern "C" void __fastcall FileClose(void* self) { Log('x'); Arg((unsigned long)self); }
extern "C" void __fastcall FileOpen(void* self, int, const void* name, int access, unsigned long flags) { Log('o'); Arg((unsigned long)self); Arg((unsigned long)name); Arg(access); Arg(flags); }
extern "C" void __fastcall FileSetPos(void* self, int, long pos) { Log('s'); Arg((unsigned long)self); Arg(pos); }
extern "C" void* __fastcall OsCtor(void* self, int, void* file) { Log('C'); g_os = self; Arg((unsigned long)file); return self; }
extern "C" void __fastcall OsDtor(void* self) { Log('D'); Arg(self == g_os); }
extern "C" void __fastcall OsSetPos(void* self, int, unsigned long pos) { Log('S'); Arg(self == g_os); Arg(pos); }
extern "C" unsigned long __fastcall OsGetPos(const void* self) { Log('G'); Arg(self == g_os); return g_streampos; }
extern "C" void __fastcall OsWrite(void* self, int, const void* data, long size) { Log('W'); Arg(self == g_os); Arg(size); Arg(memcmp(data, "BBBB", 4) == 0); }
extern "C" void __fastcall OsULong(void* self, int, unsigned long v) { Log('U'); Arg(self == g_os); Arg(v); }
extern "C" unsigned long __fastcall PackedGet(const void* self, int, unsigned long index)
{
    if (self == g_bank + 0x38) { Log('a'); return index < 40 ? g_poff[index] : 0xdead0000; }
    if (self == g_bank + 0x48) { Log('b'); return index < 40 ? g_psize[index] : 0xdead0000; }
    Log('?'); return 0;
}
extern "C" unsigned long __fastcall Rand(void* self) { Log('R'); Arg((unsigned long)self); return g_rand; }
extern "C" void __fastcall Pad(void* self, int, unsigned long n) { Log('P'); Arg((unsigned long)self); Arg(n); }
extern "C" void __fastcall Recovery(void* self, int, unsigned long a, unsigned long b, unsigned long c) { Log('E'); Arg((unsigned long)self); Arg(a); Arg(b); Arg(c); }
extern "C" unsigned long __fastcall EntryList(void* self, int, void* os) { Log('L'); Arg((unsigned long)self); Arg(os == g_os); return 0; }

static unsigned g_seed = 777u;
static unsigned Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }

static void Run(int scene, int n, bool packed, bool writable, bool retail, bool is_valid, unsigned long align, unsigned long header)
{
    memset(g_bank, 0, sizeof g_bank);
    At<const void*>(0) = g_bankvt;
    At<Rt*>(0x20) = g_rt; At<Rt*>(0x24) = g_rt + n + 3; At<Rt*>(0x28) = g_rt + n + 3;   // longer than UpdateData: the loop runs over UpdateData
    At<Upd**>(0x2c) = g_updp; At<Upd**>(0x30) = g_updp + n; At<Upd**>(0x34) = g_updp + n;
    At<unsigned long>(0x3c) = packed ? 7 : 0; At<unsigned long>(0x4c) = packed ? 9 : 0;
    At<void*>(0x7c) = &g_file; At<void*>(0x80) = (void*)0x4444;
    At<bool>(0x8c) = retail; At<bool>(0x8e) = writable;
    At<unsigned long>(0x98) = align; At<unsigned long>(0x9c) = 0x100 + scene; At<unsigned long>(0xa0) = header;
    g_filepos = Rnd(); g_dfv = Rnd() & 0xff; g_rand = Rnd(); g_streampos = 28 + (Rnd() % 8);
    g_logn = 0; g_log[0] = 0; g_argn = 0; g_os = 0;
    CallRow(g_bank, 0, is_valid);
    // the model
    char log[128]; int ln = 0; unsigned long arg[128]; int an = 0;
#define L(c) (log[ln++] = (c))
#define A(v) (arg[an++] = (unsigned long)(v))
    if (!writable) { L('p'); A(&g_file); L('x'); A(&g_file); L('o'); A(&g_file); A(g_bank + 0x84); A(2); A(4); L('s'); A(&g_file); A(g_filepos); }
    L('C'); A(&g_file); L('S'); A(1); A(0); L('W'); A(1); A(4); A(1);
    unsigned long count = 0, maxi = 0, end = header;
    for (int i = 0; i < n; ++i) {
        if (!(g_rt[i].Valid && g_rt[i].DataOffset != 0 && !g_updp[i]->RequiresUpdate)) continue;
        ++count;
        unsigned long off, size;
        if (packed) { L('a'); off = i == 0 ? 0xdead0000 : g_poff[i - 1]; L('b'); size = i == 0 ? 0xdead0000 : g_psize[i - 1]; }   // entry 0: index - 1 wraps
        else { off = g_rt[i].DataOffset; size = g_rt[i].DataSize; }
        if (off + size > end) end = off + size;
        if ((unsigned long)i > maxi) maxi = i;
    }
    unsigned long info = (end + align - 1) / align * align;
    L('R'); A(g_bank); L('V'); A(g_bank);
    unsigned long words[7] = { 0x100 + (unsigned long)scene, g_dfv, is_valid ? 1 : g_rand, align, maxi, count, info };
    for (int k = 0; k < 7; ++k) { L('U'); A(1); A(words[k]); }
    L('G'); A(1);
    L('P'); A(g_bank); A((g_streampos + align - 1) / align * align - g_streampos);
    if (!is_valid) { if (!retail) { L('E'); A(g_bank); A(g_rand); A(maxi); A(count); } }
    else { L('S'); A(1); A(info); L('L'); A(g_bank); A(1); }
    L('D'); A(1); log[ln] = 0;
    if (strcmp(log, g_log) != 0 && g_reports < 3) printf("model %s\nrow   %s\n", log, g_log);
    Check(strcmp(log, g_log) == 0, __LINE__, scene);
    Check(an == g_argn && memcmp(arg, g_arg, an * sizeof arg[0]) == 0, __LINE__, scene);
    Check(At<bool>(0x8e) == true, __LINE__, scene);
    Check(At<unsigned long>(0xa0) == g_streampos, __LINE__, scene);
}
int main()
{
    SetUnhandledExceptionFilter((void*)OnFault);
    g_bankvt[8] = (const void*)Dfv;
    g_filevt[1] = (const void*)FileOpen; g_filevt[2] = (const void*)FileClose; g_filevt[5] = (const void*)FileSetPos; g_filevt[7] = (const void*)FileGetPos;
    g_file.vptr = g_filevt;
    for (int i = 0; i < 40; ++i) g_updp[i] = &g_upd[i];
    for (int scene = 0; scene < 3000; ++scene) {
        int n = (int)(Rnd() % 24);
        for (int i = 0; i < 40; ++i) {
            g_rt[i].Valid = (Rnd() % 4) != 0; g_rt[i].DataOffset = (Rnd() % 5) ? Rnd() % 100000 : 0; g_rt[i].DataSize = Rnd() % 5000;
            g_upd[i].RequiresUpdate = (Rnd() % 5) == 0;
            g_poff[i] = Rnd() % 200000; g_psize[i] = Rnd() % 9000;
        }
        static const unsigned long aligns[] = { 1, 4, 16, 512, 2048, 4096 };
        Run(scene, n, (scene & 1) != 0, (scene & 2) != 0, (scene & 4) != 0, (scene & 8) != 0, aligns[Rnd() % 6], (scene % 3) ? 32 : 300000);
    }
    if (!g_fail) printf("009ce370_TEST PASS\n");
    return g_fail;
}