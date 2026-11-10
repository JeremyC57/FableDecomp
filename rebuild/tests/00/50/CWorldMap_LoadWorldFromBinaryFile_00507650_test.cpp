// Behaviour test for CWorldMap::LoadWorldFromBinaryFile (0x00507650): the row's body over stub engine classes.
// The file holds [mapCount][mapCount-1 map records][regionCount][regionCount-1 region records]; entry 0 of each array keeps its
// default, every map 1..n-1 is placed and registered with the engine manager in order.
#include <stdio.h>
#include <string.h>
#include <vector>

static unsigned char g_file[64];
static long g_fileLength = 0;
static long g_placed[16]; static long g_placedCount = 0;
static long g_added[16];  static long g_addedCount = 0;
static int g_managerCreated = 0;

class CWideString { public: const char* Name; CWideString(const char* n) : Name(n) {} };
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CDiskFileWin32 {
public:
    CDiskFileWin32(const CWideString&, FileAccessType, unsigned int) {}
    long GetLength() const { return g_fileLength; }
    void Read(void* buffer, long size) { memcpy(buffer, g_file, size < g_fileLength ? size : g_fileLength); }
};
template<class T> class CArray : public std::vector<T> {
public:
    explicit CArray(unsigned int n) : std::vector<T>(n) {}
    T* GetAsCArray() { return this->size() > 0 ? &(*this)[0] : (T*)this; }
};
class CMemoryDataInputStream {
public:
    const char* Data; unsigned int Size; unsigned int Pos;
    CMemoryDataInputStream(const void* data, unsigned int size) : Data((const char*)data), Size(size), Pos(0) {}
    long ReadSLONG() { long v = 0; if (Pos + 4 <= Size) { memcpy(&v, Data + Pos, 4); Pos += 4; } return v; }
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CMapInfo { public: C2DBoxI Area; long Loaded; CMapInfo() : Loaded(0) { Area.TLX = Area.TLY = Area.BRX = Area.BRY = 0; }
    void LoadBinary(CMemoryDataInputStream& s) { long v = s.ReadSLONG(); Area.TLX = v; Area.TLY = v; Area.BRX = v + 1; Area.BRY = v + 1; Loaded = 1; } };
class CRegion { public: long Id; CRegion() : Id(-1) {} void LoadBinary(CMemoryDataInputStream& s) { Id = s.ReadSLONG(); } };
class CEngineManager {
public:
    static CEngineManager& Get() { static CEngineManager* p = 0; if (!p) { p = new CEngineManager; ++g_managerCreated; } return *p; }
    void AddMap(long m) { g_added[g_addedCount++] = m; }
};
class CWorldMap {
public:
    std::vector<CMapInfo> Maps;
    std::vector<CRegion> Regions;
    void SetMapPlacement(const C2DBoxI& area, long map) { if (area.TLX == map * 10) g_placed[g_placedCount++] = map; }
    void LoadWorldFromBinaryFile(CWideString& filename)
    {
        CDiskFileWin32 file(filename, FILE_ACCESS_READ, 2);
        CArray<char> buffer(file.GetLength() + 1);
        file.Read(buffer.GetAsCArray(), (long)buffer.size());
        CMemoryDataInputStream stream(buffer.GetAsCArray(), (unsigned int)buffer.size());
        long mapCount = stream.ReadSLONG();
        Maps.resize(mapCount);
        for (long i = 1; i < mapCount; ++i)
            Maps[i].LoadBinary(stream);
        long regionCount = stream.ReadSLONG();
        Regions.resize(regionCount);
        for (long r = 1; r < regionCount; ++r)
            Regions[r].LoadBinary(stream);
        for (long m = 1; m < mapCount; ++m)
        {
            SetMapPlacement(Maps[m].Area, m);
            CEngineManager::Get().AddMap(m);
        }
    }
};

static void put(long v) { memcpy(g_file + g_fileLength, &v, 4); g_fileLength += 4; }

int main()
{
    int ok = 1;
    put(3); put(10); put(20);          // 3 maps: records for 1 and 2
    put(2); put(77);                   // 2 regions: record for 1
    CWorldMap world;
    CWideString name("world.bwd");
    world.LoadWorldFromBinaryFile(name);
    ok &= world.Maps.size() == 3 && world.Regions.size() == 2;
    ok &= world.Maps[0].Loaded == 0 && world.Maps[1].Loaded == 1 && world.Maps[2].Loaded == 1;
    ok &= world.Regions[0].Id == -1 && world.Regions[1].Id == 77;
    ok &= g_placedCount == 2 && g_placed[0] == 1 && g_placed[1] == 2;
    ok &= g_addedCount == 2 && g_added[0] == 1 && g_added[1] == 2 && g_managerCreated == 1;
    // A single-map world places nothing.
    g_fileLength = 0; g_placedCount = 0; g_addedCount = 0;
    put(1); put(1);
    CWorldMap small;
    small.LoadWorldFromBinaryFile(name);
    ok &= small.Maps.size() == 1 && small.Regions.size() == 1 && g_placedCount == 0 && g_addedCount == 0;
    if (ok) printf("00507650_TEST PASS\n"); else printf("00507650_TEST FAIL\n");
    return ok ? 0 : 1;
}