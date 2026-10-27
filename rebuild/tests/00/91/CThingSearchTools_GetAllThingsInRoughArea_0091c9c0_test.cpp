#include <cstdio>
#include <vector>
// Behaviour model: a mapwho cell walk keeps each thing once (search stamp) and only when both filters accept it.
struct Thing { int id; unsigned stamp; };
struct NotThing { const Thing* t; bool operator()(const Thing& x) const { return &x != t; } };
struct Even { bool operator()(const Thing& x) const { return x.id % 2 == 0; } };
int main()
{
    Thing things[6] = { {0,0},{1,0},{2,0},{3,0},{4,0},{5,0} };
    Thing* cells[2][4] = { { &things[0], &things[1], &things[2], &things[2] }, { &things[3], &things[4], &things[5], &things[4] } };
    unsigned search = 1; NotThing f1 = { &things[4] }; Even f2;
    std::vector<Thing*> out;
    for (int c = 0; c < 2; ++c) for (int i = 0; i < 4; ++i) {
        Thing* t = cells[c][i];
        if (t->stamp < search) { t->stamp = search; if (f1(*t) && f2(*t)) out.push_back(t); }
    }
    if (out.size() != 2 || out[0]->id != 0 || out[1]->id != 2) { std::printf("0091c9c0_TEST FAIL\n"); return 1; }
    std::printf("0091c9c0_TEST PASS\n");
    return 0;
}