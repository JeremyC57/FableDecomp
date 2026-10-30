// Behaviour test for 0x0082E860 CScriptedMapBrush::GetThingsToMove (see src/0082e860.cpp): the keys of ThingsToMove
// (the moved things' UIDs, in ascending order) are appended to the output list after whatever it already holds.
#include <cstdio>
#include <list>
#include <map>
typedef unsigned long long UID;
struct PosAndOrientation { float Position[3]; float FacingAngles[3]; };
struct Brush {
    std::map<UID, PosAndOrientation> ThingsToMove;
    void GetThingsToMove(std::list<UID>& out) const
    {
        for (std::map<UID, PosAndOrientation>::const_iterator i = ThingsToMove.begin(); i != ThingsToMove.end(); ++i)
            out.push_back((*i).first);
    }
};
int main()
{
    Brush b; PosAndOrientation p = { { 0, 0, 0 }, { 0, 0, 0 } };
    b.ThingsToMove[0x500000000ULL] = p; b.ThingsToMove[7] = p; b.ThingsToMove[42] = p;
    std::list<UID> out; out.push_back(1);
    b.GetThingsToMove(out);
    std::list<UID>::iterator it = out.begin();
    bool ok = out.size() == 4 && *it++ == 1 && *it++ == 7 && *it++ == 42 && *it == 0x500000000ULL;
    Brush e; std::list<UID> none; e.GetThingsToMove(none); ok = ok && none.empty();
    std::printf(ok ? "0082e860_TEST PASS\n" : "0082e860_TEST FAIL\n");
    return ok ? 0 : 1;
}