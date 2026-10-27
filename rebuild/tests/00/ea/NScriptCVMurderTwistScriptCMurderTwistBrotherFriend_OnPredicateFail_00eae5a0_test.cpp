#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct ThingK { bool Killed; bool MsgIsKilledBy(const char* killer) const { return Killed && killer[0] == 0; } };
struct Rune { int Id; bool operator==(const Rune& o) const { return Id == o.Id; } };
struct RuneVec { Rune* first; Rune* last;
    Rune* begin() { return first; } Rune* end() { return last; }
    Rune* erase(Rune* p) { for (Rune* q = p; q + 1 < last; ++q) *q = *(q + 1); --last; return p; } };
struct Parent { int MurderTwistItemsFound; RuneVec MagicRuneRumbleList; };
struct E { ThingK MeK; Rune Me; Parent* ParentClass;
    void OnPredicateFail()
    {
        if (MeK.MsgIsKilledBy(""))
            ++ParentClass->MurderTwistItemsFound;
        for (Rune* it = ParentClass->MagicRuneRumbleList.begin(); it != ParentClass->MagicRuneRumbleList.end(); ++it) {
            if (Me == *it) {
                ParentClass->MagicRuneRumbleList.erase(it);
                return;
            }
        }
    }
};
int main()
{
    g_log[0] = 0;
    bool ok = true;
    Rune r[3]; r[0].Id = 1; r[1].Id = 2; r[2].Id = 3;
    Parent p; p.MurderTwistItemsFound = 0; p.MagicRuneRumbleList.first = r; p.MagicRuneRumbleList.last = r + 3;
    E e; e.ParentClass = &p; e.MeK.Killed = true; e.Me.Id = 2; e.OnPredicateFail();
    ok = p.MurderTwistItemsFound == 1 && p.MagicRuneRumbleList.last == r + 2 && r[0].Id == 1 && r[1].Id == 3;
    e.MeK.Killed = false; e.Me.Id = 9; e.OnPredicateFail();
    ok = ok && p.MurderTwistItemsFound == 1 && p.MagicRuneRumbleList.last == r + 2;
    if (!ok) { std::printf("00eae5a0_TEST FAIL\n"); return 1; }
    std::printf("00eae5a0_TEST PASS\n");
    return 0;
}