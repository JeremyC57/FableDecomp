// Behaviour test for CPlayerGui::Update @ 0x0043A080: the DrawGui toggle state machine, the paused gate on the stat
// updates, expiry of script screen texts older than the previous frame, and screen-message ageing with the line budget.
#include <cstdio>
#include <list>
struct ScriptText { int FrameAdded; };
struct ScreenMessage { int FramesToShowFor; int LinesInText; void Update() { --FramesToShowFor; } };
struct Gui {
    bool DrawGui, OldDrawGui, ForcedClosed;
    int GuiMode;
    float FadeInTimer;
    int Cleared, StatUpdates, Managers;
    std::list<ScriptText> Texts;
    std::list<ScreenMessage> Messages;
    int MaxLines;
    void Update(bool paused, int worldFrame)
    {
        if (DrawGui != OldDrawGui) {
            if (DrawGui && GuiMode == 1) {
                if (!ForcedClosed)
                    GuiMode = 2;
            } else if (!DrawGui && (GuiMode == 0 || GuiMode == 2)) {
                GuiMode = 1;
                FadeInTimer = 0.0f;
                ++Cleared;
            }
            OldDrawGui = DrawGui;
        }
        ++Managers;
        if (!paused)
            ++StatUpdates;
        std::list<ScriptText>::iterator t = Texts.begin();
        while (t != Texts.end()) {
            if (t->FrameAdded < worldFrame - 1)
                t = Texts.erase(t);
            else
                ++t;
        }
        int lines = 0;
        std::list<ScreenMessage>::iterator m = Messages.begin();
        while (m != Messages.end()) {
            m->Update();
            if (m->FramesToShowFor > 0) {
                lines += m->LinesInText;
                if (lines >= MaxLines)
                    break;
                ++m;
            } else {
                m = Messages.erase(m);
            }
        }
    }
};
int main()
{
    Gui g;
    g.DrawGui = true; g.OldDrawGui = false; g.ForcedClosed = false; g.GuiMode = 1; g.FadeInTimer = 5.0f;
    g.Cleared = 0; g.StatUpdates = 0; g.Managers = 0; g.MaxLines = 3;
    ScriptText a = { 8 }, b = { 9 }, c = { 10 };
    g.Texts.push_back(a); g.Texts.push_back(b); g.Texts.push_back(c);
    ScreenMessage m1 = { 1, 1 }, m2 = { 5, 2 }, m3 = { 5, 2 }, m4 = { 1, 1 };
    g.Messages.push_back(m1); g.Messages.push_back(m2); g.Messages.push_back(m3); g.Messages.push_back(m4);
    g.Update(false, 10);
    bool ok = g.GuiMode == 2 && g.OldDrawGui && g.StatUpdates == 1 && g.Texts.size() == 2 &&
              g.Messages.size() == 3 && g.Messages.back().FramesToShowFor == 1;
    g.DrawGui = false;
    g.Update(true, 10);
    ok = ok && g.GuiMode == 1 && g.FadeInTimer == 0.0f && g.Cleared == 1 && g.StatUpdates == 1 && g.Managers == 2;
    if (!ok) { std::printf("0043a080_TEST FAIL\n"); return 1; }
    std::printf("0043a080_TEST PASS\n");
    return 0;
}