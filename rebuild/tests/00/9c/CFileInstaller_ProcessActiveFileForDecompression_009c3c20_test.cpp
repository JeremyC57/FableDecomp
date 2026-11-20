// Behaviour test for 0x009C3C20 CFileInstaller::ProcessActiveFileForDecompression (links the REAL row object: the expanded
// SetDirtyDiscError, GetErrorFlag, CPendingWrite constructor, the list iteration / erase and the list<CPendingWrite>::push_back
// COMDAT run as compiled, on the CRT heap).
// Fixtures (raw native layout, every offset read or written by an instruction of the row):
//   CFileInstaller (0xA0): CriticalSection +4, BufferChunkSize +0x64, ExitThread +0x7A, Event +0x84; static DirtyDisc.
//   CActiveFile (0xB0): InputSize +0x14, Decompressor +0x38 (its ErrorFlag +0x8E), PendingWrites +0x90 and PendingDecompression
//     +0x94 (STLport lists: one pointer to the sentinel node; nodes are next, prev, value), DecompressSrcPos +0x98, DecompressDstPos
//     +0x9C, PDecompressSrcBuffer +0xA0, PDecompressDstBuffer +0xA4, DecompressDstUsed +0xA8, DecompressFinish +0xAC.
//   CBufferChunk: Mem +0. CActiveRead: FilePos +4, TransferSize +8, PBufferChunk +0xC. CPendingWrite: PBufferChunk, FilePos,
//     WriteLength (12 bytes, in a 20-byte list node).
// Stubs (the row's out-of-line callees, each recorded with its arguments and a snapshot of the file's fields at that moment):
//   CCompressorZlib::HasStreamInput 0x00A3A7E0 / DecompressStream 0x00A3A840 / FinishDecompressStream 0x00A3ABB0 / SetStreamInput
//   0x00A3AB90 (a scripted stream), CTBaseSingleton<CFileInstaller>::Get 0x00404440 (a second installer object, so `this` and the
//   singleton are told apart), CFileInstaller::AllocateBuffer 0x009C3A60 / FreeBuffer 0x009C3330 / StartWritingFile 0x009C3660,
//   CCriticalSectionLock constructor / destructor 0x00A62920 / 0x00A629A0 (pairing checked), the global operator delete, and the
//   KERNEL32 imports Sleep / SetEvent (the test repoints its own import slots at recording functions).
// Reference model (independent): the same decisions written against the scripted stream -- allocate a destination chunk under the
//   lock when there is none (give up when none is free); decompress at most 0x40000 bytes a call while space remains, the stream
//   has input and is not finished, sleeping 1 ms after each call; a stream error raises the dirty-disc error and returns at once;
//   finish the stream when it reported finished and the file's last block is in; release the source chunk when the stream ran out
//   of input; queue the destination chunk (position, bytes used) when it is full or the file is finished, and signal the event.
//   With no stream input: release the source chunk, then under the lock take the first pending read at the current source
//   position -- the first block must start with "biz!" (else dirty disc, the read deleted) and carries the output size at +4 --
//   advance the source position, unlink it, hand its data to the stream, flag the last block and delete the read.
// Checks: (1) hand-written scenarios on every decision; (2) 4000 seeded random scenarios; in each the full call list (arguments
//   and field snapshots), both lists, the file / installer / singleton images and DirtyDisc are compared with the model.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned long UL;
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void* __stdcall GetModuleHandleA(const char* name);
extern "C" __declspec(dllimport) int __stdcall VirtualProtect(void* address, UL size, UL protect, UL* old_protect);

class CCriticalSectionLock
{
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool try_lock);
    ~CCriticalSectionLock();
    UL pad[2];
};
class CCompressorZlib
{
public:
    bool HasStreamInput() const;
    unsigned long DecompressStream(void* dst, unsigned long dst_size, bool& finished);
    void FinishDecompressStream();
    void SetStreamInput(const void* src, unsigned long src_size);
};
template<class T> class CTBaseSingleton { public: static T& __fastcall Get(); };
class CFileInstaller
{
public:
    class CBufferChunk { public: unsigned char* Mem; bool Used; bool IsInput; };
    class CActiveFile { public: unsigned char b[0xB0]; };
    static void Call(void* installer, CActiveFile& file) { ((CFileInstaller*)installer)->ProcessActiveFileForDecompression(file); }
    static bool& Dirty() { return DirtyDisc; }
protected:
    CBufferChunk* AllocateBuffer(bool is_input);
    void FreeBuffer(CBufferChunk* chunk);
    void StartWritingFile(CActiveFile& active_file, unsigned long output_size);
    void ProcessActiveFileForDecompression(CActiveFile& active_file);
    static bool DirtyDisc;
};
bool CFileInstaller::DirtyDisc;

template<class T> static T& At(void* base, int off) { return *(T*)((unsigned char*)base + off); }

// ---- fixtures
struct Node { Node* next; Node* prev; UL value[3]; };
struct Read { UL PActiveFile; long FilePos; long TransferSize; CFileInstaller::CBufferChunk* PBufferChunk; UL tail[6]; };
enum { MAXREADS = 5, MAXSTEPS = 12, DSTBYTES = 0x90000 };
static unsigned char g_inst[0xA0 + 8], g_single[0xA0 + 8];
static CFileInstaller::CActiveFile g_af;
static unsigned char g_dstmem[DSTBYTES];
static unsigned char g_readmem[MAXREADS][64];
static CFileInstaller::CBufferChunk g_dstchunk, g_srcchunk, g_readchunk[MAXREADS];
static Node* g_writes; static Node* g_reads;                  // the two sentinels
static Read* g_pread[MAXREADS]; static Node* g_rnode[MAXREADS];

struct DStep { UL ret; bool finished, error, deactivate; };
struct RStep { long pos; long size; bool magic_ok; UL out_size; };
struct Scn
{
    bool active0, have_dst, alloc_null, decompress_finish, finish_deactivates, have_src;
    UL used0, chunk_size, dst_pos0, src_pos0, input_size;
    int nsteps; DStep steps[MAXSTEPS];
    int nreads; RStep reads[MAXREADS];
};
static Scn g_s;

// ---- call log
enum { E_HAS = 1, E_DECOMP, E_FINISH, E_SETINPUT, E_GET, E_ALLOC, E_FREE, E_STARTWRITE, E_LOCK, E_UNLOCK, E_DELETE, E_SLEEP, E_SETEVENT };
struct State { UL Src, Dst, Used, DstPos, SrcPos, Finish, Dirty, Exit; };
struct Ev { int id; UL a, b, c; State s; };
enum { MAXEV = 160 };
static Ev g_got[MAXEV], g_want[MAXEV]; static int g_ngot, g_nwant;
static bool g_armed;
static State Live()
{
    State s; s.Src = At<UL>(&g_af, 0xA0); s.Dst = At<UL>(&g_af, 0xA4); s.Used = At<UL>(&g_af, 0xA8); s.DstPos = At<UL>(&g_af, 0x9C);
    s.SrcPos = At<UL>(&g_af, 0x98); s.Finish = At<unsigned char>(&g_af, 0xAC); s.Dirty = CFileInstaller::Dirty() ? 1 : 0; s.Exit = At<unsigned char>(g_single, 0x7A);
    return s;
}
static void Got(int id, UL a = 0, UL b = 0, UL c = 0)
{
    if (!g_armed) return;
    if (g_ngot < MAXEV) { Ev& e = g_got[g_ngot]; e.id = id; e.a = a; e.b = b; e.c = c; e.s = Live(); }
    ++g_ngot;
}

// ---- the scripted stream (shared by the stubs and, through its own counters, by the model)
static DStep Step(int index, UL requested)
{
    DStep st;
    if (index < g_s.nsteps) st = g_s.steps[index]; else { st.ret = requested; st.finished = true; st.error = false; st.deactivate = false; }
    if (st.ret > requested) st.ret = requested;
    if (st.ret == 0 && !st.finished && !st.error && !st.deactivate) st.finished = true;      // always makes progress
    return st;
}
static bool g_active; static int g_step; static int g_lockdepth; static void* g_lockstack[8]; static int g_lockerr;

bool CCompressorZlib::HasStreamInput() const { Got(E_HAS, (UL)this); return g_active; }
unsigned long CCompressorZlib::DecompressStream(void* dst, unsigned long dst_size, bool& finished)
{
    Got(E_DECOMP, (UL)((unsigned char*)dst - g_dstmem), dst_size, ((UL)finished) | ((UL)this == (UL)&g_af + 0x38 ? 0 : 0x100));
    DStep st = Step(g_step++, dst_size);
    finished = st.finished;
    if (st.deactivate) g_active = false;
    if (st.error) At<unsigned char>(&g_af, 0x8E) = 1;
    return st.error ? 0xDEAD0000 : st.ret;
}
void CCompressorZlib::FinishDecompressStream() { Got(E_FINISH, (UL)this); if (g_s.finish_deactivates) g_active = false; }
void CCompressorZlib::SetStreamInput(const void* src, unsigned long src_size) { Got(E_SETINPUT, (UL)this, (UL)src, src_size); g_active = true; }
template<> CFileInstaller& __fastcall CTBaseSingleton<CFileInstaller>::Get() { Got(E_GET); return *(CFileInstaller*)g_single; }
CFileInstaller::CBufferChunk* CFileInstaller::AllocateBuffer(bool is_input) { Got(E_ALLOC, (UL)this, is_input ? 1 : 0); return g_s.alloc_null ? 0 : &g_dstchunk; }
void CFileInstaller::FreeBuffer(CBufferChunk* chunk) { Got(E_FREE, (UL)this, (UL)chunk); }
void CFileInstaller::StartWritingFile(CActiveFile& active_file, unsigned long output_size) { Got(E_STARTWRITE, (UL)this, (UL)&active_file, output_size); }
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool try_lock)
{
    Got(E_LOCK, (UL)&section, try_lock ? 1 : 0);
    if (g_lockdepth < 8) g_lockstack[g_lockdepth] = this;
    ++g_lockdepth;
}
CCriticalSectionLock::~CCriticalSectionLock()
{
    --g_lockdepth;
    if (g_lockdepth < 0 || g_lockdepth >= 8 || g_lockstack[g_lockdepth] != this) ++g_lockerr;
    Got(E_UNLOCK, (UL)g_lockdepth);
}
void __cdecl operator delete(void* p) { Got(E_DELETE, (UL)p); free(p); }
static void __stdcall FakeSleep(UL milliseconds) { Got(E_SLEEP, milliseconds); }
static int __stdcall FakeSetEvent(void* event) { Got(E_SETEVENT, (UL)event); return 1; }

// repoint one of this executable's own KERNEL32 import slots
static bool PatchImport(const char* name, void* replacement)
{
    unsigned char* base = (unsigned char*)GetModuleHandleA(0);
    UL pe = At<UL>(base, 0x3C); UL import_rva = At<UL>(base, pe + 0x80);
    if (!import_rva) return false;
    for (UL* desc = (UL*)(base + import_rva); desc[3]; desc += 5)
    {
        UL* names = (UL*)(base + (desc[0] ? desc[0] : desc[4])); void** slots = (void**)(base + desc[4]);
        for (int i = 0; names[i]; ++i)
        {
            if (names[i] & 0x80000000) continue;
            if (strcmp((const char*)(base + names[i] + 2), name)) continue;
            UL old; if (!VirtualProtect(&slots[i], 4, 4, &old)) return false;
            slots[i] = replacement; VirtualProtect(&slots[i], 4, old, &old);
            return true;
        }
    }
    return false;
}

// ---- reference model
static State m; static bool m_active; static int m_step; static int m_err;
static struct { UL chunk, pos, len; } m_push; static int m_npush;
static int m_taken;                                                    // index of the read consumed (unlinked), or -1
static void Want(int id, UL a = 0, UL b = 0, UL c = 0)
{
    if (g_nwant < MAXEV) { Ev& e = g_want[g_nwant]; e.id = id; e.a = a; e.b = b; e.c = c; e.s = m; }
    ++g_nwant;
}
static bool MHas() { Want(E_HAS, (UL)&g_af + 0x38); return m_active; }
static void MDirty() { Want(E_GET); Want(E_LOCK, (UL)g_single + 4, 0); m.Dirty = 1; Want(E_GET); Want(E_GET); m.Exit = 1; Want(E_UNLOCK, 0); }
static void Model()
{
    const UL lock = (UL)g_inst + 4;
    g_nwant = 0; m_npush = 0; m_taken = -1; m_err = 0; m_step = 0; m_active = g_s.active0;
    m = Live();
    if (MHas())
    {
        if (m.Dst == 0)
        {
            Want(E_LOCK, lock, 0); Want(E_GET); Want(E_ALLOC, (UL)g_single, 0);
            m.Dst = g_s.alloc_null ? 0 : (UL)&g_dstchunk; m.Used = 0; Want(E_UNLOCK, 0);
        }
        if (m.Dst != 0)
        {
            UL off = m.Used, dst_size = g_s.chunk_size - m.Used; bool finished = false;
            while (dst_size > 0 && !finished && MHas())
            {
                UL n = dst_size < 0x40000 ? dst_size : 0x40000;
                Want(E_DECOMP, off, n, 0);
                DStep st = Step(m_step++, n);
                finished = st.finished; if (st.deactivate) m_active = false;
                if (st.error) { m_err = 1; MDirty(); return; }
                m.Used += st.ret; off += st.ret; dst_size -= st.ret;
                Want(E_SLEEP, 1);
            }
            bool file_finished = finished && m.Finish;
            if (file_finished) { Want(E_FINISH, (UL)&g_af + 0x38); if (g_s.finish_deactivates) m_active = false; }
            if (!MHas()) { Want(E_LOCK, lock, 0); Want(E_GET); Want(E_FREE, (UL)g_single, m.Src); m.Src = 0; Want(E_UNLOCK, 0); }
            if (m.Used == g_s.chunk_size || file_finished)
            {
                UL size = m.Used;
                Want(E_LOCK, lock, 0);
                m_push.chunk = m.Dst; m_push.pos = m.DstPos; m_push.len = size; m_npush = 1;
                m.Dst = 0; m.DstPos += size;
                Want(E_SETEVENT, At<UL>(g_inst, 0x84)); Want(E_UNLOCK, 0);
            }
        }
        return;
    }
    if (m.Src != 0) { Want(E_LOCK, lock, 0); Want(E_GET); Want(E_FREE, (UL)g_single, m.Src); m.Src = 0; Want(E_UNLOCK, 0); }
    Want(E_LOCK, lock, 0);
    for (int i = 0; i < g_s.nreads; ++i)
    {
        const RStep& r = g_s.reads[i];
        if ((UL)r.pos != m.SrcPos) continue;
        UL skip = 0;
        if (r.pos == 0)
        {
            if (!r.magic_ok) { MDirty(); Want(E_DELETE, (UL)g_pread[i]); Want(E_UNLOCK, 0); return; }
            skip = 8;
            Want(E_STARTWRITE, (UL)g_inst, (UL)&g_af, r.out_size);
        }
        m.SrcPos += (UL)r.size; m_taken = i; m.Src = (UL)&g_readchunk[i];
        Want(E_SETINPUT, (UL)&g_af + 0x38, (UL)g_readmem[i] + skip, (UL)r.size - skip);
        m_active = true;
        if ((UL)(r.pos + r.size) >= g_s.input_size) m.Finish = 1;
        Want(E_DELETE, (UL)g_pread[i]); Want(E_UNLOCK, 0);
        return;
    }
    Want(E_UNLOCK, 0);
}

static int g_fail, g_cases;
static void Fail(const char* what, long got, long want)
{
    if (g_fail++ < 16) printf("FAIL case %d: %s got 0x%lX want 0x%lX\n", g_cases, what, got, want);
}
static UL g_seed = 777;
static UL Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }

static void Run()
{
    ++g_cases;
    int i;
    for (i = 0; i < (int)sizeof g_inst; ++i) { g_inst[i] = (unsigned char)(Rnd() | 1); g_single[i] = (unsigned char)(Rnd() | 1); }
    for (i = 0; i < (int)sizeof g_af.b; ++i) g_af.b[i] = (unsigned char)(Rnd() | 1);
    At<UL>(g_inst, 0x64) = g_s.chunk_size; At<UL>(g_single, 0x64) = g_s.chunk_size ^ 0x55;
    At<unsigned char>(g_single, 0x7A) = 0; CFileInstaller::Dirty() = false;
    g_dstchunk.Mem = g_dstmem; g_srcchunk.Mem = (unsigned char*)0x00BAD000;
    At<UL>(&g_af, 0x14) = g_s.input_size;
    At<unsigned char>(&g_af, 0x8E) = 0;
    g_writes = (Node*)malloc(sizeof(Node)); g_writes->next = g_writes->prev = g_writes;
    g_reads = (Node*)malloc(sizeof(Node)); g_reads->next = g_reads->prev = g_reads;
    At<Node*>(&g_af, 0x90) = g_writes; At<Node*>(&g_af, 0x94) = g_reads;
    for (i = 0; i < g_s.nreads; ++i)
    {
        const RStep& r = g_s.reads[i];
        memset(g_readmem[i], 0x40 + i, sizeof g_readmem[i]);
        memcpy(g_readmem[i], r.magic_ok ? "biz!" : (i & 1) ? "biz?" : "Biz!", 4); memcpy(g_readmem[i] + 4, &r.out_size, 4);
        g_readchunk[i].Mem = g_readmem[i];
        Read* p = (Read*)malloc(sizeof(Read)); memset(p, 0x77, sizeof(Read));
        p->FilePos = r.pos; p->TransferSize = r.size; p->PBufferChunk = &g_readchunk[i]; g_pread[i] = p;
        Node* n = (Node*)malloc(12); n->value[0] = (UL)p; n->prev = g_reads->prev; n->next = g_reads; g_reads->prev->next = n; g_reads->prev = n; g_rnode[i] = n;
    }
    At<UL>(&g_af, 0x98) = g_s.src_pos0; At<UL>(&g_af, 0x9C) = g_s.dst_pos0;
    At<UL>(&g_af, 0xA0) = g_s.have_src ? (UL)&g_srcchunk : 0; At<UL>(&g_af, 0xA4) = g_s.have_dst ? (UL)&g_dstchunk : 0;
    At<UL>(&g_af, 0xA8) = g_s.used0; At<unsigned char>(&g_af, 0xAC) = g_s.decompress_finish ? 1 : 0;
    unsigned char inst0[sizeof g_inst], single0[sizeof g_single]; CFileInstaller::CActiveFile af0 = g_af;
    memcpy(inst0, g_inst, sizeof g_inst); memcpy(single0, g_single, sizeof g_single);
    Model();
    g_ngot = 0; g_active = g_s.active0; g_step = 0; g_lockdepth = 0; g_lockerr = 0;
    g_armed = true;
    CFileInstaller::Call(g_inst, g_af);
    g_armed = false;
    // the call list
    if (g_ngot != g_nwant) Fail("call count", g_ngot, g_nwant);
    for (i = 0; i < g_ngot && i < g_nwant && i < MAXEV; ++i)
    {
        const Ev& g = g_got[i]; const Ev& w = g_want[i];
        if (g.id != w.id) { Fail("call id", g.id, w.id); printf("  (call %d)\n", i); break; }
        if (g.id != E_UNLOCK && (g.a != w.a || g.b != w.b || g.c != w.c)) { Fail("call arguments", (long)g.a, (long)w.a); printf("  (call %d id %d: %lX %lX %lX / %lX %lX %lX)\n", i, g.id, g.a, g.b, g.c, w.a, w.b, w.c); break; }
        if (memcmp(&g.s, &w.s, sizeof(State))) { Fail("fields at call", (long)g.s.Used, (long)w.s.Used); printf("  (call %d id %d)\n", i, g.id); break; }
    }
    if (g_lockdepth != 0 || g_lockerr) Fail("lock pairing", g_lockdepth, g_lockerr);
    // final images
    State f = Live();
    if (memcmp(&f, &m, sizeof(State))) Fail("final fields", (long)f.Used, (long)m.Used);
    At<UL>(&af0, 0x98) = m.SrcPos; At<UL>(&af0, 0x9C) = m.DstPos; At<UL>(&af0, 0xA0) = m.Src; At<UL>(&af0, 0xA4) = m.Dst; At<UL>(&af0, 0xA8) = m.Used;
    At<unsigned char>(&af0, 0xAC) = (unsigned char)m.Finish; At<unsigned char>(&af0, 0x8E) = (unsigned char)m_err;
    if (memcmp(&af0, &g_af, sizeof g_af)) Fail("file image", 0, 0);
    if (memcmp(inst0, g_inst, sizeof g_inst)) Fail("installer written", 0, 0);
    At<unsigned char>(single0, 0x7A) = (unsigned char)m.Exit;
    if (memcmp(single0, g_single, sizeof g_single)) Fail("singleton image", 0, 0);
    // the pending-write list
    int npush = 0; Node* n;
    for (n = g_writes->next; n != g_writes; n = n->next) ++npush;
    if (npush != m_npush) Fail("pending writes", npush, m_npush);
    if (npush == 1 && m_npush == 1)
    {
        n = g_writes->next;
        if (n->prev != g_writes || n->next != g_writes || g_writes->prev != n) Fail("pending-write links", 0, 0);
        if (n->value[0] != m_push.chunk || n->value[1] != m_push.pos || n->value[2] != m_push.len) Fail("pending write", (long)n->value[2], (long)m_push.len);
    }
    for (n = g_writes->next; n != g_writes; ) { Node* next = n->next; free(n); n = next; }
    free(g_writes);
    // the pending-read list: every node but the consumed one, in order, links intact
    Node* prev = g_reads; n = g_reads->next;
    for (i = 0; i < g_s.nreads; ++i)
    {
        if (i == m_taken) continue;
        if (n != g_rnode[i] || n->prev != prev || n->value[0] != (UL)g_pread[i]) { Fail("pending-read list", i, 0); break; }
        prev = n; n = n->next;
    }
    if (i == g_s.nreads && (n != g_reads || g_reads->prev != prev)) Fail("pending-read list end", 0, 0);
    bool deleted[MAXREADS]; for (i = 0; i < MAXREADS; ++i) deleted[i] = false;
    for (i = 0; i < g_nwant && i < MAXEV; ++i) if (g_want[i].id == E_DELETE) for (int k = 0; k < g_s.nreads; ++k) if (g_want[i].a == (UL)g_pread[k]) deleted[k] = true;
    if (!g_fail)
    {
        for (i = 0; i < g_s.nreads; ++i) { if (!deleted[i]) free(g_pread[i]); if (i != m_taken) free(g_rnode[i]); }
        free(g_reads);
    }
}

static void Base()
{
    memset(&g_s, 0, sizeof g_s);
    g_s.active0 = true; g_s.have_dst = true; g_s.chunk_size = 0x1000; g_s.used0 = 0x100; g_s.dst_pos0 = 0x5000; g_s.src_pos0 = 0x300;
    g_s.input_size = 0x10000; g_s.have_src = true; g_s.finish_deactivates = true;
}
static void AddStep(UL ret, bool finished, bool error, bool deactivate) { DStep& s = g_s.steps[g_s.nsteps++]; s.ret = ret; s.finished = finished; s.error = error; s.deactivate = deactivate; }
static void AddRead(long pos, long size, bool magic_ok, UL out_size) { RStep& r = g_s.reads[g_s.nreads++]; r.pos = pos; r.size = size; r.magic_ok = magic_ok; r.out_size = out_size; }

static void Exact()
{
    // --- the stream has input
    Base(); AddStep(0x200, false, false, false); AddStep(0xD00, false, false, false); Run();              // fills the chunk exactly: queued
    Base(); AddStep(0x200, false, false, true); Run();                                                    // input runs out: source released, not queued
    Base(); AddStep(0x200, true, false, false); Run();                                                    // finished but not the last block
    Base(); g_s.decompress_finish = true; AddStep(0x200, true, false, false); Run();                      // finished + last block: finish, release, queue
    Base(); g_s.decompress_finish = true; g_s.finish_deactivates = false; AddStep(0x200, true, false, false); Run();   // stream still has input after finish
    Base(); g_s.decompress_finish = true; AddStep(0x200, false, false, true); Run();                      // last block flagged, stream not finished
    Base(); AddStep(0x80, false, true, false); Run();                                                     // stream error on the first call
    Base(); AddStep(0x80, false, false, false); AddStep(0x80, false, true, false); Run();                 // error on the second
    Base(); g_s.have_dst = false; AddStep(0x1000, false, false, false); Run();                            // allocate, then fill from 0
    Base(); g_s.have_dst = false; g_s.alloc_null = true; Run();                                           // no chunk free: nothing else
    Base(); g_s.used0 = 0x1000; Run();                                                                    // already full: no decompress, queued
    Base(); g_s.used0 = 0x1000; g_s.have_src = false; g_s.decompress_finish = true; Run();
    Base(); g_s.used0 = 0xFFF; AddStep(1, false, false, false); Run();                                    // one byte left
    Base(); g_s.used0 = 0xFFF; AddStep(0, false, false, true); Run();                                     // nothing produced, input gone
    Base(); g_s.chunk_size = 0x80000; g_s.used0 = 0; AddStep(0x40000, false, false, false); AddStep(0x40000, false, false, false); Run();   // the 0x40000 cap, twice
    Base(); g_s.chunk_size = 0x80000; g_s.used0 = 1; AddStep(0x40000, false, false, false); AddStep(0x3FFFF, false, false, false); Run();   // cap, then 0x3FFFF left
    Base(); g_s.chunk_size = 0x40000; g_s.used0 = 0; AddStep(0x40000, false, false, false); Run();        // exactly the cap
    Base(); g_s.chunk_size = 0x40001; g_s.used0 = 0; AddStep(0x40000, false, false, false); AddStep(1, false, false, false); Run();
    Base(); AddStep(0x10, false, false, false); AddStep(0x10, false, false, false); AddStep(0x10, false, false, false); AddStep(0x10, true, false, false); Run();
    // --- no stream input
    Base(); g_s.active0 = false; Run();                                                                   // release the source, no read pending
    Base(); g_s.active0 = false; g_s.have_src = false; Run();
    Base(); g_s.active0 = false; AddRead(0x300, 0x40, true, 0); Run();                                    // a middle block
    Base(); g_s.active0 = false; AddRead(0x200, 0x40, true, 0); AddRead(0x340, 0x40, true, 0); Run();     // none at the position
    Base(); g_s.active0 = false; AddRead(0x200, 0x40, true, 0); AddRead(0x300, 0x40, true, 0); AddRead(0x300, 0x30, true, 0); Run();   // the first match wins
    Base(); g_s.active0 = false; g_s.src_pos0 = 0; AddRead(0, 0x40, true, 0x12345678); Run();             // the first block: header consumed
    Base(); g_s.active0 = false; g_s.src_pos0 = 0; AddRead(0, 0x40, false, 0x12345678); Run();            // bad magic
    Base(); g_s.active0 = false; g_s.src_pos0 = 0; AddRead(0x40, 0x40, true, 1); AddRead(0, 0x40, false, 2); AddRead(0, 0x40, true, 3); Run();
    Base(); g_s.active0 = false; g_s.input_size = 0x340; AddRead(0x300, 0x40, true, 0); Run();            // ends exactly at the input size: last block
    Base(); g_s.active0 = false; g_s.input_size = 0x341; AddRead(0x300, 0x40, true, 0); Run();            // one byte short
    Base(); g_s.active0 = false; g_s.input_size = 0x33F; AddRead(0x300, 0x40, true, 0); Run();
    Base(); g_s.active0 = false; g_s.src_pos0 = 0; g_s.input_size = 0x40; g_s.have_src = false; AddRead(0, 0x40, true, 0x99); Run();   // a one-block file
    Base(); g_s.active0 = false; g_s.src_pos0 = 0; g_s.input_size = 8; AddRead(0, 8, true, 0); Run();     // header only: size 0 handed on
}

int main()
{
    if (!PatchImport("Sleep", (void*)FakeSleep) || !PatchImport("SetEvent", (void*)FakeSetEvent)) { printf("009c3c20_TEST FAIL (import slots)\n"); return 1; }
    Exact();
    int exact = g_cases;
    static const UL SIZES[] = { 0x100, 0x1000, 0x40000, 0x40001, 0x80000 };
    for (int n = 0; n < 4000 && !g_fail; ++n)
    {
        Base();
        g_s.active0 = (Rnd() % 3) != 0;
        g_s.chunk_size = SIZES[Rnd() % 5];
        g_s.have_dst = (Rnd() % 4) != 0; g_s.alloc_null = (Rnd() % 4) == 0;
        g_s.used0 = (Rnd() % 6 == 0) ? g_s.chunk_size : Rnd() % g_s.chunk_size;
        g_s.decompress_finish = (Rnd() & 1) != 0; g_s.finish_deactivates = (Rnd() % 4) != 0; g_s.have_src = (Rnd() % 4) != 0;
        g_s.dst_pos0 = Rnd(); g_s.src_pos0 = (Rnd() % 3 == 0) ? 0 : 0x40 * (Rnd() % 4);
        g_s.input_size = 0x40 * (1 + Rnd() % 5) + (Rnd() % 3) - 1;
        int steps = Rnd() % 6;
        for (int k = 0; k < steps; ++k)
        {
            UL room = g_s.chunk_size; UL ret = (Rnd() % 5 == 0) ? room : (Rnd() % 4 == 0) ? 0 : 1 + Rnd() % (room / 2 + 1);
            AddStep(ret, Rnd() % 5 == 0, Rnd() % 9 == 0, Rnd() % 5 == 0);
        }
        int reads = Rnd() % (MAXREADS + 1);
        for (int k = 0; k < reads; ++k) AddRead((Rnd() % 3 == 0) ? 0 : 0x40 * (long)(Rnd() % 4), 0x10 + 8 * (long)(Rnd() % 7), (Rnd() % 4) != 0, Rnd());
        Run();
    }
    if (g_fail) { printf("009c3c20_TEST FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    printf("009c3c20_TEST PASS (%d cases, %d hand-written)\n", g_cases, exact);
    return 0;
}