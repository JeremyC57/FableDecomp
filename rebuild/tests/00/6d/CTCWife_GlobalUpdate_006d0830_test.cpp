// Behaviour test for 0x006D0830 CTCWife::GlobalUpdate(bool): divorce: nothing until the acknowledge message (0x11); a living wife
// with a home house the player owns shows the renting text once (WaitingForDivorceRenting), otherwise Divorce + unpause. Fade-out
// (once the screen is fully faded): wedding = movie + fresco quest only for a female wife (sex 2), the nonpersistent / fresco
// inits, time fast-forward to WeddingAfterFresco / 24, every village OnHeroSlept, wife placed at home (3 m) and a living hero next
// to her (1 m), both turned to face each other (angles in turns wrapped into [0, 1)), camera reset, message 0x42, fade end, unpause,
// WaitingForFadeIn; sex = cutscene flags + mode 0x1b + sound state reset. Sex cutscene: skipped (message 0x32) or past the minimum /
// waits ends it, else sound scheduling. Fade-in (screen no longer faded out): dowry text with the def's dowry unless the info screen
// is open, or WaitingForSex cleared. Dowry: the acknowledge message ends it.
#include <cstdio>
#include <cmath>
#include <vector>
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static const unsigned long INVALID = 0xffffffffUL;
static float Rounded(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)std::fmod(a, 1.0f); if (a < 0.0f) a += 1.0f; } return a; }
static float AngleXY(float x, float y) { return Rounded((float)std::atan2((double)x, (double)y) / (2.0f * 3.14159265358979323846f)); }
struct Wife {
    // state
    bool WaitingForDivorce, WaitingForDivorceRenting, WaitingForFadeOut, WaitingForSex, WaitingForSexCutscene, WaitingForFadeIn, WaitingForDowry;
    unsigned long SampleID; long FrameStarted, FrameEnded, FrameLove; float Gap;
    // world
    bool ack, skippedMsg, fadedOut, screenFadeOut, alive, home, owned, heroExists, heroAlive, infoOpen, hasDef, soundPlayer, soundPlaying;
    int sex, villages; long dowryDef, frame; float fps, minSecs, waitSecs, timeOfDay;
    float wx, wy, hx, hy;
    // outputs
    int movies, quests, inits, slept, divorced, renting, pauses, unpauses, dowryTexts, camera, messages, fadeEnds, deeds, stops, plays;
    long dowryShown; float fastForward, heroFacing, wifeFacing; bool wifePlaced, heroPlaced, globalAdded, globalRemoved;
    Wife() : WaitingForDivorce(false), WaitingForDivorceRenting(false), WaitingForFadeOut(false), WaitingForSex(false),
             WaitingForSexCutscene(false), WaitingForFadeIn(false), WaitingForDowry(false), SampleID(INVALID), FrameStarted(0),
             FrameEnded(0x7fffffff), FrameLove(0), Gap(1.0f), ack(false), skippedMsg(false), fadedOut(true), screenFadeOut(true),
             alive(true), home(true), owned(false), heroExists(true), heroAlive(true), infoOpen(false), hasDef(true),
             soundPlayer(true), soundPlaying(false), sex(2), villages(3), dowryDef(500), frame(1000), fps(30.0f), minSecs(10.0f),
             waitSecs(2.0f), timeOfDay(12.0f), wx(0), wy(1), hx(0), hy(0), movies(0), quests(0), inits(0), slept(0), divorced(0),
             renting(0), pauses(0), unpauses(0), dowryTexts(0), camera(0), messages(0), fadeEnds(0), deeds(0), stops(0), plays(0),
             dowryShown(-1), fastForward(-1), heroFacing(-1), wifeFacing(-1), wifePlaced(false), heroPlaced(false),
             globalAdded(false), globalRemoved(false) { }
};
// Mirror of the row's decisions (same statement order as src/006d0830.cpp).
static void GlobalUpdate(Wife& w) {
    if (!w.WaitingForDivorce) {
        if (w.WaitingForFadeOut && w.fadedOut) {
            w.WaitingForFadeOut = false;
            if (!w.WaitingForSex) {
                if (w.sex == 2) { ++w.movies; ++w.quests; }
                w.inits += 2;
                w.fastForward = w.timeOfDay * (1.0f / 24.0f);
                for (int i = 0; i < w.villages; ++i) ++w.slept;
                if (w.home) {
                    w.wifePlaced = true;
                    if (w.heroExists && w.heroAlive) {
                        w.heroPlaced = true;
                        float dx = w.wx - w.hx, dy = w.wy - w.hy;
                        w.heroFacing = AngleXY(dx, dy);
                        w.wifeFacing = AngleXY(-dx, -dy);
                    }
                }
                ++w.camera; ++w.messages; ++w.fadeEnds; ++w.unpauses;
                w.WaitingForFadeIn = true;
            } else {
                w.WaitingForSexCutscene = true; w.SampleID = INVALID; w.FrameStarted = w.frame; w.FrameEnded = 0x7fffffff; w.Gap = 1.0f;
            }
        } else if (w.WaitingForSexCutscene) {
            bool skipped = w.skippedMsg;
            if (!skipped) {
                if (!((float)(w.frame - w.FrameStarted) > w.fps * w.minSecs)) {
                    if (!w.soundPlayer) return;
                    if (w.SampleID == INVALID) {
                        if (w.FrameEnded != 0x7fffffff && !((float)(w.frame - w.FrameEnded) > w.waitSecs * w.Gap * w.fps)) return;
                        w.SampleID = 7; ++w.plays; w.FrameEnded = 0x7fffffff;
                    } else if (!w.soundPlaying) {
                        w.SampleID = INVALID; w.FrameEnded = w.frame; w.Gap = 0.75f;
                    }
                    return;
                }
            }
            w.FrameEnded = 0x7fffffff;
            if (!w.soundPlayer || (w.SampleID != INVALID && !w.soundPlaying)) { w.SampleID = INVALID; w.FrameEnded = w.frame; }
            if (!skipped && !(w.FrameEnded != 0x7fffffff && (float)(w.frame - w.FrameEnded) > w.fps * w.waitSecs) &&
                !((float)(w.frame - w.FrameStarted) > (w.minSecs + w.waitSecs) * w.fps))
                return;
            w.WaitingForSexCutscene = false; w.FrameStarted = 0x7fffffff; w.FrameEnded = 0x7fffffff; w.Gap = 1.0f;
            if (w.SampleID != INVALID && w.soundPlayer) ++w.stops;
            w.FrameLove = 0x7fffffff; ++w.fadeEnds; ++w.unpauses;
            if (w.heroExists && w.heroAlive) ++w.deeds;
            w.WaitingForFadeIn = true;
        } else if (w.WaitingForFadeIn && !w.screenFadeOut) {
            w.globalRemoved = true;
            w.WaitingForFadeIn = false;
            if (!w.WaitingForSex) {
                long dowry = 0;
                if (w.hasDef) dowry = w.dowryDef;
                if (!w.infoOpen) { ++w.dowryTexts; w.dowryShown = dowry; w.WaitingForDowry = true; w.globalAdded = true; ++w.pauses; }
            } else {
                w.WaitingForSex = false;
            }
        } else if (w.WaitingForDowry) {
            if (w.ack) { w.globalRemoved = true; w.WaitingForDowry = false; ++w.unpauses; }
        }
    } else {
        if (!w.ack) return;
        if (!w.WaitingForDivorceRenting && w.alive && w.home && w.owned) { ++w.renting; w.WaitingForDivorceRenting = true; return; }
        w.WaitingForDivorceRenting = false; w.WaitingForDivorce = false; ++w.divorced; w.globalRemoved = true; ++w.unpauses;
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main() {
    { Wife w; w.WaitingForDivorce = true; GlobalUpdate(w); check(w.divorced == 0 && w.renting == 0 && w.WaitingForDivorce, "divorce waits for ack"); }
    { Wife w; w.WaitingForDivorce = true; w.ack = true; w.owned = true; GlobalUpdate(w);
      check(w.renting == 1 && w.WaitingForDivorceRenting && w.WaitingForDivorce && w.divorced == 0, "owned home: renting text once");
      GlobalUpdate(w); check(w.renting == 1 && w.divorced == 1 && !w.WaitingForDivorce && !w.WaitingForDivorceRenting, "then divorce"); }
    { Wife w; w.WaitingForDivorce = true; w.ack = true; w.owned = true; w.alive = false; GlobalUpdate(w); check(w.renting == 0 && w.divorced == 1, "dead wife: straight divorce"); }
    { Wife w; w.WaitingForFadeOut = true; w.fadedOut = false; GlobalUpdate(w); check(w.WaitingForFadeOut && w.inits == 0, "not faded out yet"); }
    { Wife w; w.WaitingForFadeOut = true; w.hx = 0; w.hy = 0; w.wx = 0; w.wy = 1; w.timeOfDay = 6.0f; GlobalUpdate(w);
      check(!w.WaitingForFadeOut && w.movies == 1 && w.quests == 1 && w.inits == 2 && w.slept == 3, "wedding: movie, inits, villages");
      check(Near(w.fastForward, 0.25f), "fast forward to 6/24");
      check(w.wifePlaced && w.heroPlaced && Near(w.heroFacing, 0.0f) && Near(w.wifeFacing, 0.5f), "facing each other");
      check(w.camera == 1 && w.messages == 1 && w.fadeEnds == 1 && w.unpauses == 1 && w.WaitingForFadeIn, "wedding tail"); }
    { Wife w; w.WaitingForFadeOut = true; w.sex = 1; w.wx = 1; w.wy = 0; GlobalUpdate(w);
      check(w.movies == 0 && w.inits == 2 && Near(w.heroFacing, 0.25f) && Near(w.wifeFacing, 0.75f), "male: no movie; east / west"); }
    { Wife w; w.WaitingForFadeOut = true; w.heroAlive = false; GlobalUpdate(w); check(w.wifePlaced && !w.heroPlaced && w.heroFacing == -1, "dead hero not placed"); }
    { Wife w; w.WaitingForFadeOut = true; w.home = false; GlobalUpdate(w); check(!w.wifePlaced && !w.heroPlaced && w.camera == 1, "homeless: no placement"); }
    { Wife w; w.WaitingForFadeOut = true; w.WaitingForSex = true; GlobalUpdate(w);
      check(w.WaitingForSexCutscene && w.FrameStarted == 1000 && w.inits == 0 && !w.WaitingForFadeIn, "sex cutscene starts"); }
    { Wife w; w.WaitingForSexCutscene = true; w.FrameStarted = 900; GlobalUpdate(w); check(w.plays == 1 && w.SampleID == 7 && w.WaitingForSexCutscene, "first sound"); }
    { Wife w; w.WaitingForSexCutscene = true; w.FrameStarted = 900; w.SampleID = 7; w.soundPlaying = false; GlobalUpdate(w);
      check(w.SampleID == INVALID && w.FrameEnded == 1000 && w.WaitingForSexCutscene, "sound ended: gap starts"); }
    { Wife w; w.WaitingForSexCutscene = true; w.FrameStarted = 900; w.FrameEnded = 990; GlobalUpdate(w); check(w.plays == 0, "inside the gap: wait"); }
    { Wife w; w.WaitingForSexCutscene = true; w.skippedMsg = true; w.SampleID = 7; w.soundPlaying = true; GlobalUpdate(w);
      check(!w.WaitingForSexCutscene && w.stops == 1 && w.deeds == 1 && w.WaitingForFadeIn && w.unpauses == 1, "skipped: ends, stops sound"); }
    { Wife w; w.WaitingForSexCutscene = true; w.FrameStarted = 650; w.SampleID = 7; w.soundPlaying = true; GlobalUpdate(w);
      check(w.WaitingForSexCutscene && w.stops == 0, "past minimum, sound still playing: keep"); }
    { Wife w; w.WaitingForSexCutscene = true; w.FrameStarted = 650; w.SampleID = 7; w.soundPlaying = false; w.frame = 1000; GlobalUpdate(w);
      check(w.WaitingForSexCutscene, "past minimum, sound just ended: wait after"); }
    { Wife w; w.WaitingForSexCutscene = true; w.FrameStarted = 0; GlobalUpdate(w); check(!w.WaitingForSexCutscene && w.WaitingForFadeIn, "past min + wait: ends"); }
    { Wife w; w.WaitingForFadeIn = true; w.screenFadeOut = false; GlobalUpdate(w);
      check(!w.WaitingForFadeIn && w.dowryTexts == 1 && w.dowryShown == 500 && w.WaitingForDowry && w.pauses == 1, "dowry text"); }
    { Wife w; w.WaitingForFadeIn = true; w.screenFadeOut = false; w.hasDef = false; GlobalUpdate(w); check(w.dowryShown == 0, "no def: dowry 0"); }
    { Wife w; w.WaitingForFadeIn = true; w.screenFadeOut = false; w.infoOpen = true; GlobalUpdate(w); check(w.dowryTexts == 0 && !w.WaitingForDowry, "info screen open: no text"); }
    { Wife w; w.WaitingForFadeIn = true; w.screenFadeOut = false; w.WaitingForSex = true; GlobalUpdate(w); check(!w.WaitingForSex && w.dowryTexts == 0, "after sex: clear flag"); }
    { Wife w; w.WaitingForFadeIn = true; GlobalUpdate(w); check(w.WaitingForFadeIn, "still faded out: wait"); }
    { Wife w; w.WaitingForDowry = true; GlobalUpdate(w); check(w.WaitingForDowry, "dowry waits for ack");
      w.ack = true; GlobalUpdate(w); check(!w.WaitingForDowry && w.unpauses == 1, "dowry acknowledged"); }
    if (!fails) std::printf("006d0830_TEST PASS\n");
    return fails;
}