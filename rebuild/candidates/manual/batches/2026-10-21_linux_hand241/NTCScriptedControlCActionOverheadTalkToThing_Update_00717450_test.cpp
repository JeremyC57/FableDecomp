#include <cstdio>
#include <cstring>
// Behaviour copy of CActionOverheadTalkToThing::Update (0x00717450): scroll the current line through the emote icon.
static const long& Max(const long& a, const long& b) { return a < b ? b : a; }
struct Emote { int calls; char text[64]; float seconds; long style; };
struct Talk {
    bool terminated, scrollText, finishedScroll; bool hasEmote; Emote* emote;
    long updateCounter, gameTurnDuration, frameRate; const char* line; int baseUpdates;
    void Update() {
        ++baseUpdates;
        if (terminated) return;
        if (!scrollText) return;
        if (finishedScroll) return;
        if (!hasEmote) return;
        long shown;
        { long chars = updateCounter - gameTurnDuration / 10; shown = Max(0L, chars); }
        {
            char text[64];
            long len = (long)std::strlen(line);
            bool partial = shown < len;
            if (partial) { std::strncpy(text, line + shown, 24); text[24] = 0; }
            else std::strcpy(text, line);
            bool finished = shown > len - 25;
            if (finished) finishedScroll = true;
            emote->calls++; std::strcpy(emote->text, text); emote->seconds = (float)(gameTurnDuration - updateCounter) / frameRate; emote->style = 7;
        }
    }
};
int main()
{
    Emote e = { 0, "", 0, 0 };
    const char* line = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJ";   // 46 chars
    Talk t = { false, true, false, true, &e, 100, 300, 15, line, 0 };
    t.Update();                                   // shown = 100 - 30 = 70 >= 46: whole line, finished
    if (e.calls != 1 || std::strcmp(e.text, line) != 0 || !t.finishedScroll || e.style != 7 || e.seconds != (float)200 / 15) { std::printf("00717450_TEST FAIL\n"); return 1; }
    t.finishedScroll = false; t.updateCounter = 35;   // shown = 5: 24 chars from 'f', 5 > 21 false
    t.Update();
    if (e.calls != 2 || std::strcmp(e.text, "fghijklmnopqrstuvwxyz012") != 0 || t.finishedScroll) { std::printf("00717450_TEST FAIL\n"); return 1; }
    t.updateCounter = 10;                             // chars = -20 -> shown 0
    t.Update();
    if (e.calls != 3 || std::strncmp(e.text, line, 24) != 0 || e.text[24] != 0) { std::printf("00717450_TEST FAIL\n"); return 1; }
    t.updateCounter = 52;                             // shown 22 < 46 partial; 22 > 21 -> finished
    t.Update();
    if (e.calls != 4 || !t.finishedScroll) { std::printf("00717450_TEST FAIL\n"); return 1; }
    t.Update();                                       // finished: no call
    t.finishedScroll = false; t.terminated = true; t.Update();
    t.terminated = false; t.scrollText = false; t.Update();
    t.scrollText = true; t.hasEmote = false; t.Update();
    if (e.calls != 4 || t.baseUpdates != 8) { std::printf("00717450_TEST FAIL\n"); return 1; }
    std::printf("00717450_TEST PASS\n"); return 0;
}
