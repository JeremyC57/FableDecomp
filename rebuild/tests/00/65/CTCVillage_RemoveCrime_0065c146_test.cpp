// 0065c146 behaviour test: CTCVillage::RemoveCrime (final.cpp body over standard containers)
#include <stdio.h>
#include <set>
#include <vector>
#include <map>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CTCHeroOpinionDeedLog { public:
    std::set<long> Deeds;
    std::map<long, std::vector<long> > Witnesses;   // deed -> witness deeds still chained to it
    std::vector<long> Log;                          // +h RemoveDeed, -h RemoveDeedStillSearchingForWitnesses
    bool GetDeedFromHandle(long h) { return Deeds.count(h) != 0; }
    long FindWitnessDeed(long h) { std::vector<long>& w = Witnesses[h]; if (w.empty()) return -1; long r = w.front(); w.erase(w.begin()); return r; }
    void RemoveDeedStillSearchingForWitnesses(long h) { Log.push_back(-h); }
    void RemoveDeed(long h) { Deeds.erase(h); Log.push_back(h); }
};
class CThing { public: CTCHeroOpinionDeedLog* Log; };
static CThing* g_hero;
class CTCVillage { public:
    std::set<long> PlayersCrimes;
    bool IsEnemyBecauseOfCrime;
    void RemoveCrime(long deedHandle);
};
void CTCVillage::RemoveCrime(long deedHandle)
{
    CThing* hero = g_hero;
    if (hero) {
        long handle = deedHandle;
        CTCHeroOpinionDeedLog* log = hero->Log;
        if (log->GetDeedFromHandle(handle)) {
            log->RemoveDeedStillSearchingForWitnesses(handle);
            long deed = handle;
            for (;;) {
                log->RemoveDeed(deed);
                long witness = log->FindWitnessDeed(handle);
                if (witness == -1)
                    break;
                log->RemoveDeedStillSearchingForWitnesses(witness);
                deed = witness;
            }
        }
    }
    PlayersCrimes.erase(deedHandle);
    if (PlayersCrimes.size() == 0)
        IsEnemyBecauseOfCrime = false;
}
int main()
{
    CTCHeroOpinionDeedLog log; CThing hero = { &log };
    log.Deeds.insert(5); log.Deeds.insert(7); log.Deeds.insert(8);
    log.Witnesses[5].push_back(7); log.Witnesses[5].push_back(8);
    CTCVillage v; v.PlayersCrimes.insert(5); v.PlayersCrimes.insert(9); v.IsEnemyBecauseOfCrime = true;
    g_hero = &hero;
    v.RemoveCrime(5);
    CHECK(log.Log.size() == 6);
    CHECK(log.Log[0] == -5 && log.Log[1] == 5 && log.Log[2] == -7 && log.Log[3] == 7 && log.Log[4] == -8 && log.Log[5] == 8);
    CHECK(log.Deeds.empty());
    CHECK(v.PlayersCrimes.size() == 1 && v.IsEnemyBecauseOfCrime);
    g_hero = 0;
    v.RemoveCrime(9);
    CHECK(v.PlayersCrimes.empty() && !v.IsEnemyBecauseOfCrime);
    CHECK(log.Log.size() == 6);
    if (!g_fail) printf("0065c146_TEST PASS\n");
    return g_fail;
}