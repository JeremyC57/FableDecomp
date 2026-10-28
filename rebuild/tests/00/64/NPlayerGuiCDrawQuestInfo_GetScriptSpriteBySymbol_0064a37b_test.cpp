// Behaviour test for 0x0064A37B NPlayerGui::CDrawQuestInfo::GetScriptSpriteBySymbol: the mapped sprite index, or 0 for an unknown symbol (standalone copy of the logic)
#include <cstdio>
#include <map>
#include <string>
struct Quest { const std::map<std::string, long>* Sprites;
    long Get(const std::string& s) const { std::map<std::string, long>::const_iterator it = Sprites->find(s); if (it != Sprites->end()) return it->second; return 0; } };

int main()
{
    std::map<std::string, long> m; m["SKULL"] = 12; m["STAR"] = 3; Quest q; q.Sprites = &m;
    if (q.Get("SKULL") != 12 || q.Get("STAR") != 3 || q.Get("MOON") != 0) { std::printf("0064a37b_TEST FAIL\n"); return 1; }
    std::printf("0064a37b_TEST PASS\n");
    return 0;
}