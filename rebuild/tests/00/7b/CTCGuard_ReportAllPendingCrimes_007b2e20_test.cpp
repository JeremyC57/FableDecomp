// Behaviour test for 0x007B2E20 CTCGuard::ReportAllPendingCrimes(CTCVillage*): every pending crime goes to the village,
// unknown deeds are passed to the village's opinion of the hero, a severity 2/3 crime triggers one security sweep
// recalculation, and the pending queue is always cleared (even with no village).
#include <cstdio>
#include <deque>
#include <vector>

struct Crime { int DeedHandle; int Type; };

struct Opinion {
    std::vector<int> known;
    std::vector<int> informed;
    bool KnowsOfDeed(int deed) const
    {
        for (unsigned int i = 0; i < known.size(); ++i) if (known[i] == deed) return true;
        return false;
    }
    void InformOfDeed(int deed) { informed.push_back(deed); }
};

struct Village {
    std::vector<int> reported;
    Opinion* opinion;
    int sweeps;
    void ReportCrime(int deed) { reported.push_back(deed); }
};

static int SeverityOf(int type) { return type == 7 ? 3 : (type == 5 ? 2 : 1); }

struct Guard {
    std::deque<Crime> PendingCrimes;
    void ReportAllPendingCrimes(Village* village, bool heroHasLog)
    {
        if (village) {
            bool severe = false;
            for (std::deque<Crime>::iterator it = PendingCrimes.begin(); it != PendingCrimes.end(); ++it) {
                Crime crime = *it;
                village->ReportCrime(crime.DeedHandle);
                if (village->opinion && !village->opinion->KnowsOfDeed(crime.DeedHandle))
                    village->opinion->InformOfDeed(crime.DeedHandle);
                if (heroHasLog) {
                    int s = SeverityOf(it->Type);
                    if (s == 2 || s == 3) severe = true;
                }
            }
            if (severe) ++village->sweeps;
        }
        PendingCrimes.clear();
    }
};

static Crime mk(int d, int t) { Crime c; c.DeedHandle = d; c.Type = t; return c; }

int main()
{
    int ok = 1;
    Guard g;
    for (int i = 0; i < 20; ++i) g.PendingCrimes.push_back(mk(100 + i, 1));   // spans two 16-crime nodes
    g.PendingCrimes.push_back(mk(200, 5));
    Opinion op; op.known.push_back(103); op.known.push_back(117);
    Village v; v.opinion = &op; v.sweeps = 0;
    g.ReportAllPendingCrimes(&v, true);
    if (v.reported.size() != 21 || v.reported[0] != 100 || v.reported[20] != 200) ok = 0;
    if (op.informed.size() != 19) ok = 0;
    if (v.sweeps != 1 || !g.PendingCrimes.empty()) ok = 0;

    g.PendingCrimes.push_back(mk(300, 7));
    g.ReportAllPendingCrimes(&v, false);                                         // no deed log: no sweep
    if (v.sweeps != 1 || v.reported.size() != 22 || !g.PendingCrimes.empty()) ok = 0;

    g.PendingCrimes.push_back(mk(400, 7));
    g.ReportAllPendingCrimes(0, true);                                           // no village: just cleared
    if (v.reported.size() != 22 || !g.PendingCrimes.empty()) ok = 0;

    if ((unsigned int)sizeof(Crime) != 8) ok = 0;
    if (!ok) { std::printf("007b2e20_TEST FAIL\n"); return 1; }
    std::printf("007b2e20_TEST PASS\n");
    return 0;
}