// Behaviour test for CSoundManagerLH::SetChannelGain (0x00A3EED0): find the channel by its name pair; if found, set its two
// values (virtual slot 1) and return true, else false. Host model.
#include <cstdio>
#include <string>
struct Entry { long a, b; void Set(long x, long y) { a = x; b = y; } };
struct Manager { Entry* found;
    Entry* FindByNamePair(const std::string& f, const std::string& s) { return (f == "MUSIC" && s == "MAIN") ? found : 0; }
    bool SetChannelGain(const std::string& f, const std::string& s, long a, long b) { Entry* e = FindByNamePair(f, s); if (!e) return false; e->Set(a, b); return true; } };
int main()
{
    Entry e; e.a = 0; e.b = 0; Manager m; m.found = &e;
    bool ok = !m.SetChannelGain("SFX", "MAIN", 1, 2) && e.a == 0;
    ok = ok && m.SetChannelGain("MUSIC", "MAIN", 3, 4) && e.a == 3 && e.b == 4;
    if (!ok) { std::printf("00a3eed0_TEST FAIL\n"); return 1; }
    std::printf("00a3eed0_TEST PASS\n");
    return 0;
}
