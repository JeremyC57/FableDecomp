// Test for 0x006C20B0 CLevelLoader::CObject::OnAdd(const CLevelLoader&): remembers the loader and marks every region
// level's map as changed, in order, on the loader's world map.
#include <stdio.h>
#include <string.h>
class CEditWorldMap {
public:
    void SetMapAsChanged(long mapIndex, bool changed);       // 0x004FCFE0
};
namespace std {
template <class T> struct allocator { };
template <class T, class A = allocator<T> > struct vector {  // STLport vector: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() { return end() - begin(); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {                                  // 28 bytes
        public:
            long          LevelID;                           // +0x00
            unsigned char _files04[0x18];                    // +0x04 three CCountedPointer<CAFile>
        };
        unsigned char                  LoadableSections[0x0c];   // +0x00 set<CCharString> (STLport, 12 bytes)
        CLevelLoader*                  PLevelLoader;             // +0x0c
        std::vector<CLevelFiles>       RegionLevelFiles;         // +0x10
        void OnAdd(const CLevelLoader& loader);
    };
    unsigned char  _pad00[0x1c];
    CEditWorldMap* WorldMap;                                     // +0x1c
};


static long g_changed[8]; static int g_n; static const CEditWorldMap* g_map; static bool g_flag = true;
void CEditWorldMap::SetMapAsChanged(long mapIndex, bool changed) { g_map = this; g_changed[g_n++] = mapIndex; g_flag = g_flag && changed; }
int main()
{
    CEditWorldMap map;
    CLevelLoader loader; memset(&loader, 0, sizeof loader); loader.WorldMap = &map;
    CLevelLoader::CObject::CLevelFiles files[3];
    files[0].LevelID = 4; files[1].LevelID = 9; files[2].LevelID = 2;
    CLevelLoader::CObject o; memset(&o, 0, sizeof o);
    o.RegionLevelFiles._M_start = files; o.RegionLevelFiles._M_finish = files + 3;
    o.OnAdd(loader);
    if (o.PLevelLoader != &loader || g_map != &map || g_n != 3 || !g_flag) return 1;
    if (g_changed[0] != 4 || g_changed[1] != 9 || g_changed[2] != 2) return 2;
    o.RegionLevelFiles._M_finish = files;
    o.OnAdd(loader);
    if (g_n != 3) return 3;
    puts("006c20b0_TEST PASS");
    return 0;
}
