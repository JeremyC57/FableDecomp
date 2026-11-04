// Behaviour test for NScript::CV_OracleScript::CVO_Woman::Main 0x00EBE620: a model frame loop. Talked to: the first time
// she speaks the on-talk line and remembers it (SaidIntro), afterwards the reminder line; hit by the hero (or by any of his
// special abilities except ability 14): the on-hit line, then she and the hero are made allies both ways.
#include <cstdio>
#include <cstring>
struct Frame { bool Talked; bool Hit; bool AnySpecial; bool Special14; };
struct Woman {
    bool SaidIntro; char Said[8][40]; int SaidCount; int AllyCalls;
    void Speak(const char* t) { std::strcpy(Said[SaidCount++], t); }
    void Run(const Frame* frames, int n)
    {
        for (int i = 0; i < n; ++i) {
            const Frame& f = frames[i];
            if (f.Talked) {
                if (!SaidIntro) { Speak("TEXT_QST_B20_WOMAN_ON_TALK"); SaidIntro = true; }
                else Speak("TEXT_QST_B20_WOMAN_REMINDER");
            }
            if (f.Hit || (f.AnySpecial && !f.Special14)) {
                Speak("TEXT_QST_B20_WOMAN_ON_HIT");
                AllyCalls += 2;
            }
        }
    }
};
int main()
{
    Frame frames[5] = { { true, false, false, false }, { true, false, false, false }, { false, false, true, true },
                        { false, false, true, false }, { false, true, false, false } };
    Woman w; w.SaidIntro = false; w.SaidCount = 0; w.AllyCalls = 0;
    w.Run(frames, 5);
    int ok = w.SaidCount == 4 && !std::strcmp(w.Said[0], "TEXT_QST_B20_WOMAN_ON_TALK")
          && !std::strcmp(w.Said[1], "TEXT_QST_B20_WOMAN_REMINDER") && !std::strcmp(w.Said[2], "TEXT_QST_B20_WOMAN_ON_HIT")
          && !std::strcmp(w.Said[3], "TEXT_QST_B20_WOMAN_ON_HIT") && w.AllyCalls == 4 && w.SaidIntro;
    std::printf(ok ? "00ebe620_TEST PASS\n" : "00ebe620_TEST FAIL\n");
    return ok ? 0 : 1;
}
