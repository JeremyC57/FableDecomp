// Behaviour test for CTexture::LoadFromDataStreamToPreallocatedSurface (0x009FB750): a standalone model of the row's logic with the
// memory traffic RECORDED instead of performed (so 4 GB boundary sizes can be exercised). The 19-byte CTextureFileInfo read first; per
// level c1 < level count: Lock(c1, 0) -- the same-TU body: rect {0, 0, X, Y} of GetDimensions(c1), WaitWhileResourceIsBusy(texture, 1.0f),
// LockRect; only `hr >= 0` writes the hidden result (retail 0x009FA4BB `jl` skips the four stores), so after a failed lock the row's
// locked_rect keeps its previous contents (indeterminate in C++; retail passes the same frame slot as the hidden result every iteration,
// 0x009FB803 `lea eax,[esp+34h]`, and nothing else writes it -- REVIEW: this case encodes retail's slot reuse); a compressed format reads GetLevelByteLength(c1) = colour depth of level 0's format * level
// width * level height / 8 straight into the locked memory; otherwise overflow_check = byte depth * (signed) width, abandon the whole load
// if > 0xFFFFFFFF, line_length = byte depth * width, overflow_check2 = (signed) height * line_length, abandon if > 0xFFFFFFFF, read
// height * line_length bytes into a buffer, copy `height` rows of line_length bytes from the buffer to the locked memory advancing dst by
// the pitch and src by line_length; Unlock(c1).
// Cases: every branch, both overflow tests just above AND exactly on 0xFFFFFFFF (each abandon with a later level that must not run),
// signed width / height, level count bounds, pitch != line_length, width != height, per-level compressed sizes, the locked memory
// address, the lock rect / flags / wait, LockRect results 0 (S_OK), 1 (positive success) and negative (failure).
// (Audit e13, 2026-11-18: file-info read, Lock body and the second-overflow `return` added to the author's model.)
#include <cstdio>

typedef unsigned long long u64; typedef long long i64;
struct Level { long w, h, pitch; unsigned long descW, descH; long hr; unsigned long pbits; };
struct Tex { int levels; Level lv[4]; unsigned long level0Depth; };
// H=file info read(0,len) L=lock(level,flags; rect right/bottom, wait) R=read(target kind,len,address) C=copy(dst,srcoff,len) U=unlock(level)
struct Ev { char kind; long a; unsigned long b; unsigned long c; long rr, rb; float wait; };
static Ev g_ev[64]; static int g_n;
static Ev& Rec(char k, long a, unsigned long b, unsigned long c) { Ev& e = g_ev[g_n++]; e.kind = k; e.a = a; e.b = b; e.c = c; e.rr = 0; e.rb = 0; e.wait = 0.0f; return e; }

struct Locked { long w, h, pitch; unsigned long pmem; };
// CTexture::Lock(level, lock_flags): writes `result` (the hidden return slot) only when LockRect succeeds
static void Lock(const Level& lr, long level, unsigned long lock_flags, Locked& result)
{
    long dimX = lr.w, dimY = lr.h;                                   // GetDimensions(level)
    Ev& e = Rec('L', level, lock_flags, 0);
    e.rr = dimX; e.rb = dimY;                                        // rect.right = X, rect.bottom = Y (top = left = 0)
    e.wait = 1.0f;                                                   // WaitWhileResourceIsBusy(PD3DTexture, 1.0f)
    long d3dPitch = -7; unsigned long d3dBits = 0xDEAD0000ul;        // Lock's own _D3DLOCKED_RECT: LockRect fills it only on success
    if (lr.hr >= 0) { d3dPitch = lr.pitch; d3dBits = lr.pbits; }
    long hr = lr.hr;
    if (hr >= 0) { result.w = dimX; result.h = dimY; result.pitch = d3dPitch; result.pmem = d3dBits; }
}

static void Load(const Tex& t, bool compressed, unsigned long byteDepth)
{
    Rec('H', 0, 19 /* sizeof(CTextureFileInfo) */, 0);
    Locked locked_rect = { 0, 0, 0, 0 };
    for (long c1 = 0; c1 < t.levels; ++c1) {
        const Level& lr = t.lv[c1];
        Lock(lr, c1, 0, locked_rect);
        if (compressed) {
            unsigned long len = t.level0Depth * lr.descW * lr.descH / 8;
            Rec('R', 1 /* locked memory */, len, locked_rect.pmem);
        } else {
            u64 overflow_check = (u64)byteDepth * (i64)locked_rect.w;
            if (overflow_check > 0xffffffffu) return;
            unsigned long line_length = byteDepth * locked_rect.w;
            u64 overflow_check2 = (i64)locked_rect.h * (u64)line_length;
            if (overflow_check2 > 0xffffffffu) return;
            unsigned long data_length = locked_rect.h * line_length;
            Rec('R', 2 /* buffer */, data_length, 0);
            unsigned long dst = locked_rect.pmem, src = 0;
            for (long cy = 0; cy < locked_rect.h; ++cy) { Rec('C', (long)dst, src, line_length); dst += locked_rect.pitch; src += line_length; }
        }
        Rec('U', c1, 0, 0);
    }
}

static int fails = 0;
static void Check(bool ok, int id) { if (!ok) { std::printf("case %d failed\n", id); ++fails; } }
// level events are numbered from 0 after the file-info read (event 0); H() checks that read, N() counts the level events
static bool E(int i, char k, long a, unsigned long b, unsigned long c) { ++i; return i < g_n && g_ev[i].kind == k && g_ev[i].a == a && g_ev[i].b == b && g_ev[i].c == c; }
static bool EL(int i, long level, long right, long bottom) { return E(i, 'L', level, 0, 0) && g_ev[i + 1].rr == right && g_ev[i + 1].rb == bottom && g_ev[i + 1].wait == 1.0f; }
static bool H() { return g_n >= 1 && g_ev[0].kind == 'H' && g_ev[0].a == 0 && g_ev[0].b == 19 && g_ev[0].c == 0; }
static int N() { return g_n - 1; }

int main()
{
    // 1: two uncompressed levels, byte depth 2: level 0 4x3 pitch 32 (line 8, data 24), level 1 2x1 pitch 16 (line 4, data 4)
    Tex t = { 2, { { 4, 3, 32, 0, 0 }, { 2, 1, 16, 0, 0 } }, 0 };
    g_n = 0; Load(t, false, 2);
    Check(H() && N() == 10, 1);
    Check(EL(0, 0, 4, 3) && E(1, 'R', 2, 24, 0) && E(2, 'C', 0, 0, 8) && E(3, 'C', 32, 8, 8) && E(4, 'C', 64, 16, 8) && E(5, 'U', 0, 0, 0), 2);
    Check(EL(6, 1, 2, 1) && E(7, 'R', 2, 4, 0) && E(8, 'C', 0, 0, 4) && E(9, 'U', 1, 0, 0), 3);
    // 2: compressed: level 0 desc 8x4, level 1 desc 4x2, level-0 format depth 4 bits -> 16 and 4 bytes into the locked memory
    Tex tc = { 2, { { 0, 0, 0, 8, 4 }, { 0, 0, 0, 4, 2 } }, 4 };
    g_n = 0; Load(tc, true, 1);
    Check(H() && N() == 6 && E(1, 'R', 1, 16, 0) && E(2, 'U', 0, 0, 0) && E(4, 'R', 1, 4, 0) && E(5, 'U', 1, 0, 0), 4);
    // 3: no levels: only the file info
    Tex t0 = { 0 }; g_n = 0; Load(t0, false, 2); Check(H() && N() == 0, 5);
    // 4: first overflow just above (3 * 0x55555556 = 0x100000002): abandon at the lock, no unlock, no later level
    Tex o1 = { 2, { { 0x55555556L, 1, 0, 0, 0 }, { 1, 1, 1, 0, 0 } }, 0 };
    g_n = 0; Load(o1, false, 3); Check(H() && N() == 1 && E(0, 'L', 0, 0, 0), 6);
    // 5: first overflow exactly 0xFFFFFFFF (3 * 0x55555555), height 0: continues (empty read, no rows), unlocks, next level runs
    Tex o2 = { 2, { { 0x55555555L, 0, 0, 0, 0 }, { 1, 1, 7, 0, 0 } }, 0 };
    g_n = 0; Load(o2, false, 3); Check(H() && N() == 7 && E(1, 'R', 2, 0, 0) && E(2, 'U', 0, 0, 0) && E(5, 'C', 0, 0, 3) && E(6, 'U', 1, 0, 0), 7);
    // 6: width is signed: -1 with byte depth 1 -> 0xFFFFFFFFFFFFFFFF > limit: abandon (an unsigned width would continue)
    Tex o3 = { 1, { { -1, 0, 0, 0, 0 } }, 0 };
    g_n = 0; Load(o3, false, 1); Check(H() && N() == 1, 8);
    // 7: second overflow just above (0x7FFFFFFF rows of 3 bytes): abandon after the lock -- no unlock and the second level never runs
    Tex o4 = { 2, { { 1, 0x7FFFFFFFL, 0, 0, 0 }, { 1, 1, 1, 0, 0 } }, 0 };
    g_n = 0; Load(o4, false, 3); Check(H() && N() == 1 && E(0, 'L', 0, 0, 0), 9);
    // 8: height is signed: -1 rows of 1 byte -> 0xFFFFFFFFFFFFFFFF -> abandon (an unsigned height gives exactly 0xFFFFFFFF and continues)
    Tex o5 = { 1, { { 1, -1, 0, 0, 0 } }, 0 };
    g_n = 0; Load(o5, false, 1); Check(H() && N() == 1, 10);
    // 9: second overflow exactly 0xFFFFFFFF: line 0xFFFFFFFF (3 * 0x55555555), one row: continues, one copy of the whole line
    Tex o6 = { 1, { { 0x55555555L, 1, 5, 0, 0 } }, 0 };
    g_n = 0; Load(o6, false, 3); Check(H() && N() == 4 && E(1, 'R', 2, 0xFFFFFFFFul, 0) && E(2, 'C', 0, 0, 0xFFFFFFFFul) && E(3, 'U', 0, 0, 0), 11);
    // 10: LockRect returns 1 (a positive success code): the lock is used; rows go to the locked memory 0x1000 at pitch 8
    Tex p1 = { 1, { { 2, 2, 8, 0, 0, 1, 0x1000ul } }, 0 };
    g_n = 0; Load(p1, false, 1);
    Check(H() && N() == 5 && EL(0, 0, 2, 2) && E(1, 'R', 2, 4, 0) && E(2, 'C', 0x1000, 0, 2) && E(3, 'C', 0x1008, 2, 2) && E(4, 'U', 0, 0, 0), 12);
    // 11: level 1's LockRect fails: Lock writes nothing, so level 1 runs on level 0's locked rect (4x3, pitch 32, memory 0x2000)
    Tex f1 = { 2, { { 4, 3, 32, 0, 0, 0, 0x2000ul }, { 2, 1, 16, 0, 0, -1, 0x3000ul } }, 0 };
    g_n = 0; Load(f1, false, 2);
    Check(H() && N() == 12 && EL(6, 1, 2, 1) && E(7, 'R', 2, 24, 0) && E(8, 'C', 0x2000, 0, 8) && E(9, 'C', 0x2020, 8, 8) &&
          E(10, 'C', 0x2040, 16, 8) && E(11, 'U', 1, 0, 0), 13);
    // 12: compressed data goes to the locked memory address of its level (0x4000, then 0x5000)
    Tex c2 = { 2, { { 0, 0, 0, 8, 4, 0, 0x4000ul }, { 0, 0, 0, 4, 2, 0, 0x5000ul } }, 4 };
    g_n = 0; Load(c2, true, 1); Check(H() && N() == 6 && E(1, 'R', 1, 16, 0x4000) && E(4, 'R', 1, 4, 0x5000), 14);
    if (fails) { std::printf("009fb750_TEST FAIL\n"); return 1; }
    std::printf("009fb750_TEST PASS\n");
    return 0;
}
