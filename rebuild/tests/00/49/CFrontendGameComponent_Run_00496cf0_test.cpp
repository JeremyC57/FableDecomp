// Behaviour test for CFrontendGameComponent::Run @ 0x00496CF0: pump the engine until it reports quit (2), running
// input/output on each frame (1); then hand over to a new frontend (release front end) or a main game component
// seeded with the world/save to load and the joystick, recording the outgoing frontend; otherwise return false.
#include <cstdio>
#include <string>
#include <vector>
struct Next { int kind; std::string world, save; int joystick; };
struct Frontend {
    std::vector<int> updates; int frames; bool releaseFrontEnd, createGame; std::string world, save; int joystick;
    bool shutdown;
    bool Run(Next& next, const Frontend*& last)
    {
        size_t i = 0; bool quit = false;
        while (!quit) {
            int r = i < updates.size() ? updates[i++] : 2;
            if (r == 2) quit = true; else if (r == 1) ++frames;
        }
        shutdown = true;
        if (releaseFrontEnd) { next.kind = 1; last = this; return true; }
        if (createGame) { next.kind = 2; next.world = world; next.save = save; next.joystick = joystick; last = this; return true; }
        last = this;
        return false;
    }
};
int main()
{
    Frontend f; f.updates.push_back(1); f.updates.push_back(0); f.updates.push_back(1); f.updates.push_back(2);
    f.frames = 0; f.releaseFrontEnd = false; f.createGame = true; f.world = "Oakvale"; f.save = "slot1"; f.joystick = 3; f.shutdown = false;
    Next n; n.kind = 0; n.joystick = -1; const Frontend* last = 0;
    if (!f.Run(n, last) || n.kind != 2 || n.world != "Oakvale" || n.save != "slot1" || n.joystick != 3 || f.frames != 2 || last != &f) { std::printf("00496cf0_TEST FAIL\n"); return 1; }
    Frontend g = f; g.updates.clear(); g.createGame = false; g.frames = 0; Next m; m.kind = 0;
    if (g.Run(m, last) || m.kind != 0 || !g.shutdown || g.frames != 0) { std::printf("00496cf0_TEST FAIL\n"); return 1; }
    Frontend h = g; h.releaseFrontEnd = true; h.createGame = true;
    if (!h.Run(m, last) || m.kind != 1) { std::printf("00496cf0_TEST FAIL\n"); return 1; }
    std::printf("00496cf0_TEST PASS\n");
    return 0;
}