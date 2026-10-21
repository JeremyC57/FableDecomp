// Behaviour test for 0x004FBDE0 (CWorldMap::SaveWorldToBinaryFile): counts include the null entry 0, but only
// entries from index 1 are serialised; the stream is then written to the file in one block.
#include <cstdio>
#include <vector>
struct Stream { std::vector<long> words; void WriteSLONG(long v) { words.push_back(v); } };
struct MapInfo { long uid; void SaveBinary(Stream& s) { s.WriteSLONG(uid); } };
struct Region { long id; void SaveBinary(Stream& s) { s.WriteSLONG(1000 + id); } };
static std::vector<long> g_file;
struct WorldMap {
    std::vector<MapInfo> Maps; std::vector<Region> Regions;
    void SaveWorldToBinaryFile()
    {
        Stream stream;
        stream.WriteSLONG((long)Maps.size());
        for (unsigned long i = 1; i < Maps.size(); ++i) Maps[i].SaveBinary(stream);
        stream.WriteSLONG((long)Regions.size());
        for (unsigned long j = 1; j < Regions.size(); ++j) Regions[j].SaveBinary(stream);
        g_file = stream.words;
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    WorldMap w;
    for (long i = 0; i < 3; ++i) { MapInfo m; m.uid = 10 + i; w.Maps.push_back(m); }
    for (long i = 0; i < 2; ++i) { Region r; r.id = i; w.Regions.push_back(r); }
    w.SaveWorldToBinaryFile();
    long expect[] = { 3, 11, 12, 2, 1001 };
    CHECK(g_file.size() == 5);
    for (int k = 0; k < 5 && k < (int)g_file.size(); ++k) CHECK(g_file[k] == expect[k]);
    WorldMap e; e.SaveWorldToBinaryFile();
    CHECK(g_file.size() == 2 && g_file[0] == 0 && g_file[1] == 0);
    if (g_fail) { std::printf("004fbde0_TEST FAIL\n"); return 1; }
    std::printf("004fbde0_TEST PASS\n");
    return 0;
}