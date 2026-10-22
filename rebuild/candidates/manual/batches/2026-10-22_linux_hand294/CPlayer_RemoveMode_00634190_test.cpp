// Behaviour test for CPlayer::RemoveMode @ 0x00634190: removing the active (top) mode of a local player uninitialises it,
// pops it and initialises the mode below; a buried mode is erased (the topmost occurrence, searched from the top).
#include <cstdio>
#include <list>
#include <vector>
struct Player {
    std::list<int> Modes;
    bool DisallowModeChanges, Local, SuppressFullModeRemoval;
    std::vector<int> Log;
    void UnInitActiveMode(int m) { Log.push_back(100 + m); }
    void InitActiveMode(int m) { Log.push_back(200 + m); }
    void RemoveMode(int mode)
    {
        if (DisallowModeChanges)
            return;
        if (Modes.back() == mode) {
            if (Local) {
                if (!SuppressFullModeRemoval) {
                    SuppressFullModeRemoval = true;
                    std::list<int>::iterator below = Modes.end();
                    --below; --below;
                    UnInitActiveMode(*below);
                    if (Modes.back() == mode)
                        Modes.pop_back();
                    InitActiveMode(mode);
                    SuppressFullModeRemoval = false;
                } else {
                    Modes.pop_back();
                }
            }
        } else {
            std::list<int>::reverse_iterator it = Modes.rbegin();
            for (; it != Modes.rend(); ++it)
                if (*it == mode)
                    break;
            if (it != Modes.rend())
                Modes.erase(--it.base());
        }
    }
};
int main()
{
    Player p;
    p.DisallowModeChanges = false; p.Local = true; p.SuppressFullModeRemoval = false;
    p.Modes.push_back(0); p.Modes.push_back(3); p.Modes.push_back(5); p.Modes.push_back(3); p.Modes.push_back(7);
    p.RemoveMode(7);
    bool ok = p.Modes.size() == 4 && p.Modes.back() == 3 && p.Log.size() == 2 && p.Log[0] == 103 && p.Log[1] == 207 && !p.SuppressFullModeRemoval;
    p.RemoveMode(5);
    std::list<int>::iterator i = p.Modes.begin();
    ok = ok && p.Modes.size() == 3 && *i++ == 0 && *i++ == 3 && *i == 3;
    p.Modes.push_back(5);
    p.RemoveMode(3);
    i = p.Modes.begin();
    ok = ok && p.Modes.size() == 3 && *i++ == 0 && *i++ == 3 && *i == 5;
    p.DisallowModeChanges = true;
    p.RemoveMode(5);
    ok = ok && p.Modes.size() == 3;
    if (!ok) { std::printf("00634190_TEST FAIL\n"); return 1; }
    std::printf("00634190_TEST PASS\n");
    return 0;
}
