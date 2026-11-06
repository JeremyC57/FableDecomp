// Behaviour test for NScript::CV_GuildMasterScript::GetGuildMasterSpeech (0x00E91F20): the gameflow-position line, the three
// two-way choices (quest completed / quest active / SeenAbbeyMotherAtGuild), the hero-souls if-chain, the empty default, the
// terminate path returning the empty default, and the one-in-four swap to an unsaid info line when the line repeats.
#include <cstdio>
#include <string>
typedef std::string Str;
enum { EGP_TRAINING = 100, EGP_SECOND_MAZE_MEETING = 600, EGP_MAZE_TELEPORT_TO_WW = 800, EGP_HOOK_COAST = 1300,
       EGP_THE_ORACLE = 2300, EGP_SCYTHE_MEETING = 2400, EGP_HERO_SOULS = 2500, EGP_FIRE_HEART = 1900 };
struct Model {
    long Position; bool SeenAbbeyMother; long SoulsPosition;
    bool QuestCompleted, QuestActive, Terminating; long Title; int RandValue;
    bool Dialogue[5]; Str Last;
    Model() : Position(0), SeenAbbeyMother(false), SoulsPosition(0), QuestCompleted(false), QuestActive(false),
              Terminating(false), Title(0), RandValue(1), Last("") { for (int i = 0; i < 5; ++i) Dialogue[i] = false; }
    Str Speech()
    {
        Str nothing = Str("");
        Str speech;
        switch (Position) {
        case EGP_TRAINING: speech = "TEXT_QST_081_TRAINING"; break;
        case EGP_SECOND_MAZE_MEETING:
            if (QuestCompleted) { if (Terminating) return nothing; speech = "TEXT_QST_081_BANDIT_CAMP"; }
            else { if (Terminating) return nothing; speech = "TEXT_QST_081_SECOND_MAZE_MEETING"; }
            break;
        case EGP_MAZE_TELEPORT_TO_WW:
            if (QuestActive) { if (Terminating) return nothing; speech = "TEXT_QST_081_TROPHY_DEALER"; }
            else { if (Terminating) return nothing; speech = "TEXT_QST_081_MAZE_TELEPORT_TO_WW"; }
            break;
        case EGP_HOOK_COAST:
            if (SeenAbbeyMother) { if (Terminating) return nothing; speech = "TEXT_QST_081_HOOK_COAST"; }
            else { if (Terminating) return nothing; speech = "TEXT_QST_081_BEFORE_HOOK_COAST"; }
            break;
        case EGP_THE_ORACLE: speech = "TEXT_QST_081_THE_ORACLE"; break;
        case EGP_SCYTHE_MEETING: speech = "TEXT_QST_081_THE_ORACLE"; break;
        case EGP_HERO_SOULS:
            if (SoulsPosition == 0) { if (Terminating) return nothing; speech = "TEXT_QST_081_HERO_SOULS"; }
            else if (SoulsPosition == 7) { if (Terminating) return nothing; speech = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE"; }
            else if (SoulsPosition >= 1 && SoulsPosition <= 6) { if (Terminating) return nothing; speech = "TEXT_QST_081_HERO_SOULS_N"; }
            else { if (Terminating) return nothing; return ""; }
            break;
        default:
            return "";
        }
        if (Last == speech) {
            if (RandValue % 4 == 0) {
                if (!Dialogue[2]) { Dialogue[2] = true; speech = "TEXT_QST_081_INFO_TELEPORTERS"; }
                else if (!Dialogue[3]) { Dialogue[3] = true; speech = "TEXT_QST_081_INFO_WEAPONS"; }
                else if (!Dialogue[4]) { Dialogue[4] = true; speech = "TEXT_QST_081_INFO_LOG_BOOK"; }
                else if (Title == 19 && !Dialogue[1]) { Dialogue[1] = true; speech = "TEXT_QST_081_INFO_HERO_TITLE"; }
            }
        }
        Last = speech;
        return speech;
    }
};
int main()
{
    int fails = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); ++fails; } } while (0)
    Model m;
    m.Position = EGP_TRAINING;
    CHECK(m.Speech() == "TEXT_QST_081_TRAINING");
    CHECK(m.Last == "TEXT_QST_081_TRAINING");
    m.RandValue = 3;                                   // repeat, rand % 4 != 0: no swap
    CHECK(m.Speech() == "TEXT_QST_081_TRAINING");
    m.RandValue = 8;                                   // repeat, rand % 4 == 0: the info lines in order
    CHECK(m.Speech() == "TEXT_QST_081_INFO_TELEPORTERS");
    CHECK(m.Speech() == "TEXT_QST_081_TRAINING");       // last was the info line: not a repeat
    CHECK(m.Speech() == "TEXT_QST_081_INFO_WEAPONS");
    CHECK(m.Speech() == "TEXT_QST_081_TRAINING");
    CHECK(m.Speech() == "TEXT_QST_081_INFO_LOG_BOOK");
    CHECK(m.Speech() == "TEXT_QST_081_TRAINING");
    CHECK(m.Speech() == "TEXT_QST_081_TRAINING");       // all three said, title not 19: the repeat stands
    m.Title = 19;
    CHECK(m.Speech() == "TEXT_QST_081_INFO_HERO_TITLE");
    CHECK(m.Dialogue[1] && m.Dialogue[2] && m.Dialogue[3] && m.Dialogue[4] && !m.Dialogue[0]);
    Model q;
    q.Position = EGP_SECOND_MAZE_MEETING; CHECK(q.Speech() == "TEXT_QST_081_SECOND_MAZE_MEETING");
    q.QuestCompleted = true; CHECK(q.Speech() == "TEXT_QST_081_BANDIT_CAMP");
    q.Position = EGP_MAZE_TELEPORT_TO_WW; CHECK(q.Speech() == "TEXT_QST_081_MAZE_TELEPORT_TO_WW");
    q.QuestActive = true; CHECK(q.Speech() == "TEXT_QST_081_TROPHY_DEALER");
    q.Position = EGP_HOOK_COAST; CHECK(q.Speech() == "TEXT_QST_081_BEFORE_HOOK_COAST");
    q.SeenAbbeyMother = true; CHECK(q.Speech() == "TEXT_QST_081_HOOK_COAST");
    q.Position = EGP_SCYTHE_MEETING; CHECK(q.Speech() == "TEXT_QST_081_THE_ORACLE");
    q.Position = EGP_HERO_SOULS; q.SoulsPosition = 7; CHECK(q.Speech() == "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE");
    Str before = q.Last;
    q.SoulsPosition = 8; CHECK(q.Speech() == ""); CHECK(q.Last == before);     // past the end: "" and no remember
    q.Position = EGP_FIRE_HEART; CHECK(q.Speech() == ""); CHECK(q.Last == before);  // not a case: ""
    q.Position = EGP_HOOK_COAST; q.Terminating = true; CHECK(q.Speech() == ""); CHECK(q.Last == before);
    q.Position = EGP_TRAINING; CHECK(q.Speech() == "TEXT_QST_081_TRAINING"); // plain cases have no terminate check
    if (fails == 0) std::printf("00e91f20_TEST PASS\n");
    return fails;
}