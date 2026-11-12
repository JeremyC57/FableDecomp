// Behaviour test for 0x008E16A0 NCreatureMode::CModeScreamerSuperAttack::FrameUpdate: base update always; a dead/missing beam does
// nothing more; a live beam with a live target gets LIGHTNING_START = the eye helper point and LIGHTNING_END = target + 1.75 Z,
// sorted by key; a live beam without a live target is killed.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
struct V3 { float X, Y, Z; };
typedef std::pair<unsigned long, V3> Param;
static bool Less(const Param& a, const Param& b) { return a.first < b.first; }
static unsigned long Crc(const char* s) { unsigned long h = 0; while (*s) h = h * 31 + (unsigned char)*s++; return h; }
struct Thing { bool Dead; V3 Pos; std::vector<Param> Params; bool Killed; };
struct Mode {
    int BaseUpdates; V3 Eye; Thing* Target; Thing* Beam;
    void FrameUpdate()
    {
        ++BaseUpdates;
        if (Beam && !Beam->Dead) {
            if (Target && !Target->Dead) {
                V3 start = Eye;
                V3 end = { Target->Pos.X, Target->Pos.Y, Target->Pos.Z + 1.75f };
                std::vector<Param> params;
                params.push_back(Param(Crc("LIGHTNING_START"), start));
                params.push_back(Param(Crc("LIGHTNING_END"), end));
                std::sort(params.begin(), params.end(), Less);
                Beam->Params = params;
            } else
                Beam->Killed = true;
        }
    }
};
int main()
{
    Thing target = { false, { 1, 2, 3 }, std::vector<Param>(), false };
    Thing beam = { false, { 0, 0, 0 }, std::vector<Param>(), false };
    V3 eye = { 9, 8, 7 };
    Mode m = { 0, eye, &target, &beam };
    m.FrameUpdate();
    bool ok = beam.Params.size() == 2 && !beam.Killed;
    for (size_t i = 0; i < beam.Params.size(); ++i) {
        if (beam.Params[i].first == Crc("LIGHTNING_END")) ok = ok && beam.Params[i].second.Z == 4.75f && beam.Params[i].second.X == 1;
        if (beam.Params[i].first == Crc("LIGHTNING_START")) ok = ok && beam.Params[i].second.X == 9;
    }
    ok = ok && beam.Params[0].first < beam.Params[1].first;
    target.Dead = true; m.FrameUpdate(); ok = ok && beam.Killed;
    beam.Dead = true; beam.Killed = false; m.FrameUpdate(); ok = ok && !beam.Killed && m.BaseUpdates == 3;
    if (!ok) { std::printf("008e16a0_TEST FAIL\n"); return 1; }
    std::printf("008e16a0_TEST PASS\n");
    return 0;
}