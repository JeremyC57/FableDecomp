// Behaviour test for 0x00714F60 NTCScriptedControl::CActionOverheadTalkToThing::DisplayText
#include <cstdio>
#include <cstring>
static long g_FrameRate = 30;
struct CEmote {
    int calls; long style; long length; float seconds; float fade;
    void SetEmoteText(long s, const char* text, float secs, float f) { ++calls; style = s; length = (long)std::strlen(text); seconds = secs; fade = f; }
};
struct CTalk {
    const char* lines[2]; long CurrentMessageIndex; long GameTurnDuration; bool ScrollText, FinishedScroll;
    bool hasEmote; CEmote* emote;
    const char* GetCurrentText() const { return lines[CurrentMessageIndex]; }
    void DisplayText()
    {
        ScrollText = (long)std::strlen(GetCurrentText()) >= 25;
        FinishedScroll = false;
        if (ScrollText)
            return;
        if (!hasEmote)
            return;
        emote->SetEmoteText(7, GetCurrentText(), (float)GameTurnDuration / g_FrameRate, 0.3f);
    }
};
int main()
{
    CEmote e; e.calls = 0;
    CTalk t; t.lines[0] = "Hello there."; t.lines[1] = "This line is certainly longer than 25.";
    t.CurrentMessageIndex = 0; t.GameTurnDuration = 60; t.ScrollText = true; t.FinishedScroll = true; t.hasEmote = true; t.emote = &e;
    t.DisplayText();
    int ok = !t.ScrollText && !t.FinishedScroll && e.calls == 1 && e.style == 7 && e.length == 12 && e.seconds == 2.0f && e.fade == 0.3f;
    t.CurrentMessageIndex = 1; t.FinishedScroll = true;
    t.DisplayText();
    ok = ok && t.ScrollText && !t.FinishedScroll && e.calls == 1;
    t.CurrentMessageIndex = 0; t.hasEmote = false;
    t.DisplayText();
    ok = ok && !t.ScrollText && e.calls == 1;
    if (!ok) { std::printf("00714f60_TEST FAIL\n"); return 1; }
    std::printf("00714f60_TEST PASS\n");
    return 0;
}