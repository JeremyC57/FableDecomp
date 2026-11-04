// Behaviour test for CTCHeroStats::GetFavouriteWeapon (0x0057AEB4, parked DIFFER): a model of the row -- the weapon name the hero carries most of for the kind (1 melee only, else ranged), empty when none.
#include <stdio.h>
#include <string>
#include <vector>
struct W { std::string name; bool melee; int count; };
static std::string Favourite(const std::vector<W>& ws, long kind)
{
    std::string best; int n = 0;
    for (size_t i = 0; i < ws.size(); ++i) if ((kind == 1) == ws[i].melee && ws[i].count > n) { n = ws[i].count; best = ws[i].name; }
    return best;
}
int main() { std::vector<W> v; W a = { "SWORD", true, 3 }, b = { "AXE", true, 5 }, c = { "BOW", false, 9 };
  v.push_back(a); v.push_back(b); v.push_back(c);
  bool ok = Favourite(v, 1) == "AXE" && Favourite(v, 2) == "BOW" && Favourite(std::vector<W>(), 1).empty();
  printf(ok ? "0057aeb4_TEST PASS\n" : "0057aeb4_TEST FAIL\n");
  return ok ? 0 : 1;
}
