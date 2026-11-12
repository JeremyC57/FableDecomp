// Behaviour test for 0x0065C5BB CTCVillage::UpdateMirroredCrimeData: the mirrored record of the current region is re-counted from
// the village's crime handles (types below 10; a type whose effects def is OnlyGetFinedForOneInstance counts once), stamped with
// FramePlayerLastSeenByGuard, and written back; missing hero / stats leave everything untouched.
#include <cstdio>
#include <map>
#include <set>
#include <string>
struct Status { long Crimes[10]; long LastTimeSeenByGuard; long Other[6]; };
struct Deed { long DeedType; };
struct DeedLog {
    std::map<long, Deed> Deeds;
    bool OnlyOnce[16];
    const Deed* GetDeedFromHandle(long h) const { std::map<long, Deed>::const_iterator it = Deeds.find(h); return it == Deeds.end() ? 0 : &it->second; }
};
struct Hero { bool HasStats; std::map<std::string, Status> OutstandingCrimes; DeedLog Log; };
struct Village {
    std::set<long> PlayersCrimes;
    long FramePlayerLastSeenByGuard;
    std::string Region;
    void UpdateMirroredCrimeData(Hero* hero)
    {
        if (!hero) return;
        if (!hero->HasStats) return;
        Status status = hero->OutstandingCrimes[Region];
        status.LastTimeSeenByGuard = FramePlayerLastSeenByGuard;
        for (long n = 0; n < 10; ++n) status.Crimes[n] = 0;
        std::set<long>::const_iterator end = PlayersCrimes.end();
        for (std::set<long>::const_iterator it = PlayersCrimes.begin(); it != end; ++it) {
            const Deed* deed = hero->Log.GetDeedFromHandle(*it);
            if (deed && deed->DeedType < 10) {
                if (!hero->Log.OnlyOnce[deed->DeedType] || status.Crimes[deed->DeedType] == 0)
                    ++status.Crimes[deed->DeedType];
            }
        }
        hero->OutstandingCrimes[Region] = status;
    }
};
int main()
{
    Hero hero; hero.HasStats = true;
    for (int i = 0; i < 16; ++i) hero.Log.OnlyOnce[i] = false;
    hero.Log.OnlyOnce[3] = true;
    Deed d;
    d.DeedType = 1; hero.Log.Deeds[10] = d; hero.Log.Deeds[11] = d;
    d.DeedType = 3; hero.Log.Deeds[20] = d; hero.Log.Deeds[21] = d; hero.Log.Deeds[22] = d;
    d.DeedType = 12; hero.Log.Deeds[30] = d;
    Status old = Status(); old.Crimes[5] = 7; old.Other[2] = 99; hero.OutstandingCrimes["Oakvale"] = old;
    Village v; v.Region = "Oakvale"; v.FramePlayerLastSeenByGuard = 1234;
    long handles[] = { 10, 11, 20, 21, 22, 30, 40 };
    for (int i = 0; i < 7; ++i) v.PlayersCrimes.insert(handles[i]);
    v.UpdateMirroredCrimeData(&hero);
    const Status& s = hero.OutstandingCrimes["Oakvale"];
    bool ok = s.Crimes[1] == 2 && s.Crimes[3] == 1 && s.Crimes[5] == 0 && s.LastTimeSeenByGuard == 1234 && s.Other[2] == 99;
    Hero none; none.HasStats = false;
    v.UpdateMirroredCrimeData(&none);
    ok = ok && none.OutstandingCrimes.empty();
    v.UpdateMirroredCrimeData(0);
    if (!ok) { std::printf("0065c5bb_TEST FAIL\n"); return 1; }
    std::printf("0065c5bb_TEST PASS\n");
    return 0;
}