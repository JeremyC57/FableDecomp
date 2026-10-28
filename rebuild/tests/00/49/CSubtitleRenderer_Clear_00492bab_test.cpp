// Behaviour test for CSubtitleRenderer::Clear(bool keepText) (0x00492BAB): always clear the primitive
// handles; unless keeping the text, reset captions/overrides, remove the player mode it added (only while the
// hero is alive), force-clear the hero's received-items display (flag 0x40), post the text-finished messages
// (flag 8 -> type 0x12, then flag 2 -> type 0x11), unpause, and show + pop the next queued text.
#include <cstdio>

struct Renderer {
    int handlesCleared;
    bool displayingText, updateTimer, pausedGame;
    int button1Alpha, button2Alpha;
    int playerModeAdded;
    unsigned long displayFlags;
    int removedMode;
    bool heroAlive, heroHasItems, forceClear;
    int messages[4]; int nMessages;
    int pauseEvents;
    int queue[4]; int qHead, qTail;
    int shownText;
};

static void Clear(Renderer& r, bool keepText)
{
    r.handlesCleared += 3;
    if (keepText)
        return;
    r.displayingText = false;
    r.updateTimer = true;
    r.button1Alpha = 0xff;
    r.button2Alpha = 0xff;
    int mode = r.playerModeAdded;
    if (mode != 0) {
        if (r.heroAlive)
            r.removedMode = mode;
        r.playerModeAdded = 0;
    }
    if ((r.displayFlags & 0x40) && r.heroAlive && r.heroHasItems)
        r.forceClear = true;
    if (r.displayFlags & 8) r.messages[r.nMessages++] = 0x12;
    if (r.displayFlags & 2) r.messages[r.nMessages++] = 0x11;
    r.displayFlags = 0;
    if (r.pausedGame) {
        ++r.pauseEvents;
        r.pausedGame = false;
    }
    if (r.qHead != r.qTail) {
        r.shownText = r.queue[r.qHead];
        ++r.qHead;
    }
}

static Renderer Make()
{
    Renderer r;
    r.handlesCleared = 0; r.displayingText = true; r.updateTimer = false; r.pausedGame = true;
    r.button1Alpha = 3; r.button2Alpha = 4; r.playerModeAdded = 5; r.displayFlags = 0x4A;
    r.removedMode = 0; r.heroAlive = true; r.heroHasItems = true; r.forceClear = false;
    r.nMessages = 0; r.pauseEvents = 0;
    r.queue[0] = 77; r.queue[1] = 88; r.qHead = 0; r.qTail = 2; r.shownText = 0;
    return r;
}

int main()
{
    int ok = 1;
    Renderer a = Make();
    Clear(a, true);
    ok &= a.handlesCleared == 3 && a.displayingText && a.playerModeAdded == 5 && a.qHead == 0;

    Renderer b = Make();
    Clear(b, false);
    ok &= !b.displayingText && b.updateTimer && b.button1Alpha == 0xff && b.button2Alpha == 0xff;
    ok &= b.removedMode == 5 && b.playerModeAdded == 0 && b.forceClear;
    ok &= b.nMessages == 2 && b.messages[0] == 0x12 && b.messages[1] == 0x11 && b.displayFlags == 0;
    ok &= b.pauseEvents == 1 && !b.pausedGame && b.shownText == 77 && b.qHead == 1;

    Renderer c = Make();
    c.heroAlive = false; c.displayFlags = 0x40; c.pausedGame = false; c.qTail = 0;
    Clear(c, false);
    ok &= c.removedMode == 0 && c.playerModeAdded == 0 && !c.forceClear && c.nMessages == 0;
    ok &= c.pauseEvents == 0 && c.shownText == 0;

    if (!ok) { std::printf("00492bab_TEST FAIL\n"); return 1; }
    std::printf("00492bab_TEST PASS\n");
    return 0;
}