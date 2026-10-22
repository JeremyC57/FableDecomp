// Behaviour test for 0x0041C260 CGraphicInfoFrame::GetFrame: no data -> fail on flag 4, else create + load (+ forced load on flag 1); loaded frame data -> LRU touch once per bank frame, hand out the frame when its mip level suffices, else load (twice with flag 0x10 and pending file data); data without a frame -> load.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct FrameData { int refs; };
struct Data {
    FrameData* Frame; int FileData; int Current; int Touched; unsigned long TouchedFrame; int texTouched, selfTouched;
    void BeginLoading(unsigned long level, unsigned long flags) { char b[48]; std::sprintf(b, "load%lu/%lu", level, flags); Log(b); }
};
static FrameData g_newFrame;
static bool g_forced;
struct InfoFrame {
    Data* D; unsigned long bankFrame; Data fresh;
    bool Forced(unsigned long flags) { Log("forced"); if (g_forced) D->Frame = &g_newFrame; return g_forced; }
    static void Assign(FrameData*& out, FrameData* f) { if (out) --out->refs; out = f; if (f) ++f->refs; }
    bool GetFrame(FrameData*& out, unsigned long level, unsigned long flags)
    {
        if (D) {
            if (D->Frame) {
                if (!(flags & 8)) {
                    if (D->TouchedFrame != bankFrame || level <= (unsigned long)D->Touched) {
                        D->TouchedFrame = bankFrame; ++D->texTouched;
                        if (level <= (unsigned long)D->Current) ++D->selfTouched;
                        if ((unsigned long)D->Touched > level) D->Touched = (int)level;
                    }
                }
                if (level < (unsigned long)D->Current) {
                    D->BeginLoading(level, flags);
                    if (!(flags & 0x10) || !D->FileData) { Assign(out, D->Frame); return true; }
                } else { Assign(out, D->Frame); return true; }
            }
            D->BeginLoading(level, flags);
        } else {
            if (flags & 4) return false;
            fresh.Frame = 0; fresh.FileData = 0; fresh.Current = 9; fresh.Touched = 9; fresh.TouchedFrame = 0; fresh.texTouched = fresh.selfTouched = 0;
            D = &fresh; Log("new");
            D->BeginLoading(level, flags);
        }
        if (!(flags & 1) || !Forced(flags)) return false;
        Assign(out, D->Frame);
        return true;
    }
};
static int Expect(bool got, bool want, const char* log)
{
    if (got != want || std::strcmp(g_log, log) != 0) { std::printf("got %d %s\nexp %d %s\n", got, g_log, want, log); return 1; }
    return 0;
}
static int RunAll(const char* tag, InfoFrame* frames, unsigned long index)
{
    int bad = 0; FrameData loaded = { 1 }; FrameData* out = 0;
    InfoFrame& f = frames[index]; f.D = 0; f.bankFrame = 5; g_forced = false;
    g_log[0] = 0; bad += Expect(f.GetFrame(out, 0, 4), false, "");
    g_log[0] = 0; bad += Expect(f.GetFrame(out, 2, 0), false, "new;load2/0;");
    g_forced = true; f.D = 0;
    g_log[0] = 0; bad += Expect(f.GetFrame(out, 2, 1), true, "new;load2/1;forced;");
    bad += (out != &g_newFrame || g_newFrame.refs != 1);
    Data d; d.Frame = &loaded; d.FileData = 0; d.Current = 1; d.Touched = 3; d.TouchedFrame = 4; d.texTouched = d.selfTouched = 0;
    f.D = &d;
    g_log[0] = 0; bad += Expect(f.GetFrame(out, 2, 0), true, "");
    bad += (out != &loaded || loaded.refs != 2 || g_newFrame.refs != 0 || d.texTouched != 1 || d.selfTouched != 0 || d.Touched != 2 || d.TouchedFrame != 5);
    g_log[0] = 0; bad += Expect(f.GetFrame(out, 0, 0x10), true, "load0/16;");
    bad += (d.texTouched != 2 || d.selfTouched != 1 || d.Touched != 0);
    d.FileData = 1; g_forced = false;
    g_log[0] = 0; bad += Expect(f.GetFrame(out, 0, 0x18), false, "load0/24;load0/24;");
    d.Frame = 0;
    g_log[0] = 0; bad += Expect(f.GetFrame(out, 0, 0), false, "load0/0;");
    if (bad) { std::printf("%s_TEST FAIL\n", tag); return 1; }
    std::printf("%s_TEST PASS\n", tag); return 0;
}
int main() { InfoFrame frame; return RunAll("0041c260", &frame, 0); }
