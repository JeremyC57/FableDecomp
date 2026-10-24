// Behaviour test for 0064a4e9: NPlayerGui::CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer*, long, float) --
// the base element is built with the scale, the timer's index and the graphic are stored.
#include <cstdio>

namespace NPlayerGui {
struct CTimer {
    long TimerIndex;
};
static int g_baseBuilt = 0;
struct CQuestInfoElement {
    virtual ~CQuestInfoElement() { }
    long  ID;
    long  ScriptID;
    float Scale;
    CQuestInfoElement(float scale) : ID(0), ScriptID(-1), Scale(scale) { ++g_baseBuilt; }
};
struct CQuestInfoElementTimer : CQuestInfoElement {
    long TimerIndex;
    long Graphic;
    CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale);
};
CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale)
    : CQuestInfoElement(scale), Graphic(graphic)
{
    TimerIndex = timer->TimerIndex;
}
}

int main()
{
    NPlayerGui::CTimer timer;
    timer.TimerIndex = 31;
    NPlayerGui::CQuestInfoElementTimer element(&timer, 0x1234, 0.75f);
    if (NPlayerGui::g_baseBuilt != 1 || element.Scale != 0.75f || element.TimerIndex != 31 || element.Graphic != 0x1234 ||
        element.ScriptID != -1) {
        std::printf("0064a4e9_TEST FAIL\n");
        return 1;
    }
    timer.TimerIndex = 32;                  // the element keeps the index it copied, not a pointer to the timer
    if (element.TimerIndex != 31) { std::printf("0064a4e9_TEST FAIL\n"); return 1; }
    std::printf("0064a4e9_TEST PASS\n");
    return 0;
}