// Behaviour test for 0x00A2F740 C3DMeshStats::InitialiseFromFileData: a portable model -- helper points and dummy objects keyed
// by the CRC32-table hash of their names (initial value 0, no final xor) and sorted by key, a dummy named "createmesh <mesh>"
// (case-insensitive) turned into a mesh generator for <mesh>, mesh volumes keyed the same way, the names collected for packing,
// the bounding sphere copied and the bounding box built as centre -+ extent; returns true.
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

static unsigned long table[256];
static void InitTable() { for (unsigned long i = 0; i < 256; ++i) { unsigned long c = i; for (int k = 0; k < 8; ++k) c = c & 1 ? 0xEDB88320UL ^ (c >> 1) : c >> 1; table[i] = c; } }
static unsigned long NameCRC(const std::string& s) { unsigned long crc = 0; for (size_t i = 0; i < s.size(); ++i) crc = table[(unsigned char)s[i] ^ (crc & 0xff)] ^ (crc >> 8); return crc; }

struct Dummy { std::string name; int bone; };
struct Stats {
    std::vector<std::pair<unsigned long, int> > helpers, dummies, volumes; std::vector<std::string> generators;
    std::set<std::string> names; float sphere[4]; float boxMin[3], boxMax[3];
};
static bool Initialise(Stats& st, const std::vector<std::pair<std::string, int> >& points, const std::vector<Dummy>& dummies,
                       const std::vector<std::string>& volumes, const float centre[3], const float extent[3], const float sphere[4])
{
    for (size_t i = 0; i < points.size(); ++i) { st.helpers.push_back(std::make_pair(NameCRC(points[i].first), points[i].second)); st.names.insert(points[i].first); }
    for (size_t i = 0; i < dummies.size(); ++i) {
        std::string lower = dummies[i].name; for (size_t k = 0; k < lower.size(); ++k) lower[k] = (char)std::tolower((unsigned char)lower[k]);
        if (std::strncmp(lower.c_str(), "createmesh ", 11) == 0) st.generators.push_back(dummies[i].name.substr(11));
        else { st.dummies.push_back(std::make_pair(NameCRC(dummies[i].name), dummies[i].bone)); st.names.insert(dummies[i].name); }
    }
    for (size_t i = 0; i < volumes.size(); ++i) st.volumes.push_back(std::make_pair(NameCRC(volumes[i]), (int)i));
    std::sort(st.volumes.begin(), st.volumes.end()); std::sort(st.helpers.begin(), st.helpers.end()); std::sort(st.dummies.begin(), st.dummies.end());
    for (int k = 0; k < 4; ++k) st.sphere[k] = sphere[k];
    for (int k = 0; k < 3; ++k) { st.boxMin[k] = centre[k] - extent[k]; st.boxMax[k] = centre[k] + extent[k]; }
    return true;
}

int main()
{
    InitTable();
    std::vector<std::pair<std::string, int> > points; points.push_back(std::make_pair(std::string("hand_r"), 3)); points.push_back(std::make_pair(std::string("hand_l"), 4));
    std::vector<Dummy> dummies; Dummy d1 = { "CreateMesh torch", 1 }, d2 = { "muzzle", 2 }; dummies.push_back(d1); dummies.push_back(d2);
    std::vector<std::string> volumes; volumes.push_back("vol_a");
    float centre[3] = { 1, 2, 3 }, extent[3] = { 0.5f, 1, 2 }, sphere[4] = { 1, 2, 3, 4 };
    Stats st;
    bool ok = Initialise(st, points, dummies, volumes, centre, extent, sphere);
    ok = ok && NameCRC("") == 0 && NameCRC("a") == table['a'] && st.helpers.size() == 2 && st.helpers[0].first <= st.helpers[1].first
            && st.generators.size() == 1 && st.generators[0] == "torch" && st.dummies.size() == 1 && st.dummies[0].first == NameCRC("muzzle")
            && st.names.size() == 3 && st.volumes.size() == 1 && st.boxMin[0] == 0.5f && st.boxMax[2] == 5.0f && st.sphere[3] == 4.0f;
    if (!ok) { std::printf("00a2f740_TEST FAIL\n"); return 1; }
    std::printf("00a2f740_TEST PASS\n");
    return 0;
}
