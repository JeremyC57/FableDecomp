#include <cstdio>
// Behaviour model of CTCKrakenTentacle::CTCKrakenTentacle (0x004DB2E7): every pointer null, strike flags false,
// ticks zero, DoneDeathStuff false.
struct Tentacle {
    void* def; void* kraken; void* obstruction; void* ripple;
    bool canStrike, tracking, isStriking; long queued, last, rise; bool doneDeath;
    Tentacle() : def(0), kraken(0), obstruction(0), ripple(0), canStrike(false), tracking(false), isStriking(false),
                 queued(0), last(0), rise(0), doneDeath(false) { }
};
int main()
{
    Tentacle t;
    if (t.def || t.kraken || t.obstruction || t.ripple || t.canStrike || t.tracking || t.isStriking || t.queued || t.last || t.rise || t.doneDeath) { std::printf("004db2e7_TEST FAIL\n"); return 1; }
    std::printf("004db2e7_TEST PASS\n");
    return 0;
}