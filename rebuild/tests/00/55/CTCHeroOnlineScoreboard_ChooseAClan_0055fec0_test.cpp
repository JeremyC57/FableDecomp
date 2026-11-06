// Behaviour test for CTCHeroOnlineScoreboard::ChooseAClan (0x0055FEC0): a plain model of the control flow -- clear the
// logo cache, hide the connecting message, bail without an inventory, reset the state off the scoreboard screen (8),
// otherwise build one menu entry (back action 274, select action 3014 with the clan name, name / description / logo
// children) per clan, register the list, wire the arrows and clickables, initialise and show it (state 5), reset the state.
#include <cstdio>

struct Entry { int back_action; int select_action; int select_params; int children; };
struct Model {
    int cache_size; int connecting_state; bool has_inventory; int open_menu; int state;
    int clans; Entry entries[8]; int entry_count; int registered; int observed; int list_state; bool initialised;
};

static void ChooseAClan(Model& m)
{
    m.cache_size = 0;
    m.connecting_state = 0;
    if (!m.has_inventory)
        return;
    if (m.open_menu != 8) {
        m.state = 0;
        return;
    }
    m.entry_count = 0;
    for (int i = 0; i < m.clans; ++i) {
        Entry& e = m.entries[m.entry_count++];
        e.back_action = 274;
        e.select_action = 3014;
        e.select_params = 2;
        e.children = 3;
    }
    m.registered++;
    m.observed = 6;
    m.initialised = true;
    m.list_state = 5;
    m.state = 0;
}

int main()
{
    Model m = { 4, 1, false, 8, 3, 2 };
    ChooseAClan(m);
    if (m.cache_size != 0 || m.connecting_state != 0 || m.state != 3 || m.registered != 0) { std::printf("0055fec0_TEST FAIL\n"); return 1; }
    m.has_inventory = true; m.open_menu = 2;
    ChooseAClan(m);
    if (m.state != 0 || m.registered != 0) { std::printf("0055fec0_TEST FAIL\n"); return 1; }
    m.open_menu = 8; m.state = 7; m.clans = 3;
    ChooseAClan(m);
    if (m.entry_count != 3 || m.entries[2].back_action != 274 || m.entries[0].select_action != 3014 || m.entries[1].select_params != 2
        || m.entries[1].children != 3 || m.registered != 1 || m.observed != 6 || !m.initialised || m.list_state != 5 || m.state != 0) {
        std::printf("0055fec0_TEST FAIL\n"); return 1;
    }
    std::printf("0055fec0_TEST PASS\n");
    return 0;
}