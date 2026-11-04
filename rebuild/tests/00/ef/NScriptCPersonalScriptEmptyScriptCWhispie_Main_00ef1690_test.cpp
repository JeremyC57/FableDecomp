// Behaviour test for NScript::CPersonalScript_EmptyScript::CWhispie::Main 00ef1690: a model of the button tutorial -- after
// sheathing, each of five inputs must go high then low, with one screen message on each edge; termination ends the thread early.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
struct Pad { int frame; int stopAt; bool sheathed; std::vector<std::string> messages; };
static bool Input(const Pad& p, int which) { int t = p.frame - 3 - which * 4; return t >= 0 && t < 2; }
static bool Frame(Pad& p) { ++p.frame; return p.frame >= p.stopAt; }
static const char* const kPressed[5] = {
    "You pressed the unsheathe ranged weapon button!", "You pressed the first person targeting button!",
    "You pressed the lock target button!", "You pressed the fire ranged weapon button!", "You're in projectile weapon mode!" };
static void Main(Pad& p)
{
    if (Frame(p)) return;
    p.sheathed = true;
    for (;;) {
        for (int i = 0; i < 5; ++i) {
            while (!Input(p, i)) if (Frame(p)) return;
            p.messages.push_back(kPressed[i]);
            while (Input(p, i)) if (Frame(p)) return;
            p.messages.push_back(i == 4 ? "Now you're not!" : "You let go!");
        }
        if (Frame(p)) return;
        if (p.frame > 40) return;   // the model's inputs fire once; stop instead of waiting forever
    }
}
int main()
{
    Pad p = { 0, 1000, false, std::vector<std::string>() };
    Main(p);
    if (!p.sheathed || p.messages.size() != 10 || p.messages[1] != "You let go!" || p.messages[9] != "Now you're not!") {
        std::printf("00ef1690_TEST FAIL\n"); return 1;
    }
    Pad t = { 0, 1, false, std::vector<std::string>() };
    Main(t);
    if (t.sheathed || !t.messages.empty()) { std::printf("00ef1690_TEST FAIL\n"); return 1; }
    std::printf("00ef1690_TEST PASS\n");
    return 0;
}