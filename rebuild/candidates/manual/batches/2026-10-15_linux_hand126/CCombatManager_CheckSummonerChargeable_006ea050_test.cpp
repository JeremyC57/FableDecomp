// Behaviour test for 006ea050 CCombatManager::CheckSummonerChargeable: unregistered summoners and
// summoners without attackers are not chargeable; otherwise the per-info test decides.
#include <cstdio>
#include <map>
#ifdef _MSC_VER
typedef unsigned __int64 UID64;
#else
typedef unsigned long long UID64;
#endif

struct CInfo { unsigned long attackers; bool due; };
struct CThing { UID64 UniqueID; };
struct CManager {
    std::map<UID64, CInfo> Combatants;
    int m_tests;
    bool IsDefenderDueForAttack(CInfo* info) { ++m_tests; return info->due; }
    bool CheckSummonerChargeable(CThing* summoner)
    {
        UID64 id = summoner->UniqueID;
        std::map<UID64, CInfo>::iterator it = Combatants.find(id);
        if (it != Combatants.end() && it->second.attackers != 0)
            return IsDefenderDueForAttack(&Combatants.find(id)->second);
        return false;
    }
};

int main()
{
    CManager m; m.m_tests = 0;
    CInfo idle = { 0, true }, busyDue = { 2, true }, busyNot = { 1, false };
    m.Combatants[1] = idle; m.Combatants[2] = busyDue; m.Combatants[3] = busyNot;
    CThing a = { 1 }, b = { 2 }, c = { 3 }, d = { 9 };
    bool ok = !m.CheckSummonerChargeable(&a) && m.CheckSummonerChargeable(&b) && !m.CheckSummonerChargeable(&c)
        && !m.CheckSummonerChargeable(&d) && m.m_tests == 2;
    if (!ok) { std::printf("006ea050_TEST FAIL\n"); return 1; }
    std::printf("006ea050_TEST PASS\n");
    return 0;
}
