#include <cstdio>
#include <string>
// Behaviour test for CMiniMapDisplay::ConstructMinimapFilename (0x00829A30): file name of the full path, "_", the
// current region's name, the index (zero-padded below 10), ".tga".
static std::string SplitName(const std::string& path)
{
    std::string::size_type slash = path.find_last_of("\\/");
    std::string name = slash == std::string::npos ? path : path.substr(slash + 1);
    std::string::size_type dot = name.find_last_of('.');
    return dot == std::string::npos ? name : name.substr(0, dot);
}
static std::string ToString(long value) { char buf[32]; std::sprintf(buf, "%ld", value); return buf; }
class CRegion { public: std::string Name; };
class CWorldMap { public:
    long Current; CRegion Regions[3];
    long GetCurrentRegionIndex() const { return Current; }
    CRegion& GetRegion(long i) { return Regions[i]; }
};
static std::string Construct(const std::string& path, CWorldMap* map, long index)
{
    std::string filename = SplitName(path);
    std::string regionName(map->GetRegion(map->GetCurrentRegionIndex()).Name);
    filename += "_";
    filename += regionName;
    std::string number;
    if (index < 10) number = ToString(0) + ToString(index);
    else number = ToString(index);
    std::string unused = regionName + number;
    filename += number;
    filename += ".tga";
    return filename;
}
static int Fail() { std::printf("00829a30_TEST FAIL\n"); return 1; }
int main()
{
    CWorldMap map;
    map.Current = 1; map.Regions[0].Name = "A"; map.Regions[1].Name = "Oakvale"; map.Regions[2].Name = "C";
    if (Construct("Data\\Maps\\Minimap.lev", &map, 3) != "Minimap_Oakvale03.tga") return Fail();
    if (Construct("Minimap", &map, 12) != "Minimap_Oakvale12.tga") return Fail();
    map.Current = 2;
    if (Construct("x/y/Mini.bmp", &map, 10) != "Mini_C10.tga") return Fail();
    if (Construct("Mini", &map, 0) != "Mini_C00.tga") return Fail();
    std::printf("00829a30_TEST PASS\n");
    return 0;
}