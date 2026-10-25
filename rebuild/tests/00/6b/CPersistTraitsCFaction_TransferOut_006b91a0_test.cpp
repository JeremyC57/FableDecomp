// Behaviour test for 0x006B91A0 CPersistTraits<CFaction>::TransferOut (model copy): the faction's text block is appended
// to the output in order -- name, ally count and allies (sorted), enemy count and enemies, EndFaction.
#include <cstdio>
#include <cstring>
#include <set>
#include <string>

struct Faction { std::string Name; std::set<std::string> AlliedFactions; std::set<std::string> EnemyFactions; };

static std::string IntToString(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }

static void TransferOut(std::string& out, const Faction& faction)
{
    out += "Name \"" + faction.Name + "\";\r\n";
    out += "NumberOfAllies " + IntToString((long)faction.AlliedFactions.size()) + ";\r\n";
    for (std::set<std::string>::const_iterator it = faction.AlliedFactions.begin(); it != faction.AlliedFactions.end(); ++it)
        out += "Ally \"" + *it + "\";\r\n";
    out += "NumberOfEnemies " + IntToString((long)faction.EnemyFactions.size()) + ";\r\n";
    for (std::set<std::string>::const_iterator it = faction.EnemyFactions.begin(); it != faction.EnemyFactions.end(); ++it)
        out += "Enemy \"" + *it + "\";\r\n";
    out += "EndFaction";
}

int main()
{
    Faction f;
    f.Name = "Guards";
    f.AlliedFactions.insert("Villagers");
    f.AlliedFactions.insert("Hero");
    f.EnemyFactions.insert("Bandits");
    std::string out = "X";
    TransferOut(out, f);
    const char* want = "XName \"Guards\";\r\nNumberOfAllies 2;\r\nAlly \"Hero\";\r\nAlly \"Villagers\";\r\n"
                       "NumberOfEnemies 1;\r\nEnemy \"Bandits\";\r\nEndFaction";
    int ok = out == want;
    Faction empty;
    std::string out2;
    TransferOut(out2, empty);
    ok = ok && out2 == "Name \"\";\r\nNumberOfAllies 0;\r\nNumberOfEnemies 0;\r\nEndFaction";
    if (!ok) { std::printf("006b91a0_TEST FAIL\n"); return 1; }
    std::printf("006b91a0_TEST PASS\n");
    return 0;
}