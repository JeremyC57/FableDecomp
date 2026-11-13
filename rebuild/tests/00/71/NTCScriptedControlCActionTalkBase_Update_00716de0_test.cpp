// Behaviour test for NTCScriptedControl::CActionTalkBase::Update (0x00716DE0): a host model of the row, statement for statement as
// src/00716de0.cpp, over stand-in speaker / listener / talk / look / subtitle objects that record what the update asks of them:
//  - look-at: the speaker looks at the listener's focal point (unless its look is overridden), LookingAtListener = in view range,
//    and the listener looks back; a missing / self target skips both;
//  - speech: the line timer starts once the speech is not pending (no talk TC, not playing, or loaded and playing);
//  - end of line: a skipped line or the line time running out (waits while the subtitle waits for a click-past outside a
//    cutscene) clears the subtitle, stops the speech, and starts the next line or terminates;
//  - tags: animation / attitude / camera tags fire when UpdateCounter reaches key * GameTurnDuration, gestures every
//    FramesUntilNextGesture frames; the camera falls back to the default camera tag and re-sets it after ten moved frames.
#include <cstdio>
#include <cstring>
#include <cmath>

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

struct Vec { float x, y, z; };
struct Look { bool overridden; bool inView; int lookAtCalls; Vec lastPos; };
struct Thing { Vec focal; Look* look; bool ableToConverse; int actions; int lastPriority; int lastKind; long lastAnim; };
struct Talk { bool playing; bool loadedAndPlaying; int attitude; int stops; };
struct Subtitles { bool inCutscene; bool waiting; bool updateTimer; int clears; };

struct AnimTag { float key; long anim; };
struct AttTag { float key; int attitude; };
struct CamTag { float key; int camera; };
struct Message { AnimTag anims[4]; long animCount; AttTag atts[4]; long attCount; CamTag cams[4]; long camCount; };

static double g_time = 0.0;
static const long kFPS = 30;

struct Action {
    Thing* parent; Thing* target; Talk* talk; Subtitles* subs;
    bool terminated;
    long UpdateCounter; double TimeCurrentLineStarted, TimeCurrentLineLasts;
    Message* Messages; long MessageCount; long CurrentMessageIndex; long GameTurnDuration;
    long NextAttitudeTagIndex, NextAnimationTagIndex, NextCameraTagIndex, LastActionPriority, FramesUntilNextGesture, LastGestureAnimIndex;
    long CameraToBeMovedCount;
    bool CameraTagsWereUsed, LookingAtListener, SoundStartedPlaying, SkippedCurrentLine;
    long CurrentCameraTagIndex; int defaultCameras[3];
    int setUpCameraCalls; int lastCamera; int setUpTextCalls; bool cameraMoved;

    void SetUpCamera(int camera) { ++setUpCameraCalls; lastCamera = camera; }
    void SetUpTextAndSpeech() { ++setUpTextCalls; }
    void Act(int kind, long anim, long prio) { parent->actions++; parent->lastKind = kind; parent->lastAnim = anim; parent->lastPriority = (int)prio; }

    void Update()
    {
        LookingAtListener = true;
        if (target && target != parent) {
            if (parent->look) {
                Vec pos = target->focal;
                if (!parent->look->overridden) { parent->look->lookAtCalls++; parent->look->lastPos = pos; }
                LookingAtListener = parent->look->inView;
            }
            if (target->look) {
                Vec pos = parent->focal;
                if (!target->look->overridden) { target->look->lookAtCalls++; target->look->lastPos = pos; }
            }
        }
        if (!SoundStartedPlaying) {
            if (!talk || !talk->playing || talk->loadedAndPlaying) {
                TimeCurrentLineStarted = g_time;
                if (!subs->inCutscene) subs->updateTimer = true;
                SoundStartedPlaying = true;
            }
        }
        if (SoundStartedPlaying) ++UpdateCounter;
        if (SkippedCurrentLine || (SoundStartedPlaying && g_time - TimeCurrentLineStarted > TimeCurrentLineLasts)) {
            if (!subs->inCutscene && subs->waiting) return;
            subs->clears++;
            if (talk) talk->stops++;
            if (++CurrentMessageIndex < MessageCount) { SetUpTextAndSpeech(); return; }
            terminated = true;
            return;
        }
        if (!target) { terminated = true; return; }
        Message& m = Messages[CurrentMessageIndex];
        if (NextAnimationTagIndex != -1) {
            if (m.anims[NextAnimationTagIndex].key * GameTurnDuration <= UpdateCounter) {
                Act(1, m.anims[NextAnimationTagIndex].anim, ++LastActionPriority);
                if (++NextAnimationTagIndex >= m.animCount) NextAnimationTagIndex = -1;
            }
        }
        if (FramesUntilNextGesture > 0) --FramesUntilNextGesture;
        if (FramesUntilNextGesture == 0 && LookingAtListener) {
            long anim = 77;                                                // the random CONV_GENERAL gesture
            float seconds = 0.5f;
            long duration = (long)std::ceil(seconds * kFPS);
            if (duration <= GameTurnDuration - UpdateCounter && parent->ableToConverse)
                Act(2, anim, LastActionPriority);
            LastGestureAnimIndex = anim;
            FramesUntilNextGesture = (long)std::ceil((1.0 + 0.5) * kFPS);
        }
        if (NextAttitudeTagIndex != -1) {
            if (m.atts[NextAttitudeTagIndex].key * GameTurnDuration <= UpdateCounter) {
                if (talk) talk->attitude = m.atts[NextAttitudeTagIndex].attitude;
                else {
                    ++LastActionPriority;
                    if (parent->ableToConverse) Act(3, m.atts[NextAttitudeTagIndex].attitude, LastActionPriority);
                }
                if (++NextAttitudeTagIndex >= m.attCount) NextAttitudeTagIndex = -1;
            }
        }
        bool camera_tag_set = false;
        if (NextCameraTagIndex != -1) {
            CameraTagsWereUsed = true;
            if (m.cams[NextCameraTagIndex].key * GameTurnDuration <= UpdateCounter) {
                SetUpCamera(m.cams[NextCameraTagIndex].camera);
                camera_tag_set = true;
                if (++NextCameraTagIndex >= m.camCount) NextCameraTagIndex = -1;
            }
        }
        int tag;
        if (NextCameraTagIndex > 0 && CameraTagsWereUsed && !camera_tag_set) tag = m.cams[NextCameraTagIndex - 1].camera;
        else {
            if (CurrentCameraTagIndex == -1) return;
            tag = defaultCameras[CurrentCameraTagIndex];
        }
        if (CameraToBeMovedCount) {
            if (++CameraToBeMovedCount > 10) { CameraToBeMovedCount = 0; SetUpCamera(tag); }
        } else if (cameraMoved) ++CameraToBeMovedCount;
    }
};

static void Reset(Action& a, Thing* parent, Thing* target, Talk* talk, Subtitles* subs, Message* msgs, long count)
{
    std::memset(&a, 0, sizeof(a));
    a.parent = parent; a.target = target; a.talk = talk; a.subs = subs; a.Messages = msgs; a.MessageCount = count;
    a.GameTurnDuration = 100; a.TimeCurrentLineLasts = 3.0; a.NextAttitudeTagIndex = a.NextAnimationTagIndex = a.NextCameraTagIndex = -1;
    a.FramesUntilNextGesture = -1; a.CurrentCameraTagIndex = -1; a.lastCamera = -1;
    a.defaultCameras[0] = 100; a.defaultCameras[1] = 101; a.defaultCameras[2] = 102;
}

int main()
{
    Look lookA = { false, true, 0, {0, 0, 0} }, lookB = { true, false, 0, {0, 0, 0} };
    Thing speaker = { {1, 2, 3}, &lookA, true, 0, 0, 0, 0 }, listener = { {4, 5, 6}, &lookB, true, 0, 0, 0, 0 };
    Talk talk = { true, false, 0, 0 };
    Subtitles subs = { false, false, false, 0 };
    Message msgs[2];
    std::memset(msgs, 0, sizeof(msgs));
    msgs[0].anims[0].key = 0.0f; msgs[0].anims[0].anim = 11; msgs[0].anims[1].key = 0.5f; msgs[0].anims[1].anim = 12; msgs[0].animCount = 2;
    msgs[0].atts[0].key = 0.01f; msgs[0].atts[0].attitude = 4; msgs[0].attCount = 1;
    msgs[0].cams[0].key = 0.0f; msgs[0].cams[0].camera = 7; msgs[0].cams[1].key = 0.9f; msgs[0].cams[1].camera = 8; msgs[0].camCount = 2;

    Action a; Reset(a, &speaker, &listener, &talk, &subs, msgs, 2);
    a.NextAnimationTagIndex = 0; a.NextAttitudeTagIndex = 0; a.NextCameraTagIndex = 0;
    // frame 1: speech still pending (playing, not loaded): no timer, no counter
    a.Update();
    CHECK(!a.SoundStartedPlaying && a.UpdateCounter == 0 && a.LookingAtListener);
    CHECK(lookA.lookAtCalls == 1 && lookA.lastPos.x == 4 && lookB.lookAtCalls == 0);
    // frame 2: loaded and playing -> timer starts, first animation and camera tags fire
    talk.loadedAndPlaying = true; g_time = 10.0;
    a.Update();
    CHECK(a.SoundStartedPlaying && a.TimeCurrentLineStarted == 10.0 && subs.updateTimer && a.UpdateCounter == 1);
    CHECK(speaker.actions == 1 && speaker.lastKind == 1 && speaker.lastAnim == 11 && speaker.lastPriority == 1);
    CHECK(a.NextAnimationTagIndex == 1);
    CHECK(talk.attitude == 4 && a.NextAttitudeTagIndex == -1);
    CHECK(a.setUpCameraCalls == 1 && a.lastCamera == 7 && a.NextCameraTagIndex == 1 && a.CameraTagsWereUsed);
    // run to counter 50: the second animation tag
    while (a.UpdateCounter < 50) a.Update();
    CHECK(speaker.actions == 2 && speaker.lastAnim == 12 && speaker.lastPriority == 2 && a.NextAnimationTagIndex == -1);
    // the camera tag in effect (the previous one) re-sets after ten frames once its setup positions moved
    a.cameraMoved = true;
    for (int i = 0; i < 11; ++i) a.Update();
    CHECK(a.setUpCameraCalls == 2 && a.lastCamera == 7 && a.CameraToBeMovedCount == 0);
    // time out while the subtitle waits for a click: nothing; then the next line
    subs.waiting = true; g_time = 20.0;
    long counter = a.UpdateCounter;
    a.Update();
    CHECK(a.setUpTextCalls == 0 && subs.clears == 0 && a.UpdateCounter == counter + 1);
    subs.waiting = false;
    a.Update();
    CHECK(a.setUpTextCalls == 1 && subs.clears == 1 && talk.stops == 1 && a.CurrentMessageIndex == 1 && !a.terminated);
    // last line skipped -> terminate
    a.SkippedCurrentLine = true;
    a.Update();
    CHECK(a.terminated && subs.clears == 2 && talk.stops == 2);

    // no talk TC: attitude tags become conversation animations; gestures fire when idle
    Action b; Reset(b, &speaker, &listener, 0, &subs, msgs, 1);
    speaker.actions = 0;
    b.NextAttitudeTagIndex = 0; b.FramesUntilNextGesture = 1; b.LastActionPriority = 5;
    b.Update();
    CHECK(b.SoundStartedPlaying && b.UpdateCounter == 1);
    CHECK(speaker.actions == 2 && speaker.lastKind == 3 && speaker.lastAnim == 4 && speaker.lastPriority == 6);
    CHECK(b.LastGestureAnimIndex == 77 && b.FramesUntilNextGesture == 45);
    // default camera fallback
    b.CurrentCameraTagIndex = 2; b.cameraMoved = true;
    b.Update();
    CHECK(b.CameraToBeMovedCount == 1);

    // no target: look-at skipped, the action terminates once the line runs
    Action c; Reset(c, &speaker, 0, 0, &subs, msgs, 1);
    c.Update();
    CHECK(c.terminated);

    if (g_fail) { std::printf("00716de0_TEST FAIL\n"); return 1; }
    std::printf("00716de0_TEST PASS\n");
    return 0;
}