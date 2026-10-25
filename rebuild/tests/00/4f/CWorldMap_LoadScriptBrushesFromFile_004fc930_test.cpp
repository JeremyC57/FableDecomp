// Behaviour test (0x004fc930): CWorldMap::LoadScriptBrushesFromFile hands the file to the map's CMap and releases its by-value counted pointer
#include <cstdio>
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CAFile { public: int Id; };
static int g_loads = 0, g_lastId = 0, g_deleted = 0;
class CMap {
public:
    void LoadBrushesFromFile(CAFile& file) { ++g_loads; g_lastId = file.Id; }
};
class CCPPointerInfo {
public:
    long  Count;
    void (*DeleteData)(void* data);
    void* Data;
};
static void DeleteAFile(void* data) { ++g_deleted; delete (CAFile*)data; }
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data) : Data(data), Info(new CCPPointerInfo) { Info->Count = 1; Info->DeleteData = DeleteAFile; Info->Data = data; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->Count; }
    ~CCountedPointer()
    {
        if (Info)
        {
            if (--Info->Count == 0)
            {
                Info->DeleteData(Info->Data);
                delete Info;
            }
        }
    }
    T& operator*() const { return *Data; }
};
class CMapInfo { public: C2DBoxI Area; CMap* PMap; void* PMapInfo; };
template <class T> class CArray {
public:
    T* Start; T* Finish; T* EndOfStorage;
    const T& operator[](unsigned long i) const { return Start[i]; }
};
class CWorldMap {
public:
    CArray<CMapInfo> Maps;
    void LoadScriptBrushesFromFile(long map, CCountedPointer<CAFile> file)
    {
        Maps[map].PMap->LoadBrushesFromFile(*file);
    }
};
int main()
{
    CMap maps[2];
    CMapInfo infos[2]; infos[0].PMap = &maps[0]; infos[1].PMap = &maps[1];
    CWorldMap wm; wm.Maps.Start = infos; wm.Maps.Finish = infos + 2; wm.Maps.EndOfStorage = infos + 2;
    CAFile* f = new CAFile; f->Id = 77;
    {
        CCountedPointer<CAFile> held(f);
        wm.LoadScriptBrushesFromFile(1, held);   // the copy is released by the callee
        if (g_loads != 1 || g_lastId != 77 || g_deleted != 0 || held.Info->Count != 1) { std::printf("004fc930_TEST FAIL\n"); return 1; }
    }
    if (g_deleted != 1) { std::printf("004fc930_TEST FAIL\n"); return 1; }
    std::printf("004fc930_TEST PASS\n");
    return 0;
}