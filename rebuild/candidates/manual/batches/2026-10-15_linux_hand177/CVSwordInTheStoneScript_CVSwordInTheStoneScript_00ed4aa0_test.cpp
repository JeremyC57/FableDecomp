#include <cstdio>
// Behaviour model of CV_SwordInTheStoneScript's ctor: data/interface words stored, both CScriptThing handles empty, the
// timer index taken from the global interface's RegisterTimer.
struct Iface { long next; long RegisterTimer() { return next++; } };
static Iface g_iface = { 7 };
struct CTimer { long Index; CTimer() : Index(g_iface.RegisterTimer()) {} };
struct Words { void* Data; void* Info; Words() : Data(0), Info(0) {} };
struct Thing { Words PImp; };
struct Script {
    void* Game; void* Data;
    Thing Current; Thing Null; CTimer Timer;
    Script(void* data, void* game) : Game(game), Data(data) {}
};
int main()
{
    int a, b;
    Script s(&a, &b);
    if (s.Data != &a || s.Game != &b || s.Current.PImp.Data || s.Current.PImp.Info || s.Null.PImp.Info || s.Timer.Index != 7) {
        std::printf("00ed4aa0_TEST FAIL\n"); return 1;
    }
    Script t(0, 0);
    if (t.Timer.Index != 8) { std::printf("00ed4aa0_TEST FAIL\n"); return 1; }
    std::printf("00ed4aa0_TEST PASS\n");
    return 0;
}
