// Behaviour test for 0x006C7370 CTCHeroOpinionDeedLog::OnSerialise(CPersistContext&, bool): a model; checks the opinion-deed map round-trips through the flattened record vector (built from the map when saving, re-inserted when loading) with the scalar fields.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct State { long nextHandle; bool crimeA, crimeB; long village; std::map<long, long> deeds; };
static std::vector<long> Save(const State& s)
{
    std::vector<long> r; r.push_back(s.nextHandle); r.push_back(s.crimeA); r.push_back(s.crimeB); r.push_back(s.village);
    for (std::map<long, long>::const_iterator i = s.deeds.begin(); i != s.deeds.end(); ++i) { r.push_back(i->first); r.push_back(i->second); }
    return r;
}
static State Load(const std::vector<long>& r)
{
    State s; s.nextHandle = r[0]; s.crimeA = r[1] != 0; s.crimeB = r[2] != 0; s.village = r[3];
    for (size_t i = 4; i + 1 < r.size(); i += 2) s.deeds.insert(std::make_pair(r[i], r[i + 1]));
    return s;
}
int main()
{
    int ok = 1; State s; s.nextHandle = 9; s.crimeA = true; s.crimeB = false; s.village = 4; s.deeds[3] = 30; s.deeds[1] = 10;
    State t = Load(Save(s)); ok &= t.nextHandle == 9 && t.crimeA && !t.crimeB && t.village == 4 && t.deeds == s.deeds;
    std::printf(ok ? "006C7370_TEST PASS\n" : "006C7370_TEST FAIL\n"); return ok ? 0 : 1;
}