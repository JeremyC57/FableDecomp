#include <cstdio>
// Model of NUISystem::CIconText::AddIcon(graphic, charIndex, flags): build an embedded icon (offset 0,0, size 40x40 by default);
// a set graphic (> 0) takes the bank frame's width / height, starts loading it (BeginLoading(0, 0x10)) and fetches the frame
// (GetFrame(frame, 0, 0x11): forced load, do not wait for a pending finer level), releasing the fetched frame again; a graphic
// <= 0 gets a zero size. The icon is appended to Icons. The bank handle is a counted copy, released on exit.
struct FrameData { int refs; };
struct InfoFrame {
    unsigned short w, h; FrameData* data; int beginLoading; unsigned long lastFlags; int getFrame;
    void BeginLoading(unsigned long, unsigned long flags) { ++beginLoading; lastFlags = flags; }
    bool GetFrame(FrameData*& out, unsigned long, unsigned long flags)
    {
        ++getFrame; lastFlags = flags;
        if (!data) { if (flags & 4) return false; if (!(flags & 1)) return false; data = new FrameData; data->refs = 1; }
        out = data; ++data->refs; return true;
    }
};
struct Bank {
    int refs; InfoFrame* frames; unsigned long count;
    InfoFrame* PeekGraphicInfoFrame(unsigned long n) { return n < count ? &frames[n] : 0; }
};
struct Icon { float OffX, OffY, W, H; long Graphic, CharIndex, Flags; };
struct IconText {
    Icon icons[8]; int n; Bank* bank;
    void AddIcon(long graphic, long charIndex, long flags)
    {
        Icon icon; icon.OffX = 0.0f; icon.OffY = 0.0f; icon.W = 40.0f; icon.H = 40.0f;
        icon.Graphic = graphic; icon.CharIndex = charIndex; icon.Flags = flags;
        Bank* b = bank; ++b->refs;
        if (graphic > 0) {
            InfoFrame* frame = b->PeekGraphicInfoFrame((unsigned long)graphic);
            icon.W = (float)frame->w; icon.H = (float)frame->h;
            frame->BeginLoading(0, 0x10);
            FrameData* got = 0;
            frame->GetFrame(got, 0, 0x11);
            if (got) --got->refs;
        } else { icon.W = 0.0f; icon.H = 0.0f; }
        icons[n++] = icon;
        --b->refs;
    }
};
int main()
{
    int bad = 0;
    InfoFrame frames[3] = { { 1, 1, 0, 0, 0, 0 }, { 64, 32, 0, 0, 0, 0 }, { 128, 256, 0, 0, 0, 0 } };
    Bank bank = { 1, frames, 3 };
    IconText t; t.n = 0; t.bank = &bank;
    t.AddIcon(1, 5, 2);
    if (t.n != 1 || t.icons[0].W != 64.0f || t.icons[0].H != 32.0f || t.icons[0].Graphic != 1 || t.icons[0].CharIndex != 5 || t.icons[0].Flags != 2) bad = 1;
    if (t.icons[0].OffX != 0.0f || t.icons[0].OffY != 0.0f) bad = 1;
    if (frames[1].beginLoading != 1 || frames[1].getFrame != 1 || frames[1].lastFlags != 0x11 || !frames[1].data || frames[1].data->refs != 1) bad = 1;
    t.AddIcon(0, 9, 0);
    if (t.n != 2 || t.icons[1].W != 0.0f || t.icons[1].H != 0.0f || t.icons[1].CharIndex != 9) bad = 1;
    if (frames[0].beginLoading != 0 || frames[0].getFrame != 0) bad = 1;
    t.AddIcon(-3, 1, 1);
    if (t.n != 3 || t.icons[2].W != 0.0f || t.icons[2].H != 0.0f || t.icons[2].Graphic != -3) bad = 1;
    t.AddIcon(2, 7, 4);
    if (t.n != 4 || t.icons[3].W != 128.0f || t.icons[3].H != 256.0f || frames[2].beginLoading != 1 || frames[2].getFrame != 1) bad = 1;
    t.AddIcon(1, 8, 0);
    if (frames[1].beginLoading != 2 || frames[1].getFrame != 2 || frames[1].data->refs != 1) bad = 1;
    if (bank.refs != 1) bad = 1;
    delete frames[1].data; delete frames[2].data;
    if (bad) { std::printf("00542780_TEST FAIL\n"); return 1; }
    std::printf("00542780_TEST PASS\n");
    return 0;
}