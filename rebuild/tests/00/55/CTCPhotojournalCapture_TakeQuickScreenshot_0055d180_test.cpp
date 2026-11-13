// Behaviour test for 0x0055D180 CTCPhotojournalCapture::TakeQuickScreenshot: the same control flow and string building through a
// copy of the model, with the engine callees replaced by recording fakes.
#include <cstdio>
#include <string>
typedef std::wstring Str;
static bool g_showing, g_room;
static int g_hidden, g_shown, g_saved, g_created, g_closed, g_played;
static Str g_savedPath, g_createdPath;
static long g_roomSize;
static void Replace(Str& s, const Str& search, const Str& with)
{
    for (Str::size_type p = s.find(search); p != Str::npos; p = s.find(search, p + with.size()))
        s.replace(p, search.size(), with);
}
static Str GetPhotojournalDir() { return L"Photos\\"; }
static Str GetValidProfileName(const Str& n) { return n; }
static bool IsRoomForFile(long size, const Str&) { g_roomSize = size; return g_room; }
struct Capture {
    long TakeScreenshotAfterFrames;
    void TakeQuickScreenshot(const Str& dateText, const Str& timeText, const Str& profile)
    {
        if (g_showing) { ++g_hidden; TakeScreenshotAfterFrames = 4; return; }
        Str name(dateText);
        Str clock(timeText);
        name = name + L" " + clock;
        Replace(name, L"\\", L"-");
        Replace(name, L"/", L"-");
        Replace(name, L":", L".");
        Str path = GetPhotojournalDir() + GetValidProfileName(profile) + L"\\" + name + L".jpg";
        if (!IsRoomForFile(10000000, path))
            return;
        ++g_created; g_createdPath = path; ++g_closed;
        ++g_saved; g_savedPath = path;
        ++g_shown;
        ++g_played;
    }
};
int main()
{
    Capture c; c.TakeScreenshotAfterFrames = 0;
    g_showing = true;
    c.TakeQuickScreenshot(L"12/11/2026", L"10:20:30", L"Hero");
    if (g_hidden != 1 || c.TakeScreenshotAfterFrames != 4 || g_saved) { std::printf("0055d180_TEST FAIL\n"); return 1; }
    g_showing = false; g_room = false; c.TakeScreenshotAfterFrames = 0;
    c.TakeQuickScreenshot(L"12/11/2026", L"10:20:30", L"Hero");
    if (g_saved || g_roomSize != 10000000 || c.TakeScreenshotAfterFrames != 0) { std::printf("0055d180_TEST FAIL\n"); return 1; }
    g_room = true;
    c.TakeQuickScreenshot(L"12/11\\2026", L"10:20:30", L"Hero");
    if (g_saved != 1 || g_created != 1 || g_closed != 1 || g_shown != 1 || g_played != 1 ||
        g_savedPath != Str(L"Photos\\Hero\\12-11-2026 10.20.30.jpg") || g_createdPath != g_savedPath) {
        std::printf("0055d180_TEST FAIL\n"); return 1;
    }
    std::printf("0055d180_TEST PASS\n");
    return 0;
}