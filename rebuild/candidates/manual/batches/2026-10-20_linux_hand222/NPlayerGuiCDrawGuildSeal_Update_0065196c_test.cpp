// Behaviour test for 0x0065196C NPlayerGui::CDrawGuildSeal::Update (model copy of the control flow).
#include <cstdio>

struct Hero { bool alive, child, hasXp, ready; };
static int g_messagesAdded = 0, g_displayed = 0;
struct Seal {
    bool worldA, worldB, ready, blockGui, blockScreen; long frame, endFrame; int queued; Hero* hero;
    void Update()
    {
        if (worldB && worldA) {
            if (hero->alive && !hero->child && hero->hasXp) {
                bool r = hero->ready;
                if (r != ready) { ready = r; if (r) { ++g_messagesAdded; ++queued; } }
                goto messages;
            }
        }
        ready = false;
    messages:
        if (frame > endFrame && !blockGui && !blockScreen && queued != 0) { ++g_displayed; --queued; }
    }
};

int main()
{
    Hero h = { true, false, true, true };
    Seal s = { true, true, false, false, false, 10, 5, 0, &h };
    s.Update();
    if (!s.ready || g_messagesAdded != 1 || g_displayed != 1) { std::printf("0065196c_TEST FAIL\n"); return 1; }
    s.Update();                                         // no edge: nothing new
    if (g_messagesAdded != 1) { std::printf("0065196c_TEST FAIL\n"); return 1; }
    h.child = true; s.Update();                         // a child hero clears the state
    if (s.ready) { std::printf("0065196c_TEST FAIL\n"); return 1; }
    std::printf("0065196c_TEST PASS\n");
    return 0;
}
