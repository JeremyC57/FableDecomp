// Behaviour test for NScript::CQ_FireHeartScript::CFH_Prisoner::Main (0x00D3A4A0): a copy of the Main logic over a mock parent script;
// checks the scripting start, the round wait with comments, the released branch (speech line, camera name, suns down, history false)
// and the killed branch (cutscene, suns up, history true), the round advance, the CheckCompletion thread and the removal.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>

struct Parent {
    int PuzzleRound, Guest, ProphetsScripted, FreedProphets, KilledProphets, StartNumberOfSuns;
    bool ProphetsReleased[4], ProphetsKilled[4], ShouldProphetSpeak[4], KilledAllPrisoners, FreedAllPrisoners;
    int MoralityChangeOnFreed, MoralityChangeOnKilled, morality, historyCalls; bool lastHistory;
    std::map<std::string, std::string> csargs; std::string lastCutscene, lastThread; int frames, startTries, comments, roundComments, removed, quit;
    bool released[4];
};

struct Prisoner {
    Parent* P; int MyCageNumber; bool Chosen; int releaseAt, killAt, terminateAt;
    bool T() const { return terminateAt && P->frames >= terminateAt; }
    void NSF() { ++P->frames; if (P->frames == 4) P->PuzzleRound = MyCageNumber; if (releaseAt && P->frames == releaseAt) P->ProphetsReleased[MyCageNumber] = true;
                 if (killAt && P->frames == killAt) P->ProphetsKilled[MyCageNumber] = true; P->ShouldProphetSpeak[MyCageNumber] = (P->frames == 3 || P->frames == 6); }
    bool StartScripting() { return ++P->startTries >= 2; }
    void MakeComment() { ++P->comments; }
    void MakeRoundStartComment() { ++P->roundComments; }
    void GetEndSpeech(std::string& s) const { s = "FIREHEART_PRISONER_LINE"; }
    void GetEndCutscene(std::string& s) const { s = "CS_FIREHEART_KILL"; }
    void Main()
    {
        ++P->frames; if (T()) return;
        P->released[MyCageNumber] = true;
        while (!StartScripting()) { NSF(); if (T()) return; }
        if (T()) return;
        P->ProphetsScripted++;
        while (P->PuzzleRound < MyCageNumber) { NSF(); if (T()) return; if (P->ShouldProphetSpeak[MyCageNumber]) { if (T()) return; MakeComment(); } }
        if (T()) return;
        P->Guest = MyCageNumber; MakeRoundStartComment();
        while (!Chosen) {
            NSF(); if (T()) return;
            if (P->ProphetsReleased[MyCageNumber]) {
                if (T()) return;
                GetEndSpeech(P->csargs["$LINE"]);
                char n[16]; std::sprintf(n, "%d", MyCageNumber); P->csargs["$CAMERA"] = std::string("CAM_FH_PRIS") + n;
                P->lastCutscene = "CS_FIREHEART_RELEASE"; P->FreedProphets++; P->StartNumberOfSuns--;
                ++P->historyCalls; P->lastHistory = false; Chosen = true; P->KilledAllPrisoners = false; P->morality += P->MoralityChangeOnFreed;
            } else if (P->ProphetsKilled[MyCageNumber]) {
                if (T()) return;
                std::string cutscene(""); GetEndCutscene(cutscene); P->lastCutscene = cutscene; P->KilledProphets++; P->StartNumberOfSuns++;
                ++P->historyCalls; P->lastHistory = true; Chosen = true; P->FreedAllPrisoners = false; P->morality += P->MoralityChangeOnKilled;
            } else if (P->ShouldProphetSpeak[MyCageNumber]) { if (T()) return; MakeComment(); }
        }
        if (T()) return;
        P->PuzzleRound++;
        P->lastThread = std::string("ParentClass.") + std::string("CheckCompletion");
        ++P->removed; ++P->quit;
    }
};

static int Fail(int n) { std::printf("00d3a4a0_TEST FAIL %d\n", n); return 1; }
static void Reset(Parent& p) { std::memset(&p.PuzzleRound, 0, (char*)&p.csargs - (char*)&p.PuzzleRound); p.csargs.clear(); p.lastCutscene = p.lastThread = "";
    p.frames = p.startTries = p.comments = p.roundComments = p.removed = p.quit = 0; p.StartNumberOfSuns = 3; p.MoralityChangeOnFreed = 5; p.MoralityChangeOnKilled = -7;
    p.KilledAllPrisoners = p.FreedAllPrisoners = true; for (int i = 0; i < 4; ++i) p.released[i] = false; }

int main()
{
    Parent p; Reset(p);
    Prisoner r; r.P = &p; r.MyCageNumber = 2; r.Chosen = false; r.releaseAt = 7; r.killAt = 0; r.terminateAt = 0;
    r.Main();
    if (p.ProphetsScripted != 1 || p.Guest != 2 || p.roundComments != 1 || p.comments != 2 || p.csargs["$LINE"] != "FIREHEART_PRISONER_LINE") return Fail(1);
    if (p.csargs["$CAMERA"] != "CAM_FH_PRIS2" || p.lastCutscene != "CS_FIREHEART_RELEASE" || p.FreedProphets != 1 || p.StartNumberOfSuns != 2) return Fail(2);
    if (p.historyCalls != 1 || p.lastHistory || p.KilledAllPrisoners || !p.FreedAllPrisoners || p.morality != 5 || p.PuzzleRound != 3) return Fail(3);
    if (p.lastThread != "ParentClass.CheckCompletion" || p.removed != 1 || p.quit != 1) return Fail(4);
    Reset(p); Prisoner k; k.P = &p; k.MyCageNumber = 1; k.Chosen = false; k.releaseAt = 0; k.killAt = 7; k.terminateAt = 0;
    k.Main();
    if (p.lastCutscene != "CS_FIREHEART_KILL" || p.KilledProphets != 1 || p.StartNumberOfSuns != 4 || !p.lastHistory || p.FreedAllPrisoners || !p.KilledAllPrisoners) return Fail(5);
    if (p.morality != -7 || p.PuzzleRound != 2 || p.removed != 1) return Fail(6);
    Reset(p); Prisoner t; t.P = &p; t.MyCageNumber = 3; t.Chosen = false; t.releaseAt = 0; t.killAt = 0; t.terminateAt = 4;
    t.Main();
    if (p.removed != 0 || p.lastThread != "" || p.PuzzleRound != 3 || p.Guest != 0 || p.comments != 1) return Fail(7);
    std::printf("00d3a4a0_TEST PASS\n");
    return 0;
}
