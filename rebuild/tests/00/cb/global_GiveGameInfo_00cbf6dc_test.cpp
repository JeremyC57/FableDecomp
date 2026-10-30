// Behaviour test for 0x00CBF6DC GiveGameInfo(info): start a movie sequence, pause non-scripted entities for the scope, and
// for each string ask a hero yes/no question with the TEXT_GUI_SKIP answer, waiting frame by frame until it is answered or
// the script runner stops; the pause is lifted on exit.
#include <cstdio>
#include <cstring>

struct Script {
    int movies, frames, questions, unpaused, answerAfter, waited; const char* lastQ; const char* lastA1; bool stop;
    void StartMovieSequence() { ++movies; }
    void NewScriptFrame() { ++frames; }
    void GiveHeroYesNoQuestion(const char* q, const char* a1) { ++questions; lastQ = q; lastA1 = a1; waited = 0; }
    int MsgIsQuestionAnsweredYesOrNo() { return waited++ < answerAfter ? -1 : 1; }
    void PauseAllNonScriptedEntities(bool on) { if (!on) ++unpaused; }
};
struct Pause { Script* s; Pause(Script* p) : s(p) { } ~Pause() { s->PauseAllNonScriptedEntities(false); } };

static void GiveGameInfo(Script* g, const char* const* info, unsigned int count)
{
    g->StartMovieSequence();
    Pause pause(g);
    for (unsigned int i = 0; i < count; ++i) {
        g->NewScriptFrame();
        g->GiveHeroYesNoQuestion(info[i], "TEXT_GUI_SKIP");
        while (g->MsgIsQuestionAnsweredYesOrNo() < 0) {
            if (g->stop) break;
            g->NewScriptFrame();
        }
    }
}

static int fail() { std::printf("00cbf6dc_TEST FAIL\n"); return 1; }

int main()
{
    const char* info[2] = { "TEXT_INFO_A", "TEXT_INFO_B" };
    Script s; std::memset(&s, 0, sizeof(s)); s.answerAfter = 2;
    GiveGameInfo(&s, info, 2);
    if (s.movies != 1 || s.questions != 2 || s.unpaused != 1 || s.frames != 2 * (1 + 2)) return fail();
    if (std::strcmp(s.lastQ, "TEXT_INFO_B") != 0 || std::strcmp(s.lastA1, "TEXT_GUI_SKIP") != 0) return fail();
    std::memset(&s, 0, sizeof(s)); s.answerAfter = 5; s.stop = true;
    GiveGameInfo(&s, info, 2);                                          // stopping runner: one frame per question
    if (s.questions != 2 || s.frames != 2 || s.unpaused != 1) return fail();
    std::memset(&s, 0, sizeof(s));
    GiveGameInfo(&s, info, 0);
    if (s.movies != 1 || s.questions != 0 || s.unpaused != 1) return fail();
    std::printf("00cbf6dc_TEST PASS\n");
    return 0;
}