// Behaviour test for 0x00847520 CCreatureAction_PerformExpressionExtended::FinishAction: a standalone model of the replace /
// finish decision (button still held -> replacement action with frames running + FrameCount; else clean-up and out-of animation).
#include <cstdio>

struct Model {
    bool replaced, finished, hasInventory, quickHeld, contextHeld, quickButton, contextButton;
    bool alive, emoteAdded, deactivateScript, hasScript, performTo, hasToOther, hasOut;
    long framesRunning, frameCount, group; unsigned long sound;
    // outputs
    bool startedReplacement; long replacementFrames, replacementGroup;
    bool baseFinished, emoteCleared, soundReleased, questDeactivated; int animation; // 0 none, 1 to-other, 2 out-of
};

static void FinishAction(Model& m)
{
    if (!m.replaced && m.finished && m.hasInventory) {
        if (m.quickButton && m.quickHeld) m.replaced = true;
        if (!m.replaced && m.contextButton && m.contextHeld) m.replaced = true;
        if (m.replaced) {
            m.startedReplacement = true;
            m.replacementFrames = m.framesRunning + m.frameCount;
            m.replacementGroup = m.group + 1;
        }
    }
    if (m.replaced) return;
    m.baseFinished = true;
    if (m.alive && m.emoteAdded) m.emoteCleared = true;
    if (m.sound > 0) m.soundReleased = true;
    if (m.deactivateScript && m.hasScript) m.questDeactivated = true;
    if (m.performTo && m.hasToOther) m.animation = 1;
    else if (m.hasOut) m.animation = 2;
}

int main()
{
    Model a = Model();
    a.finished = true; a.hasInventory = true; a.contextButton = true; a.contextHeld = true; a.framesRunning = 10; a.frameCount = 5; a.group = 3;
    FinishAction(a);
    if (!a.startedReplacement || a.replacementFrames != 15 || a.replacementGroup != 4 || a.baseFinished) { std::printf("00847520_TEST FAIL\n"); return 1; }
    Model b = Model();
    b.finished = true; b.hasInventory = true; b.alive = true; b.emoteAdded = true; b.sound = 7; b.deactivateScript = true; b.hasScript = true;
    b.performTo = true; b.hasToOther = false; b.hasOut = true;
    FinishAction(b);
    if (b.startedReplacement || !b.baseFinished || !b.emoteCleared || !b.soundReleased || !b.questDeactivated || b.animation != 2) {
        std::printf("00847520_TEST FAIL\n"); return 1; }
    std::printf("00847520_TEST PASS\n");
    return 0;
}