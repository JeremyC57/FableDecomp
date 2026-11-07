// Behaviour test for CTH_Whisper::Main (0x00EE3140): past gameflow position 2100 Whisper leaves at once; else, each frame, a hit
// (not a heal-life spell) or a talk drops any open conversation and the rumour and plays the leave / pub cutscene; between 1 and 5
// units she says TEXT_QST_B14_WHISPERPUB_<n> (n cycling 1..3 after the first line 0) every (rand()%30+30)*15 frames and re-faces
// the hero every 15 frames in between.
#include <cstdio>

struct Whisper {
    long Line, TalkTimer, FaceTimer, Lines[8], LineCount, Faces, Conversation;
    bool CutscenePlayed;
    long NextDelay(long r) { return (r % 30 + 30) * 15; }
    void Frame(bool hit, bool heal, bool talked, float distance, long r)
    {
        if (hit && !heal) { Conversation = -1; CutscenePlayed = true; }
        else if (talked) { Conversation = -1; CutscenePlayed = true; }
        else if (distance < 5.0f && distance > 1.0f) {
            if (TalkTimer <= 0) {
                if (Line > 3) Line = 1;
                ++Faces;
                Conversation = 7;
                Lines[LineCount++] = Line;
                ++Line;
                TalkTimer = NextDelay(r);
                FaceTimer = 15;
            }
            else if (FaceTimer <= 0) { ++Faces; FaceTimer = 15; }
        }
        --FaceTimer;
        --TalkTimer;
    }
};

int main()
{
    Whisper w = { 0, 0, 0, {0}, 0, 0, -1, false };
    bool ok = true;
    w.Frame(false, false, false, 3.0f, 0);   ok = ok && w.LineCount == 1 && w.Lines[0] == 0 && w.TalkTimer == 449 && w.FaceTimer == 14;
    for (int i = 0; i < 15; ++i) w.Frame(false, false, false, 3.0f, 0);
    ok = ok && w.Faces == 2 && w.LineCount == 1;
    w.TalkTimer = 0; w.Frame(false, false, false, 3.0f, 29); ok = ok && w.Lines[1] == 1 && w.TalkTimer == (29 + 30) * 15 - 1;
    w.TalkTimer = 0; w.Frame(false, false, false, 3.0f, 0); w.TalkTimer = 0; w.Frame(false, false, false, 3.0f, 0);
    w.TalkTimer = 0; w.Frame(false, false, false, 3.0f, 0); ok = ok && w.Lines[4] == 1;
    w.Frame(true, true, false, 3.0f, 0);     ok = ok && !w.CutscenePlayed;
    w.Frame(true, false, false, 3.0f, 0);    ok = ok && w.CutscenePlayed && w.Conversation == -1;
    if (!ok) { std::printf("00ee3140_TEST FAIL\n"); return 1; }
    std::printf("00ee3140_TEST PASS\n");
    return 0;
}
