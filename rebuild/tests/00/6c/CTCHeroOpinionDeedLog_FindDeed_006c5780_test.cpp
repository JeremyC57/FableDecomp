// Behaviour test for 0x006C5780 CTCHeroOpinionDeedLog::FindDeed(EOpinionDeedType, const CThing*).
#include <cstdio>
#include <map>
struct CThing { int id; };
struct IPtr { const CThing* item; const CThing* GetPItem() const { return item; } };
struct COpinionDeed {
    int DeedType; long DeedHandle; IPtr PTarget; bool IsCrime; long ExtraData;
};
struct CTCHeroOpinionDeedLog {
    std::map<long, COpinionDeed*> OpinionDeedMap;
    long FindWitnessDeed(long deedHandle) {
        for (std::map<long, COpinionDeed*>::iterator it = OpinionDeedMap.begin(); it != OpinionDeedMap.end(); ++it) {
            const COpinionDeed* deed = it->second;
            if (deed && deed->ExtraData == deedHandle && deed->DeedType == 0x35) return it->first;
        }
        return -1;
    }
    long FindDeed(int type, const CThing* target) {
        std::map<long, COpinionDeed*>::iterator end = OpinionDeedMap.end();
        for (std::map<long, COpinionDeed*>::iterator it = OpinionDeedMap.begin(); it != end; ++it) {
            const COpinionDeed* deed = it->second;
            if (deed && deed->DeedType == type && deed->PTarget.GetPItem() == target) return it->first;
        }
        return -1;
    }
    long FindCriminalDeed(int type, const CThing* target) {
        std::map<long, COpinionDeed*>::iterator end = OpinionDeedMap.end();
        for (std::map<long, COpinionDeed*>::iterator it = OpinionDeedMap.begin(); it != end; ++it)
            if (it->second && it->second->DeedType == type && it->second->PTarget.GetPItem() == target && it->second->IsCrime)
                return it->first;
        return -1;
    }
};
int main() {
    int ok = 1;
    CThing a = { 1 }, b = { 2 };
    COpinionDeed d10 = { 7, 10, { &a }, false, 0 };
    COpinionDeed d12 = { 0x35, 12, { &b }, false, 10 };   // a witness deed of deed 10
    COpinionDeed d15 = { 7, 15, { &b }, true, 0 };
    COpinionDeed d17 = { 7, 17, { &a }, true, 0 };
    COpinionDeed d20 = { 0x35, 20, { &a }, false, 15 };   // a witness deed of deed 15
    CTCHeroOpinionDeedLog log;
    log.OpinionDeedMap[20] = &d20;
    log.OpinionDeedMap[5] = 0;                            // a null slot is skipped
    log.OpinionDeedMap[17] = &d17;
    log.OpinionDeedMap[10] = &d10;
    log.OpinionDeedMap[15] = &d15;
    log.OpinionDeedMap[12] = &d12;
    CTCHeroOpinionDeedLog empty;
    ok &= log.FindDeed(7, &a) == 10;                       // the lowest handle wins (in-order walk)
    ok &= log.FindDeed(7, &b) == 15 && log.FindDeed(0x35, &a) == 20 && log.FindDeed(0x35, &b) == 12;
    ok &= log.FindDeed(8, &a) == -1 && log.FindDeed(7, 0) == -1;
    ok &= empty.FindDeed(7, &a) == -1;
    if (!ok) { std::printf("006c5780_TEST FAIL\n"); return 1; }
    std::printf("006c5780_TEST PASS\n");
    return 0;
}