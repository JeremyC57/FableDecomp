// Behaviour test for CTCShop::SellItem (0x00774730, parked DIFFER): a model of the row -- quantity drops by one (not below 0), max stock grows by the def change (not below 0), the special item is cleared and the first living display thing of the def is killed.
#include <stdio.h>
#include <map>
#include <vector>
struct Item { float q, maxStock; };
struct Thing { int def; bool dead; };
static void SellItem(std::map<long, Item>& stock, long& special, long& gold, std::vector<Thing>& things, long def, float change)
{
    if (def == special) { special = -1; gold = 0; }
    if (stock.find(def) == stock.end()) return;
    Item& it = stock[def]; it.q = it.q < 1.0f ? 0.0f : it.q - 1.0f; it.maxStock += change; if (it.maxStock < 0) it.maxStock = 0;
    for (size_t i = 0; i < things.size(); ++i) if (!things[i].dead && things[i].def == def) { things[i].dead = true; break; }
}
int main() { std::map<long, Item> s; Item a = { 3, 1 }; s[7] = a; long sp = 7, g = 50; std::vector<Thing> t;
  Thing t1 = { 7, true }, t2 = { 7, false }, t3 = { 7, false }; t.push_back(t1); t.push_back(t2); t.push_back(t3);
  SellItem(s, sp, g, t, 7, -2.0f); SellItem(s, sp, g, t, 9, 1.0f);
  bool ok = s[7].q == 2 && s[7].maxStock == 0 && sp == -1 && g == 0 && t[1].dead && !t[2].dead && s.size() == 1;
  printf(ok ? "00774730_TEST PASS\n" : "00774730_TEST FAIL\n");
  return ok ? 0 : 1;
}
