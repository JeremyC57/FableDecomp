// Behaviour test for CTCInventoryWeapons::OnSerialise @ 0x005C3A95: save writes names for positive indices; load maps non-empty names back
#include <cstdio>
#include <cstring>
#include <string>
static const char* g_names[] = { "", "SWORD", "BOW", "AXE", "XBOW" };
static std::string NameOf(long i) { return (i > 0 && i < 5) ? g_names[i] : ""; }
static long IndexOf(const std::string& s) { for (long i = 1; i < 5; ++i) if (s == g_names[i]) return i; return -1; }
struct Weapons { long conf[2]; long prev[2]; };
static void Save(const Weapons& w, std::string out[4])
{
    std::string melee, ranged;
    if (w.conf[0] > 0) melee = NameOf(w.conf[0]);
    if (w.conf[1] > 0) ranged = NameOf(w.conf[1]);
    out[0] = melee; out[1] = ranged;
    if (w.prev[0] > 0) melee = NameOf(w.prev[0]);
    if (w.prev[1] > 0) ranged = NameOf(w.prev[1]);
    out[2] = melee; out[3] = ranged;
}
static void Load(Weapons& w, const std::string in[4])
{
    if (in[0] != "") w.conf[0] = IndexOf(in[0]);
    if (in[1] != "") w.conf[1] = IndexOf(in[1]);
    if (in[2] != "") w.prev[0] = IndexOf(in[2]);
    if (in[3] != "") w.prev[1] = IndexOf(in[3]);
}
int main()
{
    int ok = 1;
    Weapons w = { { 1, 0 }, { 0, 4 } };
    std::string names[4];
    Save(w, names);
    if (names[0] != "SWORD" || names[1] != "" || names[2] != "SWORD" || names[3] != "XBOW") ok = 0;
    Weapons r = { { 0, 0 }, { 0, 0 } };
    Load(r, names);
    if (r.conf[0] != 1 || r.conf[1] != 0 || r.prev[0] != 1 || r.prev[1] != 4) ok = 0;
    if (!ok) { std::printf("005c3a95_TEST FAIL\n"); return 1; }
    std::printf("005c3a95_TEST PASS\n");
    return 0;
}