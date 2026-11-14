// Behaviour test for 0x00ED14E0 NScript::CV_SingingStonesScript::CSingingStone::Init: the stone number is the integer value of the
// entity's data string; the particle emitter is the thing whose script name is "M_WispMarker" followed by that data string; the stone
// gets the small green orb mini-map marker; and stones 0..3 get the readable text tags TEXT_QST_060_NAME_A..D (any other number gets
// no tag).
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
struct Game {
    std::vector<std::string> looked, markers, tags;
    int Find(const std::string& name) { looked.push_back(name); return (int)name.size(); }
    void AddMarker(const std::string& icon) { markers.push_back(icon); }
    void SetTag(const std::string& tag) { tags.push_back(tag); }
};
struct Stone {
    Game* game; std::string data; unsigned long number; int emitter;
    void Init() {
        number = (unsigned long)std::atoi(data.c_str());
        emitter = game->Find(std::string("M_WispMarker") + data);
        game->AddMarker("HUD_ORB_GREEN_SMALL");
        switch (number) {
        case 0: game->SetTag("TEXT_QST_060_NAME_A"); break;
        case 1: game->SetTag("TEXT_QST_060_NAME_B"); break;
        case 2: game->SetTag("TEXT_QST_060_NAME_C"); break;
        case 3: game->SetTag("TEXT_QST_060_NAME_D"); break;
        }
    }
};
int main() {
    const char* expect[4] = { "TEXT_QST_060_NAME_A", "TEXT_QST_060_NAME_B", "TEXT_QST_060_NAME_C", "TEXT_QST_060_NAME_D" };
    for (int i = 0; i < 4; ++i) {
        Game g; Stone s; s.game = &g; s.data = std::string(1, (char)('0' + i)); s.number = 99; s.emitter = 0;
        s.Init();
        check(s.number == (unsigned long)i, "stone number from the data string");
        check(g.looked.size() == 1 && g.looked[0] == "M_WispMarker" + s.data && s.emitter == 13, "emitter is the wisp marker of that number");
        check(g.markers.size() == 1 && g.markers[0] == "HUD_ORB_GREEN_SMALL", "one green orb marker");
        check(g.tags.size() == 1 && g.tags[0] == expect[i], "name tag of the stone's letter");
    }
    Game g; Stone s; s.game = &g; s.data = "7"; s.number = 0; s.emitter = 0;
    s.Init();
    check(s.number == 7 && g.looked[0] == "M_WispMarker7" && g.markers.size() == 1 && g.tags.empty(), "numbers above 3: marker but no name tag");
    if (!fails) std::printf("00ed14e0_TEST PASS\n");
    return fails;
}
