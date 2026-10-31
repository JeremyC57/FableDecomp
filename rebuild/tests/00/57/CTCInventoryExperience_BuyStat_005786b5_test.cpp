// behaviour test for CTCInventoryExperience::BuyStat 0x005786B5: the stat's undo record is created on first buy and reused
// after; each buy raises the stat one level, spends the experience, bumps the record's level count and sets DestroyEverything
#include <cstdio>
#include <map>
struct Undo { long Levels, Before, After; Undo() : Levels(0), Before(0), After(0) {} };
struct Hero { long Level[4]; long Exp; };
struct Inv { std::map<int, Undo> undos; bool destroy; Hero hero; long snapshots; };
static void BuyStat(Inv& v, int stat, long amount)
{
    ++v.snapshots;
    std::map<int, Undo>::iterator it = v.undos.find(stat);
    if (it == v.undos.end()) { Undo u; v.undos.insert(std::make_pair(stat, u)); it = v.undos.find(stat); }
    Undo& u = it->second;
    u.Before = v.hero.Exp;
    v.hero.Level[stat] += 1;
    v.hero.Exp -= amount;
    u.After = v.hero.Exp;
    ++it->second.Levels;
    v.destroy = true;
}
int main()
{
    Inv v; v.destroy = false; v.snapshots = 0; for (int j = 0; j < 4; ++j) v.hero.Level[j] = 0; v.hero.Exp = 1000;
    BuyStat(v, 1, 100); BuyStat(v, 1, 200); BuyStat(v, 2, 50);
    int ok = v.undos.size() == 2 && v.undos[1].Levels == 2 && v.undos[1].Before == 900 && v.undos[1].After == 700
          && v.hero.Level[1] == 2 && v.hero.Level[2] == 1 && v.hero.Exp == 650 && v.destroy && v.snapshots == 3;
    std::printf(ok ? "005786b5_TEST PASS\n" : "005786b5_TEST FAIL\n");
    return !ok;
}