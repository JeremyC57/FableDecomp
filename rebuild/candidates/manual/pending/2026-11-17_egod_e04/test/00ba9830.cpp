// Behaviour test for CEnginePrimitiveManagerMeshBase::GetReflectionTexture (0x00BA9830) with the inline per-frame GetTexture body it
// expands: a material with no reflection map (index 0) gets the bank's blank texture; an index past the graphic list gives no frame
// (not dereferenced here: a valid index is used for every other case); the flags are 1 when the renderer blocks on loads or
// block_on_load is set; the frame body touches the LRU only when not told to skip (flag 8) and the frame or a finer mipmap changed,
// re-touches the frame data when the wanted mipmap is resident, lowers the touched mipmap, begins loading a finer mipmap and returns
// the resident texture unless flag 0x10 waits on pending file data, creates the frame data on first use (unless flag 4: blank), and
// forces the load only with flag 1; every failure returns the frame's blank texture.
#include <cstdio>
struct Tex { int id; };
static Tex g_bankBlank = { 1 }, g_frameBlank = { 2 }, g_tex = { 3 };
struct Res { bool inList; unsigned long lastUsed; int listMoves; };
static unsigned long g_listFrame = 7;
static void TouchRes(Res& r) { if (r.inList && r.lastUsed != g_listFrame) { ++r.listMoves; r.lastUsed = g_listFrame; } }
struct Data {
    bool hasFrame; Res self; bool hasManaged; Res managed; void* fileData; signed char cur, touchedMip; unsigned long touchedFrame;
    int beginLoads; unsigned long lastLoadMip, lastLoadFlags;
    void BeginLoading(unsigned long m, unsigned long f) { ++beginLoads; lastLoadMip = m; lastLoadFlags = f; }
};
struct Frame {
    Data* data; Data fresh; bool forcedOk; int forcedCalls; unsigned long bankFrame;
    bool GetTexture(unsigned long mip, unsigned long flags, const Tex*& texture)
    {
        if (data) {
            if (data->hasFrame) {
                if (!(flags & 8)) {
                    unsigned long frame = bankFrame;
                    if (data->touchedFrame != frame || mip <= (unsigned long)data->touchedMip) {
                        data->touchedFrame = frame;
                        if (data->hasManaged) TouchRes(data->managed);
                        if (mip <= (unsigned long)data->cur) TouchRes(data->self);
                        if ((unsigned long)data->touchedMip > mip) data->touchedMip = (signed char)mip;
                    }
                }
                if (mip < (unsigned long)data->cur) {
                    data->BeginLoading(mip, flags);
                    if (!(flags & 0x10) || !data->fileData) { texture = &g_tex; return true; }
                } else { texture = &g_tex; return true; }
            }
            data->BeginLoading(mip, flags);
        } else {
            if (flags & 4) return false;
            data = &fresh;
            data->BeginLoading(mip, flags);
        }
        ++forcedCalls;
        if (!(flags & 1) || !forcedOk) return false;
        texture = &g_tex;
        return true;
    }
};
static bool g_blockOnLoad = false;
static const Tex* GetReflectionTexture(long reflectionIndex, Frame* frames, unsigned long count, unsigned long mip, bool block_on_load, unsigned long* flagsSeen)
{
    long graphic_index = reflectionIndex;
    if (graphic_index == 0) return &g_bankBlank;
    Frame* frame_info = (unsigned long)graphic_index < count ? &frames[graphic_index] : 0;
    unsigned long get_flags = 0;
    if (g_blockOnLoad || block_on_load) get_flags = 1;
    *flagsSeen = get_flags;
    const Tex* texture;
    if (frame_info->GetTexture(mip, get_flags, texture)) return texture;
    return &g_frameBlank;
}
static int fail(int n) { std::printf("00ba9830_TEST FAIL %d\n", n); return 1; }
static Data MakeData() { Data d = { true, { true, 0, 0 }, true, { true, 0, 0 }, 0, 2, 5, 0, 0, 0, 0 }; return d; }
int main()
{
    unsigned long f = 99;
    Frame frames[3] = {};
    if (GetReflectionTexture(0, frames, 3, 0, false, &f) != &g_bankBlank || f != 99) return fail(1);    // no map: bank blank
    // resident (mip 3 >= cur 2), new frame -> touch managed + no self touch (3 > 2), touched mip lowered 5 -> 3
    Data d = MakeData(); frames[1].data = &d; frames[1].bankFrame = 4;
    if (GetReflectionTexture(1, frames, 3, 3, false, &f) != &g_tex || f != 0) return fail(2);
    if (d.touchedFrame != 4 || d.managed.listMoves != 1 || d.self.listMoves != 0 || d.touchedMip != 3 || d.beginLoads != 0) return fail(3);
    // same frame, coarser mip (4 > touched 3): no touch at all
    g_listFrame = 8;
    if (GetReflectionTexture(1, frames, 3, 4, false, &f) != &g_tex || d.managed.listMoves != 1 || d.touchedMip != 3) return fail(4);
    // same frame, mip 2 <= touched 3 and <= cur 2: both touched, touched mip -> 2
    if (GetReflectionTexture(1, frames, 3, 2, false, &f) != &g_tex || d.managed.listMoves != 2 || d.self.listMoves != 1 || d.touchedMip != 2) return fail(5);
    // finer mip 1 < cur 2: begin loading, texture still returned (no 0x10 flag)
    if (GetReflectionTexture(1, frames, 3, 1, false, &f) != &g_tex || d.beginLoads != 1 || d.lastLoadMip != 1) return fail(6);
    // block_on_load: flags 1; finer mip; texture returned (flag 0x10 not set)
    if (GetReflectionTexture(1, frames, 3, 0, true, &f) != &g_tex || f != 1 || d.beginLoads != 2 || d.lastLoadFlags != 1) return fail(7);
    g_blockOnLoad = true;
    if (GetReflectionTexture(1, frames, 3, 0, false, &f) != &g_tex || f != 1) return fail(8);
    g_blockOnLoad = false;
    // frame data without a frame: begin loading then forced load only with flag 1
    Data e = MakeData(); e.hasFrame = false; frames[2].data = &e; frames[2].forcedOk = true;
    if (GetReflectionTexture(2, frames, 3, 0, false, &f) != &g_frameBlank || e.beginLoads != 1 || frames[2].forcedCalls != 1) return fail(9);
    if (GetReflectionTexture(2, frames, 3, 0, true, &f) != &g_tex || frames[2].forcedCalls != 2) return fail(10);
    frames[2].forcedOk = false;
    if (GetReflectionTexture(2, frames, 3, 0, true, &f) != &g_frameBlank) return fail(11);
    // no frame data yet: created, loading begun, forced with flag 1
    frames[0].data = 0; frames[0].forcedOk = true; frames[0].fresh = MakeData();
    Frame one[2] = {}; one[1].forcedOk = true; one[1].fresh = MakeData();
    if (GetReflectionTexture(1, one, 2, 0, true, &f) != &g_tex || one[1].data != &one[1].fresh || one[1].fresh.beginLoads != 1) return fail(12);
    Frame two[2] = {}; two[1].fresh = MakeData();
    if (GetReflectionTexture(1, two, 2, 0, false, &f) != &g_frameBlank || two[1].data == 0) return fail(13);
    // flag 8 skips the LRU touch (only the inline sees it: call the frame body directly)
    Data g = MakeData(); Frame h = {}; h.data = &g; h.bankFrame = 9; const Tex* t = 0;
    if (!h.GetTexture(3, 8, t) || g.touchedFrame != 0 || g.managed.listMoves != 0) return fail(14);
    // flag 0x10 with pending file data on a finer mip: waits (forced path), fails without flag 1
    g.fileData = &g; t = 0;
    if (h.GetTexture(1, 0x10, t) || h.forcedCalls != 1) return fail(15);
    g.fileData = 0;
    if (!h.GetTexture(1, 0x10, t)) return fail(16);
    // flag 4 with no frame data: fail without creating it
    Frame k = {};
    if (k.GetTexture(0, 4, t) || k.data != 0) return fail(17);
    // a resource not in a list, or already touched this frame, does not move
    Res r = { false, 0, 0 }; TouchRes(r); if (r.listMoves) return fail(18);
    Res s = { true, g_listFrame, 0 }; TouchRes(s); if (s.listMoves) return fail(19);
    std::printf("00ba9830_TEST PASS\n");
    return 0;
}
