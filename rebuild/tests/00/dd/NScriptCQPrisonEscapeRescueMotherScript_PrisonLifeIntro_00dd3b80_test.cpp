// Behaviour test for NScript::CQ_PrisonEscapeRescueMotherScript::PrisonLifeIntro (0x00DD3B80): a copy of the control flow
// over a recording stand-in game interface (waits, quest start, the two conversations, the YearsPassedInCell / Whisper
// branches, the terminate exits and the final PrisonCutsceneFinished store).
#include <cstdio>
#include <cstring>

struct Game {
    int levelWait;          // frames until PrisonCells_1 is loaded
    int scriptWait;         // frames until StartScriptingEntity succeeds
    int convFrames;         // frames each conversation stays active
    int frames;
    int terminateAt;        // terminate once frames reaches this (-1 never)
    int lines;
    int conversations;
    int lastConv;
    int convActiveLeft;
    char line8[64];
    bool kickedOff;
    bool movedGuard;
    int people;
    void NewScriptFrame() { ++frames; if (convActiveLeft > 0) --convActiveLeft; }
    bool IsLevelLoaded() { return frames >= levelWait; }
    bool StartScriptingEntity() { return frames >= scriptWait; }
    int AddNewConversation() { ++conversations; convActiveLeft = convFrames; return lastConv = 100 + conversations; }
    void AddPerson() { ++people; }
    void AddLine(int conv, const char* text) { if (conv == lastConv) ++lines; if (std::strstr(text, "_08")) std::strcpy(line8, text); }
    bool IsConversationActive(int) { return convActiveLeft > 0; }
};

struct Script {
    Game* G;
    bool MotherRescued, WhisperKilled, PrisonCutsceneFinished;
    long YearsPassedInCell;
    long PrisonerConversationIndex;
    bool T() { return G->terminateAt >= 0 && G->frames >= G->terminateAt; }
    void PrisonLifeIntro();
};

void Script::PrisonLifeIntro()
{
    if (MotherRescued) { if (T()) return; return; }
    while (!G->IsLevelLoaded()) { G->NewScriptFrame(); if (T()) return; }
    if (T()) return;
    G->kickedOff = true;
    while (!G->StartScriptingEntity()) { G->NewScriptFrame(); if (T()) return; }
    if (T()) return;
    while (!G->StartScriptingEntity()) { G->NewScriptFrame(); if (T()) return; }
    if (T()) return;
    long conv = G->AddNewConversation();
    G->AddPerson(); G->AddPerson();
    G->AddLine(conv, "01"); G->AddLine(conv, "02"); G->AddLine(conv, "03"); G->AddLine(conv, "04");
    if (YearsPassedInCell == 0) { if (T()) return; G->AddLine(conv, "04_B"); }
    while (G->IsConversationActive(conv)) { G->NewScriptFrame(); if (T()) return; }
    if (T()) return;
    G->movedGuard = true;
    PrisonerConversationIndex = conv = G->AddNewConversation();
    G->AddPerson();
    G->AddLine(conv, "05"); G->AddLine(conv, "06"); G->AddLine(conv, "07");
    char text[64]; std::strcpy(text, "TEXT_QST_055_PRISON_INTRO_08"); std::strcat(text, "_HERO");
    G->AddLine(conv, text);
    if (WhisperKilled) { if (T()) return; G->AddLine(conv, "09_A"); }
    else { if (T()) return; G->AddLine(conv, "09_B"); }
    for (int i = 10; i <= 15; ++i) G->AddLine(conv, "1x");
    while (G->IsConversationActive(conv)) { G->NewScriptFrame(); if (T()) return; }
    if (T()) return;
    PrisonCutsceneFinished = true;
}

static bool run(bool rescued, long years, int terminateAt, Script& s, Game& g)
{
    std::memset(&g, 0, sizeof(g));
    g.levelWait = 3; g.scriptWait = 4; g.convFrames = 2; g.terminateAt = terminateAt;
    s.G = &g; s.MotherRescued = rescued; s.WhisperKilled = false; s.PrisonCutsceneFinished = false;
    s.YearsPassedInCell = years; s.PrisonerConversationIndex = 0;
    s.PrisonLifeIntro();
    return s.PrisonCutsceneFinished;
}

int main()
{
    Script s; Game g;
    if (run(true, 0, -1, s, g) || g.kickedOff) { std::printf("00dd3b80_TEST FAIL\n"); return 1; }
    if (!run(false, 0, -1, s, g) || g.lines != 16 || g.conversations != 2 || s.PrisonerConversationIndex != 102
        || std::strcmp(g.line8, "TEXT_QST_055_PRISON_INTRO_08_HERO") != 0 || !g.movedGuard) { std::printf("00dd3b80_TEST FAIL\n"); return 1; }
    run(false, 2, -1, s, g);
    if (!s.PrisonCutsceneFinished || g.conversations != 2) { std::printf("00dd3b80_TEST FAIL\n"); return 1; }
    if (run(false, 0, 1, s, g) || g.kickedOff) { std::printf("00dd3b80_TEST FAIL\n"); return 1; }
    if (run(false, 0, 5, s, g) || !g.kickedOff || g.movedGuard) { std::printf("00dd3b80_TEST FAIL\n"); return 1; }
    std::printf("00dd3b80_TEST PASS\n");
    return 0;
}