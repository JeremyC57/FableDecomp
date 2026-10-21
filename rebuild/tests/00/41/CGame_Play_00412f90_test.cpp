// Behaviour test for 00412f90 CGame::Play: component selection, start, and the run loop until a component reports no successor.
#include <cstdio>
struct Component {
    int kind; int runs; int started; Component* next;
    Component(int k) : kind(k), runs(0), started(0), next(0) {}
    void Start() { ++started; }
    bool Run(Component*& out) { ++runs; if (next) { out = next; return true; } return false; }
};
struct Game { Component* Current; bool Quit; };
static Component* Play(Game& g, bool startMain, bool legacy, bool compileOnly, Component* follow)
{
    Component* component = 0;
    if (startMain) { component = new Component(1); component->Start(); }
    else if (legacy) { component = new Component(2); component->Start(); }
    else { component = new Component(3); component->Start(); }
    if (compileOnly) { delete component; return 0; }
    if (component) g.Current = component;
    component->next = follow;
    while (!g.Quit) {
        bool running = g.Current->Run(component);
        if (!running) g.Quit = true; else g.Current = component;
    }
    return g.Current;
}
int main()
{
    Component tail(9);
    Game g = { 0, false };
    Component* last = Play(g, false, true, false, &tail);
    Game h = { 0, false };
    Component* none = Play(h, true, false, true, 0);
    if (last != &tail || !g.Quit || tail.runs != 1 || none != 0 || h.Current != 0) { std::printf("00412f90_TEST FAIL\n"); return 1; }
    std::printf("00412f90_TEST PASS\n");
    return 0;
}