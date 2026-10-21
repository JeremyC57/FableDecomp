// Behaviour test for CTCInventoryMap::SetPanMode (0x005FB8F0): nothing happens without both components; pan mode stores
// the flag and picks the region-list text; leaving it picks the map text and moves to centre - offset * zoom when the
// selected region exists.
#include <cstdio>
#include <cstring>

struct V2 { float x, y; };
struct State { bool hasList, hasMap, pan; const char* text; int moves; V2 target; };
static void SetPanMode(State& s, bool pan, bool regionFound, V2 centre, V2 offset, float zoom)
{
    if (s.hasList && s.hasMap) {
        s.pan = pan;
        if (!pan) {
            s.text = "TEXT_GUI_MENU_MAP_SCROLL";
            if (regionFound) {
                s.target.x = centre.x - offset.x * zoom;
                s.target.y = centre.y - offset.y * zoom;
                ++s.moves;
            }
        } else {
            s.text = "TEXT_GUI_MENU_REGION_LIST_SCROLL";
        }
    }
}
int main()
{
    State s; s.hasList = true; s.hasMap = false; s.pan = false; s.text = ""; s.moves = 0;
    V2 c = { 10.0f, 20.0f }, o = { 2.0f, 4.0f };
    SetPanMode(s, true, true, c, o, 2.0f);
    if (s.pan || std::strlen(s.text) != 0) { std::printf("005fb8f0_TEST FAIL\n"); return 1; }
    s.hasMap = true;
    SetPanMode(s, true, true, c, o, 2.0f);
    if (!s.pan || std::strcmp(s.text, "TEXT_GUI_MENU_REGION_LIST_SCROLL") != 0 || s.moves) { std::printf("005fb8f0_TEST FAIL\n"); return 1; }
    SetPanMode(s, false, true, c, o, 2.0f);
    if (s.pan || s.moves != 1 || s.target.x != 6.0f || s.target.y != 12.0f) { std::printf("005fb8f0_TEST FAIL\n"); return 1; }
    std::printf("005fb8f0_TEST PASS\n");
    return 0;
}