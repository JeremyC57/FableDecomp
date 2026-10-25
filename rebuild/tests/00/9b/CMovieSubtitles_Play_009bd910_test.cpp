// Behaviour test for 0x009BD910 CMovieSubtitles::Play(float, bool): window layout, start (format + sound with the "SND_"
// fallback), end (pop_front), centred draw only while showing and asked to, frame counter.
#include <cstdio>
#include <cstring>

struct Box { float TLX, TLY, BRX, BRY; };
struct Entry { const char* text; const char* bank; const char* sound; };
struct SubTitle { const Entry* entry; float StartTime, EndTime; };

static int g_width = 1000, g_height = 800;
static Box g_vbox;
static const char* g_formatted = "";
static int g_draws = 0, g_plays = 0;
static long g_lastSample = -1;
static const char* g_lastBank = 0;

static void ABoxToVBox(const Box& a, Box& v) { v = a; }                   // identity stand-in
static const char* FindBank(const char* name) { return std::strcmp(name, "none") ? name : 0; }
static long IndexFromSymbol(const char* s)
{
    if (std::strcmp(s, "HELLO") == 0) return 3;
    if (std::strcmp(s, "SND_BYE") == 0) return 9;
    return 0;
}

struct Subtitles {
    SubTitle list[4];
    int first, count;
    bool IsSubtitleTime;
    long FrameCounter;
    Box VSubtitleWindow;
    void Play(float time, bool display)
    {
        float width = (float)g_width;
        float margin = width * 0.1f;
        Box screenBox = { margin, (float)(g_height - 300), width - margin, (float)(g_height - 30) };
        ABoxToVBox(screenBox, VSubtitleWindow);
        if (count != 0) {
            SubTitle& subtitle = list[first];
            if (!IsSubtitleTime) {
                if (time >= subtitle.StartTime) {
                    IsSubtitleTime = true;
                    g_formatted = subtitle.entry->text;
                    const char* bank = FindBank(subtitle.entry->bank);
                    long sample = 0;
                    if (bank) {
                        const char* soundName = subtitle.entry->sound;
                        sample = IndexFromSymbol(soundName);
                        if (sample == 0) {
                            char prefixed[64];
                            std::strcpy(prefixed, "SND_");
                            std::strcat(prefixed, soundName);
                            sample = IndexFromSymbol(prefixed);
                        }
                    }
                    ++g_plays; g_lastSample = sample; g_lastBank = bank;
                }
            }
            else if (time >= subtitle.EndTime) {
                IsSubtitleTime = false;
                ++first; --count;
            }
        }
        if (IsSubtitleTime && display)
            ++g_draws;
        ++FrameCounter;
    }
};

int main()
{
    int ok = 1;
    Entry e1 = { "Hello", "bank", "HELLO" }, e2 = { "Bye", "bank", "BYE" }, e3 = { "Mute", "none", "X" };
    Subtitles s;
    s.list[0].entry = &e1; s.list[0].StartTime = 1.0f; s.list[0].EndTime = 2.0f;
    s.list[1].entry = &e2; s.list[1].StartTime = 3.0f; s.list[1].EndTime = 4.0f;
    s.list[2].entry = &e3; s.list[2].StartTime = 5.0f; s.list[2].EndTime = 6.0f;
    s.first = 0; s.count = 3; s.IsSubtitleTime = false; s.FrameCounter = 0;
    s.Play(0.5f, true);                                      // before the first start: nothing shown
    if (s.VSubtitleWindow.TLX != 100.0f || s.VSubtitleWindow.BRX != 900.0f || s.VSubtitleWindow.TLY != 500.0f ||
        s.VSubtitleWindow.BRY != 770.0f || g_plays != 0 || g_draws != 0) ok = 0;
    s.Play(1.0f, true);                                      // start reached: format, play sample 3, draw
    if (!s.IsSubtitleTime || std::strcmp(g_formatted, "Hello") != 0 || g_plays != 1 || g_lastSample != 3 || g_draws != 1) ok = 0;
    s.Play(1.5f, false);                                     // showing but not asked to draw
    if (g_draws != 1) ok = 0;
    s.Play(2.0f, true);                                      // end reached: dropped, not drawn
    if (s.IsSubtitleTime || s.count != 2 || g_draws != 1) ok = 0;
    s.Play(3.5f, true);                                      // "SND_" fallback
    if (g_lastSample != 9 || g_plays != 2) ok = 0;
    s.Play(4.0f, true);
    s.Play(5.0f, true);                                      // no bank: still "played" with sample 0
    if (g_lastSample != 0 || g_lastBank != 0 || g_plays != 3) ok = 0;
    s.Play(7.0f, true);
    s.Play(8.0f, true);                                      // list empty: just counts
    if (s.count != 0 || s.IsSubtitleTime || s.FrameCounter != 9) ok = 0;
    if (!ok) { std::printf("009bd910_TEST FAIL\n"); return 1; }
    std::printf("009bd910_TEST PASS\n");
    return 0;
}