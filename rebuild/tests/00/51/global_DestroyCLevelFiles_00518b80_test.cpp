// Behaviour test for 00518b80 (_Destroy_CLevelFiles): every element of [first, last) is destroyed once, in order; an empty range destroys nothing.
#include <cstdio>
#include <new>
static const void* g_log[8];
static int g_count = 0;
class CAFile;
struct CCountedPointerAFile { CAFile* Object; void* Block; };
struct CLevelFiles {                        // CLevelLoader::CObject::CLevelFiles (28 bytes in retail)
    long LevelID;
    CCountedPointerAFile PTopologyFile;
    CCountedPointerAFile PNavigationFile;
    CCountedPointerAFile PThingFile;
    CLevelFiles() : LevelID(7) { PTopologyFile.Object = 0; PTopologyFile.Block = 0; PNavigationFile = PTopologyFile; PThingFile = PTopologyFile; }
    ~CLevelFiles() { g_log[g_count++] = this; }   // stands in for 0x0050F980
};
void _Destroy_CLevelFiles(CLevelFiles* first, CLevelFiles* last)
{
    for (; first != last; ++first)
        first->~CLevelFiles();
}
union UStorage { double d; void* p; long l; char bytes[sizeof(CLevelFiles) * 3]; };
int main()
{
    UStorage storage;
    CLevelFiles* items = reinterpret_cast<CLevelFiles*>(storage.bytes);
    for (int i = 0; i < 3; ++i)
        new (&items[i]) CLevelFiles();
    _Destroy_CLevelFiles(items, items);
    if (g_count != 0) { std::printf("00518b80_TEST FAIL\n"); return 1; }
    _Destroy_CLevelFiles(items, items + 3);
    if (g_count != 3) { std::printf("00518b80_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (g_log[i] != &items[i]) { std::printf("00518b80_TEST FAIL\n"); return 1; }
    std::printf("00518b80_TEST PASS\n");
    return 0;
}