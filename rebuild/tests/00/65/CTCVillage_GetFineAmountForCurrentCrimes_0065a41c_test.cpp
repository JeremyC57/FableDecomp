// Behaviour test for CTCVillage::GetFineAmountForCurrentCrimes (0x0065A41C): the same logic through a small copy of the model.
#include <cstdio>
#include <cstring>
#include <map>
#include <set>

struct CRegionDefModel { bool UseCrimes; };
struct CEffectsModel { long CrimeFineAmount; bool OnlyGetFinedForOneInstance; };
struct CDeedLogModel {
    std::map<long, long> Deeds;                 // handle -> deed type
    CEffectsModel Effects[10];
    const long* GetDeedFromHandle(long handle) const
    {
        std::map<long, long>::const_iterator it = Deeds.find(handle);
        return it == Deeds.end() ? 0 : &it->second;
    }
};
struct CVillageModel {
    const CRegionDefModel* RegionDef;
    const CDeedLogModel* HeroLog;               // null: no hero
    std::set<long> PlayersCrimes;
    long AmountOfFineAlreadyPaid;
    long GetFineAmountForCurrentCrimes()
    {
        const CRegionDefModel* def = RegionDef;
        if (def && def->UseCrimes) {
            long fine = -AmountOfFineAlreadyPaid;
            if (HeroLog) {
                bool seen[10];
                std::memset(seen, 0, sizeof(seen));
                std::set<long>::iterator end = PlayersCrimes.end();
                for (std::set<long>::iterator it = PlayersCrimes.begin(); it != end; ++it) {
                    const long* deed = HeroLog->GetDeedFromHandle(*it);
                    if (deed) {
                        const CEffectsModel* effects = &HeroLog->Effects[*deed];
                        if (effects->CrimeFineAmount == -1) {
                            fine = -1;
                            break;
                        }
                        if (!effects->OnlyGetFinedForOneInstance || !seen[*deed])
                            fine += effects->CrimeFineAmount;
                        seen[*deed] = true;
                    }
                }
            }
            if (fine < 0) {
                AmountOfFineAlreadyPaid = -fine;
                fine = 0;
            }
            return fine;
        }
        return 0;
    }
};

static int Fail(int n) { std::printf("0065a41c_TEST FAIL (case %d)\n", n); return 1; }

int main()
{
    CDeedLogModel log;
    std::memset(log.Effects, 0, sizeof(log.Effects));
    log.Effects[1].CrimeFineAmount = 100; log.Effects[1].OnlyGetFinedForOneInstance = true;
    log.Effects[2].CrimeFineAmount = 50;  log.Effects[2].OnlyGetFinedForOneInstance = false;
    log.Effects[3].CrimeFineAmount = -1;  log.Effects[3].OnlyGetFinedForOneInstance = false;
    log.Deeds[10] = 1; log.Deeds[11] = 1; log.Deeds[12] = 2; log.Deeds[13] = 2; log.Deeds[20] = 3;
    CRegionDefModel on = { true }, off = { false };

    CVillageModel v;
    v.HeroLog = &log;
    v.PlayersCrimes.insert(10); v.PlayersCrimes.insert(11); v.PlayersCrimes.insert(12); v.PlayersCrimes.insert(13);
    v.PlayersCrimes.insert(99);                 // a handle the log no longer knows: skipped

    // 1: no region def -> 0, nothing touched
    v.RegionDef = 0; v.AmountOfFineAlreadyPaid = 30;
    if (v.GetFineAmountForCurrentCrimes() != 0 || v.AmountOfFineAlreadyPaid != 30) return Fail(1);
    // 2: the region does not use crimes -> 0, nothing touched
    v.RegionDef = &off;
    if (v.GetFineAmountForCurrentCrimes() != 0 || v.AmountOfFineAlreadyPaid != 30) return Fail(2);
    // 3: type 1 fined once (100), type 2 fined per instance (50 + 50), less the 30 already paid
    v.RegionDef = &on;
    if (v.GetFineAmountForCurrentCrimes() != 170 || v.AmountOfFineAlreadyPaid != 30) return Fail(3);
    // 4: nothing paid
    v.AmountOfFineAlreadyPaid = 0;
    if (v.GetFineAmountForCurrentCrimes() != 200 || v.AmountOfFineAlreadyPaid != 0) return Fail(4);
    // 5: paid more than owed -> 0, the surplus stays as paid
    v.AmountOfFineAlreadyPaid = 500;
    if (v.GetFineAmountForCurrentCrimes() != 0 || v.AmountOfFineAlreadyPaid != 300) return Fail(5);
    // 6: no hero -> the paid amount alone (negative total folds back, result 0)
    v.HeroLog = 0; v.AmountOfFineAlreadyPaid = 40;
    if (v.GetFineAmountForCurrentCrimes() != 0 || v.AmountOfFineAlreadyPaid != 40) return Fail(6);
    // 7: a crime whose fine is -1 ends the walk: result 0, paid becomes 1
    v.HeroLog = &log; v.PlayersCrimes.insert(20); v.AmountOfFineAlreadyPaid = 0;
    if (v.GetFineAmountForCurrentCrimes() != 0 || v.AmountOfFineAlreadyPaid != 1) return Fail(7);
    // 8: the -1 crime sorts first in the set: nothing before it matters either
    v.PlayersCrimes.clear(); v.PlayersCrimes.insert(20); v.PlayersCrimes.insert(30); log.Deeds[30] = 2; v.AmountOfFineAlreadyPaid = 7;
    if (v.GetFineAmountForCurrentCrimes() != 0 || v.AmountOfFineAlreadyPaid != 1) return Fail(8);
    // 9: no crimes on record and nothing paid -> 0
    v.PlayersCrimes.clear(); v.AmountOfFineAlreadyPaid = 0;
    if (v.GetFineAmountForCurrentCrimes() != 0 || v.AmountOfFineAlreadyPaid != 0) return Fail(9);

    std::printf("0065a41c_TEST PASS\n");
    return 0;
}