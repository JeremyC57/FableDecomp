// Behaviour test for 0x006C2AA0 std::__uninitialized_copy over CLevelLoader::CObject::CLevelFiles (byte twin of 0x00513500) (placement copy, each counted pointer bumps its reference count).
#include <cstdio>
#include <new>

class CAFile { public: int id; };
class CCPPointerInfo { public: unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
template <class T>
class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* d, CCPPointerInfo* i) : Data(d), Info(i) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
struct CLevelFiles {
    long                    LevelID;
    CCountedPointer<CAFile> PTopologyFile;
    CCountedPointer<CAFile> PNavigationFile;
    CCountedPointer<CAFile> PThingFile;
    CLevelFiles(long id, CAFile* f, CCPPointerInfo* i0, CCPPointerInfo* i1, CCPPointerInfo* i2)
        : LevelID(id), PTopologyFile(f, i0), PNavigationFile(f + 1, i1), PThingFile(f + 2, i2) { }
};
struct __false_type { __false_type() { } };

static CLevelFiles* __uninitialized_copy(CLevelFiles* first, CLevelFiles* last, CLevelFiles* result, const __false_type&)
{
    CLevelFiles* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CLevelFiles(*first);
    return cur;
}

int main()
{
    CAFile files[9];
    CCPPointerInfo info[3];
    for (int k = 0; k < 3; ++k) { info[k].RefCount = 1; info[k].DeleteFunc = 0; info[k].Data = 0; }
    // level 0 shares info[0] for two files and has no thing file; level 1/2 use info[1]/info[2]
    CLevelFiles src[3] = {
        CLevelFiles(10, files + 0, &info[0], &info[0], 0),
        CLevelFiles(11, files + 3, &info[1], 0, &info[1]),
        CLevelFiles(12, files + 6, &info[2], &info[2], &info[2]),
    };
    for (int k = 0; k < 3; ++k) info[k].RefCount = 1;   // the array initialisation may have copied (and counted) temporaries
    static double raw[3 * sizeof(CLevelFiles) / sizeof(double) + 2];   // suitably aligned raw storage
    CLevelFiles* dst = (CLevelFiles*)(void*)raw;
    CLevelFiles* end = __uninitialized_copy(src, src + 3, dst, __false_type());
    if (end != dst + 3) { std::printf("006c2aa0_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 3; ++k)
        if (dst[k].LevelID != 10 + k || dst[k].PTopologyFile.Data != files + 3 * k || dst[k].PNavigationFile.Data != files + 3 * k + 1
            || dst[k].PThingFile.Data != files + 3 * k + 2 || dst[k].PThingFile.Info != src[k].PThingFile.Info)
            { std::printf("006c2aa0_TEST FAIL\n"); return 1; }
    if (info[0].RefCount != 3 || info[1].RefCount != 3 || info[2].RefCount != 4) { std::printf("006c2aa0_TEST FAIL\n"); return 1; }
    if (__uninitialized_copy(src, src, dst, __false_type()) != dst) { std::printf("006c2aa0_TEST FAIL\n"); return 1; }
    std::printf("006c2aa0_TEST PASS\n");
    return 0;
}