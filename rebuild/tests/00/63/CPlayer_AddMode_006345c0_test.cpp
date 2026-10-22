// Behaviour test for CPlayer::AddMode @ 0x006345C0: a local player pushes the mode (optionally resetting the stack to
// the null mode first); without a pending full change the active mode is uninitialised and the previous one re-initialised.
#include <cstdio>
#include <list>
#include <vector>
struct Player {
    std::list<int> Modes;
    bool DisallowModeChanges, Local, SuppressFullModeRemoval;
    std::vector<int> Log;
    int GetCurrentMode() const { return Modes.back(); }
    void UnInitActiveMode(int m) { Log.push_back(100 + m); }
    void InitActiveMode(int m) { Log.push_back(200 + m); }
    void AddMode(int mode, bool clearOthers)
    {
        if (DisallowModeChanges || !Local)
            return;
        int previous = GetCurrentMode();
        if (clearOthers) {
            UnInitActiveMode(mode);
            Modes.clear();
            Modes.push_front(0);
        }
        if (!SuppressFullModeRemoval) {
            SuppressFullModeRemoval = true;
            UnInitActiveMode(mode);
            Modes.push_back(mode);
            InitActiveMode(previous);
            SuppressFullModeRemoval = false;
        } else {
            Modes.push_back(mode);
        }
    }
};
int main()
{
    Player p;
    p.DisallowModeChanges = false; p.Local = true; p.SuppressFullModeRemoval = false;
    p.Modes.push_back(0); p.Modes.push_back(4);
    p.AddMode(6, false);
    bool ok = p.Modes.size() == 3 && p.Modes.back() == 6 && p.Log.size() == 2 && p.Log[0] == 106 && p.Log[1] == 204;
    p.AddMode(9, true);
    ok = ok && p.Modes.size() == 2 && p.Modes.front() == 0 && p.Modes.back() == 9 && p.Log.size() == 5 && p.Log[4] == 206;
    p.SuppressFullModeRemoval = true;
    p.AddMode(2, false);
    ok = ok && p.Modes.back() == 2 && p.Log.size() == 5;
    p.Local = false;
    p.AddMode(3, false);
    ok = ok && p.Modes.back() == 2;
    if (!ok) { std::printf("006345c0_TEST FAIL\n"); return 1; }
    std::printf("006345c0_TEST PASS\n");
    return 0;
}