#include <cstdio>
// Behaviour model of CTCKraken::CTCKraken (0x004DEFA7).
struct Ptr { void* p; Ptr() : p(0) {} };
struct List { int size; List() : size(0) {} };
struct Kraken {
    void* parent; const void* def; Ptr Chamber, Centre, Target, Head; List Explosions; Ptr Beam; List Tentacles;
    long StrikeCounter; bool Ready, Track, Hit; long LastStrike, KillBeam; long Phase, Mode;
    Kraken(void* p) : parent(p), def(0), StrikeCounter(0), Ready(false), Track(false), Hit(false), LastStrike(0), KillBeam(0), Phase(0), Mode(0) {}
};
int main()
{
    int t; Kraken k(&t);
    if (k.parent != &t || k.def || k.Head.p || k.Beam.p || k.Tentacles.size || k.Ready || k.Phase || k.Mode || k.KillBeam) { std::printf("004defa7_TEST FAIL\n"); return 1; }
    std::printf("004defa7_TEST PASS\n");
    return 0;
}
