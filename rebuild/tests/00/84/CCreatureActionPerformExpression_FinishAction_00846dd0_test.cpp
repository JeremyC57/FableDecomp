// Behaviour test for 0x00846DD0 CCreatureAction_PerformExpression::FinishAction: a standalone model of the decision flow
// (finished + extended + button held -> PerformExpressionExtended replacement; otherwise base finish, clean-up, the
// FOLLOW/WAIT opinion deeds for the hero, then the out-of animation).
#include <cstdio>
#include <cstring>

struct Model {
    bool replaced, extended, buttonHeld, alive, emoteAdded, deactivate, hasScript, performTo, performToAI, performerHero;
    bool respondingToFollowAndWait, hasFollowed, validFollower, addToFront, isFollower, isAIFollower, hasToOther, hasOut;
    long framesRunning, totalTime; unsigned long sound; const char* name;
    // outputs
    bool startedExtended, itemCleared, baseFinished, emoteCleared, soundReleased, questDeactivated, eventPosted;
    long deed; int animation;
};

static void FinishAction(Model& m)
{
    if (m.replaced) return;
    if (m.framesRunning >= m.totalTime) {
        if (m.extended && m.buttonHeld) { m.replaced = true; m.startedExtended = true; }
    } else
        m.itemCleared = true;
    m.baseFinished = true;
    if (!m.replaced) {
        if (m.alive && m.emoteAdded) m.emoteCleared = true;
        if (m.sound > 0) m.soundReleased = true;
        if (m.deactivate && m.hasScript) m.questDeactivated = true;
    }
    if (m.performTo) {
        bool skip = m.performToAI && (!std::strcmp(m.name, "EXPRESSION_FOLLOW") || !std::strcmp(m.name, "EXPRESSION_WAIT"))
                    && !m.respondingToFollowAndWait;
        if (!skip) {
            m.eventPosted = true;
            if (m.performToAI && m.performerHero && m.hasFollowed) {
                if (!std::strcmp(m.name, "EXPRESSION_FOLLOW")) {
                    if (m.addToFront && !m.isFollower) m.deed = m.validFollower ? 0x2e : 0x2f;
                } else if (!std::strcmp(m.name, "EXPRESSION_WAIT")) {
                    if (m.isAIFollower) m.deed = 0x30;
                }
            }
        }
    }
    if (m.performTo && m.hasToOther) m.animation = 1;
    else if (m.hasOut) m.animation = 2;
}

int main()
{
    Model a = Model(); a.name = "EXPRESSION_WAVE";
    a.framesRunning = 10; a.totalTime = 5; a.extended = true; a.buttonHeld = true; a.hasOut = true;
    FinishAction(a);
    if (!a.startedExtended || a.emoteCleared || a.animation != 2) { std::printf("00846dd0_TEST FAIL\n"); return 1; }
    Model b = Model(); b.name = "EXPRESSION_FOLLOW";
    b.framesRunning = 10; b.totalTime = 5; b.alive = true; b.emoteAdded = true; b.sound = 3; b.performTo = true; b.performToAI = true;
    b.performerHero = true; b.respondingToFollowAndWait = true; b.hasFollowed = true; b.addToFront = true; b.validFollower = false;
    b.hasToOther = true;
    FinishAction(b);
    if (b.startedExtended || !b.emoteCleared || !b.soundReleased || !b.eventPosted || b.deed != 0x2f || b.animation != 1) {
        std::printf("00846dd0_TEST FAIL\n"); return 1; }
    Model c = Model(); c.name = "EXPRESSION_WAIT";
    c.framesRunning = 1; c.totalTime = 5; c.performTo = true; c.performToAI = true; c.respondingToFollowAndWait = false; c.hasOut = true;
    FinishAction(c);
    if (!c.itemCleared || c.eventPosted || c.deed != 0 || c.animation != 2) { std::printf("00846dd0_TEST FAIL\n"); return 1; }
    std::printf("00846dd0_TEST PASS\n");
    return 0;
}