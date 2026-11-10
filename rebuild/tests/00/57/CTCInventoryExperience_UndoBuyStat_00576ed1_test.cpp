// Behaviour test for CTCInventoryExperience::UndoBuyStat (0x00576ED1): a model of both paths. Unconfirmed: the pending stat is parked,
// the yes/no dialog is centred at (width/2 - scale.x*104, height/2 - scale.y*52), dialog and mouse pointer become the parked
// observers. Confirmed: the screen is flagged for rebuild, the stat drops one level, the record's experience is given back and
// the record is erased once its list of purchases is empty.
#include <cstdio>
#include <map>
#include <list>
struct SUndo { std::list<long> Generals; long spent; };
struct SState {
    long pending; float x, y; int dialog, pointer; bool rebuild, destroy;
    std::map<int, long> levels; std::map<int, SUndo> undo; long refunded;
};
static void UndoBuyStat(SState& s, int key, bool confirmed, float width, float height, float sx, float sy)
{
    if (!confirmed) {
        s.pending = key;
        s.y = height * 0.5f - sy * 52.0f;
        s.x = (width * 0.5f) - sx * 104.0f;
        s.dialog = 1;
        s.pointer = 1;
    } else {
        s.rebuild = true;
        std::map<int, SUndo>::iterator it = s.undo.find(key);
        s.levels[key] = s.levels[key] - 1;
        s.refunded += it->second.spent;
        it->second.Generals.pop_back();
        if (it->second.Generals.size() == 0)
            s.undo.erase(it);
        s.destroy = true;
    }
}
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
int main()
{
    int fails = 0;
    SState s; s.pending = -1; s.x = s.y = 0.0f; s.dialog = s.pointer = 0; s.rebuild = s.destroy = false; s.refunded = 0;
    UndoBuyStat(s, 7, false, 640.0f, 480.0f, 1.0f, 1.0f);
    if (s.pending != 7 || !Near(s.x, 216.0f) || !Near(s.y, 188.0f) || s.dialog != 1 || s.pointer != 1 || s.rebuild) ++fails;
    s.levels[7] = 3;
    SUndo u; u.Generals.push_back(10); u.Generals.push_back(20); u.spent = 30; s.undo[7] = u;
    UndoBuyStat(s, 7, true, 0.0f, 0.0f, 0.0f, 0.0f);
    if (s.levels[7] != 2 || s.refunded != 30 || s.undo.size() != 1 || !s.rebuild || !s.destroy) ++fails;
    UndoBuyStat(s, 7, true, 0.0f, 0.0f, 0.0f, 0.0f);
    if (s.levels[7] != 1 || s.refunded != 60 || !s.undo.empty()) ++fails;
    std::printf(fails ? "00576ed1_TEST FAIL\n" : "00576ed1_TEST PASS\n");
    return fails;
}