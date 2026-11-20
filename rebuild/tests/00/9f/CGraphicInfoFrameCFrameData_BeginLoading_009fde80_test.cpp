// Behaviour test for CGraphicInfoFrame::CFrameData::BeginLoading (0x009FDE80): the requested mip level is clamped to the graphic's
// mip count - 1 and to the frame's small-mip level; a valid current texture at least that fine only touches the frame (TouchFrame:
// re-touch when the touched frame differs or the level is at or below the touched level; touch the resource when the level is at or
// below the current level; lower the touched level) and returns; otherwise the finest allowed level is limited by current - 1 and by
// a loading level - 1 (a loading request at least as fine returns); then both textures are locked, a volume texture is allocated in
// one go (A8R8G8B8) or flat ones level by level (halving width / height) up to the limit; both are unlocked; on success an in-flight
// read is aborted (if the abort fails: return unless get_flags has 0x10, else wait) and released, the new texture becomes the loading
// one, the level is stored and an async read of (offset, size) is queued: size = entry size (divided by the frame count for a
// non-flat multi-frame graphic of entry type 1, offset = frame * size), minus the bytes of each skipped level (level 0 uses its
// compressed size when non-zero), offset plus the same; then SetResourceSize(Sizeof()), RemoveFromResourceList, AddToLoadingQueue.
#include <cstdio>
#include <cstring>
struct Header { unsigned short Width, Height, Depth, FrameCount; unsigned char MipmapLevels; bool Flat; unsigned long FrameDataSize;
                unsigned long Compressed[4]; };
struct Log { char s[512]; void add(const char* t) { std::strcat(s, t); std::strcat(s, ";"); } };
static Log L;
static char buf[64];
struct Data { long TouchedFrame; signed char TouchedMip; signed char CurMip; int resTouch, texTouch; };
struct Frame {
    Header* header; signed char SmallMip; bool volume; long current_frame; Data* data; unsigned long FrameNum; unsigned long entry_size;
    int entry_type;
    void TouchFrame(unsigned long m) {
        if (data) {
            if (data->TouchedFrame != current_frame || m <= (unsigned long)data->TouchedMip) {
                data->TouchedFrame = current_frame; data->texTouch++;
                if (m <= (unsigned long)data->CurMip) data->resTouch++;
                if ((unsigned long)data->TouchedMip > m) data->TouchedMip = (signed char)m;
            }
        }
    }
};
struct FD {
    Frame* f; bool valid; signed char CurrentMipmapLevel; bool loading; signed char LoadingMipmapLevel;
    int alloc_ok_from;           // flat: first level that succeeds (-1 never); volume: >= 0 succeeds
    bool abort_ok; int loaded_tex;
    void BeginLoading(unsigned long mipmap_level, unsigned long get_flags) {
        const Header& header = *f->header;
        if (mipmap_level > (unsigned long)(header.MipmapLevels - 1)) mipmap_level = header.MipmapLevels - 1;
        long max_level = f->SmallMip;
        if (mipmap_level > (unsigned long)max_level) mipmap_level = max_level;
        if (valid) {
            if ((unsigned long)CurrentMipmapLevel <= mipmap_level) { f->TouchFrame(mipmap_level); L.add("touch"); return; }
            if (max_level > CurrentMipmapLevel - 1) max_level = CurrentMipmapLevel - 1;
        }
        if (loading) {
            if (mipmap_level >= (unsigned long)LoadingMipmapLevel) { L.add("busy"); return; }
            if (max_level > LoadingMipmapLevel - 1) max_level = LoadingMipmapLevel - 1;
        }
        L.add("lock"); bool ok = false;
        long w = header.Width >> mipmap_level, h = header.Height >> mipmap_level, d = header.Depth >> mipmap_level;
        if (f->volume) {
            std::sprintf(buf, "vol%ld,%ld,%ld,%d", w, h, d, (int)(header.MipmapLevels - mipmap_level)); L.add(buf);
            ok = alloc_ok_from >= 0;
        } else {
            for (; (long)mipmap_level <= max_level; ++mipmap_level, w /= 2, h /= 2) {
                std::sprintf(buf, "tex%ld,%ld,%d", w, h, (int)(header.MipmapLevels - mipmap_level)); L.add(buf);
                if (alloc_ok_from >= 0 && (long)mipmap_level >= alloc_ok_from) { ok = true; break; }
            }
        }
        L.add("unlock");
        if (!ok) return;
        if (loading) {
            if (!abort_ok) { if (!(get_flags & 0x10)) { L.add("keep"); return; } L.add("wait"); }
            L.add("release"); loading = false;
        }
        LoadingMipmapLevel = (signed char)mipmap_level; loading = true; loaded_tex++;
        unsigned long total = f->entry_size, off = 0;
        if (header.FrameCount > 1 && !header.Flat && f->entry_type == 1) { total /= header.FrameCount; off = f->FrameNum * total; }
        unsigned long mb = header.FrameDataSize; long size = total;
        for (unsigned long c = 0; c < mipmap_level; ++c) {
            unsigned long cur = mb;
            if (c < 1 && header.Compressed[c] > 0) cur = header.Compressed[c];
            size -= cur; off += cur; mb >>= 2;
        }
        std::sprintf(buf, "read%lu,%ld", off, size); L.add(buf); L.add("size;unlist;queue");
    }
};
static int fail(int n) { std::printf("009fde80_TEST FAIL %d: %s\n", n, L.s); return 1; }
static bool is(const char* e) { return std::strcmp(L.s, e) == 0; }
int main()
{
    Header H = { 64, 32, 16, 1, 4, false, 4096, { 0, 0, 0, 0 } };
    Data D = { 7, 3, 2, 0, 0 };
    Frame F = { &H, 3, false, 7, &D, 2, 10000, 1 };
    FD fd = { &F, false, 0, false, 0, 0, true, 0 };
    // 1: level 9 clamps to 3 (mips - 1 = 3, small 3): allocate 8x4 at level 3; read skips 4096+1024+256.
    L.s[0] = 0; fd.BeginLoading(9, 0);
    if (!is("lock;tex8,4,1;unlock;read5376,4624;size;unlist;queue;") || fd.LoadingMipmapLevel != 3) return fail(1);
    // 2: small-mip clamp below mips - 1: small 1 -> level 1.
    F.SmallMip = 1; fd.loading = false; L.s[0] = 0; fd.BeginLoading(3, 0);
    if (!is("lock;tex32,16,3;unlock;read4096,5904;size;unlist;queue;")) return fail(2);
    // 3: compressed level-0 size used (non-zero), and only for level 0; flat graphics level 0 exact boundary (no skip).
    F.SmallMip = 3; H.Compressed[0] = 100; H.Compressed[1] = 7; fd.loading = false; L.s[0] = 0; fd.BeginLoading(2, 0);
    if (!is("lock;tex16,8,2;unlock;read1124,8876;size;unlist;queue;")) return fail(3);
    fd.loading = false; L.s[0] = 0; fd.BeginLoading(0, 0);
    if (!is("lock;tex64,32,4;unlock;read0,10000;size;unlist;queue;")) return fail(4);
    H.Compressed[0] = 0; H.Compressed[1] = 0;
    // 5: allocation fails at levels 1..2, succeeds at 3 (halving); 6: never succeeds -> nothing after unlock.
    fd.loading = false; fd.alloc_ok_from = 3; L.s[0] = 0; fd.BeginLoading(1, 0);
    if (!is("lock;tex32,16,3;tex16,8,2;tex8,4,1;unlock;read5376,4624;size;unlist;queue;")) return fail(5);
    fd.loading = false; fd.alloc_ok_from = -1; L.s[0] = 0; fd.BeginLoading(1, 0);
    if (!is("lock;tex32,16,3;tex16,8,2;tex8,4,1;unlock;") || fd.loading) return fail(6);
    fd.alloc_ok_from = 0;
    // 7: valid texture at level 2: request 2 (equal) touches; touched frame differs -> retouch, level <= current -> resource touch.
    fd.valid = true; fd.CurrentMipmapLevel = 2; D.TouchedFrame = 6; D.TouchedMip = 3; L.s[0] = 0; fd.BeginLoading(2, 0);
    if (!is("touch;") || D.TouchedFrame != 7 || D.texTouch != 1 || D.resTouch != 1 || D.TouchedMip != 2) return fail(7);
    // 8: same frame, level 3 > touched 2 -> no retouch; 9: same frame, level == touched -> retouch, 3 > current 2 -> no resource touch.
    L.s[0] = 0; fd.BeginLoading(3, 0);
    if (!is("touch;") || D.texTouch != 1) return fail(8);
    D.TouchedMip = 3; L.s[0] = 0; fd.BeginLoading(3, 0);
    if (D.texTouch != 2 || D.resTouch != 1 || D.TouchedMip != 3) return fail(9);
    // 10: valid at level 2, request 0: limit current - 1 = 1 -> tries 0, 1 only (fails both).
    fd.loading = false; fd.alloc_ok_from = 5; L.s[0] = 0; fd.BeginLoading(0, 0);
    if (!is("lock;tex64,32,4;tex32,16,3;unlock;")) return fail(10);
    fd.valid = false; fd.alloc_ok_from = 0;
    // 11: loading at level 2: request 2 (equal) is busy; request 0 with limit 1 and abort OK: release then read.
    fd.loading = true; fd.LoadingMipmapLevel = 2; L.s[0] = 0; fd.BeginLoading(2, 0);
    if (!is("busy;")) return fail(11);
    fd.alloc_ok_from = 5; L.s[0] = 0; fd.BeginLoading(0, 0);
    if (!is("lock;tex64,32,4;tex32,16,3;unlock;")) return fail(12);
    fd.alloc_ok_from = 1; L.s[0] = 0; fd.BeginLoading(0, 0);
    if (!is("lock;tex64,32,4;tex32,16,3;unlock;release;read4096,5904;size;unlist;queue;")) return fail(13);
    // 14: abort fails without 0x10 (0x0F, 0x20): keep loading; with 0x10: wait and release.
    fd.LoadingMipmapLevel = 3; fd.abort_ok = false; fd.alloc_ok_from = 0; L.s[0] = 0; fd.BeginLoading(0, 0x2F);
    if (!is("lock;tex64,32,4;unlock;keep;") || fd.LoadingMipmapLevel != 3) return fail(14);
    L.s[0] = 0; fd.BeginLoading(0, 0x10);
    if (!is("lock;tex64,32,4;unlock;wait;release;read0,10000;size;unlist;queue;") || fd.LoadingMipmapLevel != 0) return fail(15);
    fd.loading = false; fd.abort_ok = true;
    // 16: multi-frame non-flat entry type 1: size / frames, offset = frame * size (unsigned division).
    H.FrameCount = 4; L.s[0] = 0; fd.BeginLoading(1, 0);
    if (!is("lock;tex32,16,3;unlock;read9096,-1596;size;unlist;queue;")) return fail(16);
    H.Flat = true; fd.loading = false; L.s[0] = 0; fd.BeginLoading(1, 0);                     // flat: whole entry
    if (!is("lock;tex32,16,3;unlock;read4096,5904;size;unlist;queue;")) return fail(17);
    H.Flat = false; F.entry_type = 2; fd.loading = false; L.s[0] = 0; fd.BeginLoading(1, 0); // entry type 2: whole entry
    if (!is("lock;tex32,16,3;unlock;read4096,5904;size;unlist;queue;")) return fail(18);
    F.entry_type = 1; H.FrameCount = 1; fd.loading = false; L.s[0] = 0; fd.BeginLoading(1, 0); // one frame: whole entry
    if (!is("lock;tex32,16,3;unlock;read4096,5904;size;unlist;queue;")) return fail(19);
    // 20: volume texture: one allocation with depth, mips - level.
    F.volume = true; fd.loading = false; L.s[0] = 0; fd.BeginLoading(1, 0);
    if (!is("lock;vol32,16,8,3;unlock;read4096,5904;size;unlist;queue;")) return fail(20);
    // 21 (audit): the mip-count clamp alone (small-mip level 5 above mips - 1 = 3): 9 -> 3, not 4.
    F.volume = false; F.SmallMip = 5; fd.loading = false; L.s[0] = 0; fd.BeginLoading(9, 0);
    if (!is("lock;tex8,4,1;unlock;read5376,4624;size;unlist;queue;") || fd.LoadingMipmapLevel != 3) return fail(21);
    std::printf("009fde80_TEST PASS\n");
    return 0;
}