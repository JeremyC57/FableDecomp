// Behaviour test for CPreMeleeWhisper::Main (0x00D524A0): the per-frame step of the pre-melee guide -- answer a hero chat, and
// when she and the hero are both within 7 units of a numbered PreMeleeChatMarker (height difference <= 1, chat timer <= 5)
// remove every marker carrying that number, re-arm the chat timer to 10 and pick the guide line by the marker number.
#include <cstdio>
#include <cstring>
#include <cmath>

struct Marker { long Number; bool Removed; };

static const char* GuideLine(long number)
{
    switch (number) {
    case 2: return "LIBRARY";
    case 4: return "SHOP";
    case 5: return "CLOISTERS";
    case 6: return "GRAVE1";
    case 7: return "GRAVE2";
    case 8: return "GRAVE3";
    case 9: return "GRAVE4";
    case 10: return "MAZE";
    case 11: return "WILL";
    case 13: return "WOODS";
    case 14: return "SKILL";
    case 15: return "SERVANTS";
    case 16: return "MAIN_DORM";
    case 19: return "DOOR";
    case 20: return "DINING_ROOM";
    }
    return 0;
}

struct Whisper {
    long ChatTimer, Chats, Lines;
    const char* LastLine;
    void Step(bool talked, Marker* markers, long count, long nearest, float myDist, float heroDist, float heightDiff)
    {
        if (talked) ++Chats;
        if (myDist < 7.0f && heroDist < 7.0f && ChatTimer <= 5 && std::fabs(heightDiff) <= 1.0f) {
            long number = markers[nearest].Number;
            for (long i = 0; i < count; ++i)
                if (markers[i].Number == number) markers[i].Removed = true;
            ChatTimer = 10;
            const char* line = GuideLine(number);
            if (line) { LastLine = line; ++Lines; }
        }
        if (ChatTimer > 0) --ChatTimer;
    }
};

int main()
{
    Marker m[4] = { { 2, false }, { 5, false }, { 2, false }, { 3, false } };
    Whisper w = { 0, 0, 0, 0 };
    bool ok = true;
    w.Step(true, m, 4, 0, 8.0f, 2.0f, 0.0f);
    ok = ok && w.Chats == 1 && w.Lines == 0 && !m[0].Removed;
    w.Step(false, m, 4, 0, 3.0f, 2.0f, 0.5f);
    ok = ok && m[0].Removed && m[2].Removed && !m[1].Removed && w.Lines == 1 && std::strcmp(w.LastLine, "LIBRARY") == 0 && w.ChatTimer == 9;
    w.Step(false, m, 4, 1, 3.0f, 2.0f, 0.0f);       // timer still above 5: no line
    ok = ok && !m[1].Removed && w.Lines == 1;
    w.ChatTimer = 0;
    w.Step(false, m, 4, 1, 3.0f, 2.0f, 2.0f);       // height difference too large
    ok = ok && !m[1].Removed;
    w.Step(false, m, 4, 3, 3.0f, 2.0f, 0.0f);       // number 3 has no guide line, but its marker still goes
    ok = ok && m[3].Removed && w.Lines == 1 && w.ChatTimer == 9;
    w.ChatTimer = 0;
    w.Step(false, m, 4, 1, 1.0f, 1.0f, -1.0f);
    ok = ok && m[1].Removed && std::strcmp(w.LastLine, "CLOISTERS") == 0 && w.Lines == 2;
    if (!ok) { std::printf("00d524a0_TEST FAIL\n"); return 1; }
    std::printf("00d524a0_TEST PASS\n");
    return 0;
}