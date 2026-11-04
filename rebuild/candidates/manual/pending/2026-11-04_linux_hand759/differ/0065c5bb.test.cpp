// Behaviour test for CTCVillage::UpdateMirroredCrimeData (0x0065C5BB, parked DIFFER): a model of the row -- the mirror gets every crime record, updated against the hero.
#include <stdio.h>
#include <map>
static void Mirror(const std::map<int, int>& crimes, std::map<int, int>& mirror, int heroBonus)
{ mirror.clear(); for (std::map<int, int>::const_iterator i = crimes.begin(); i != crimes.end(); ++i) mirror[i->first] = i->second + heroBonus; }
int main() { std::map<int, int> c, m; c[1] = 5; c[3] = 7; m[9] = 1; Mirror(c, m, 1);
  bool ok = m.size() == 2 && m[1] == 6 && m[3] == 8 && m.find(9) == m.end();
  printf(ok ? "0065c5bb_TEST PASS\n" : "0065c5bb_TEST FAIL\n");
  return ok ? 0 : 1;
}
