// Behaviour test for 0x006C2940 CLevelLoader::CObject::CLevelFiles::operator=(const CLevelFiles&).
#include <cstdio>
struct Info;
static int g_deleted = 0, g_freedFiles = 0;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteFile(void* p) { ++g_freedFiles; (void)p; }
struct CP {
    void* Data; Info* PInfo;
    CP& operator=(const CP& other) {
        Info* info = other.PInfo; void* data = other.Data;
        if (PInfo != info) {
            if (PInfo) { --PInfo->RefCount; if (PInfo->RefCount == 0) { PInfo->DeleteFunc(PInfo->Data); ++g_deleted; } }
            Data = data; PInfo = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
};
struct CLevelFiles { long LevelID; CP Topology, Navigation, Thing; };
int main() {
    int ok = 1;
    int f1, f2, f3, f4;
    Info i1 = { 1, DeleteFile, &f1 }, i2 = { 2, DeleteFile, &f2 }, i3 = { 1, DeleteFile, &f3 }, i4 = { 1, DeleteFile, &f4 };
    CLevelFiles a = { 5, { &f1, &i1 }, { &f2, &i2 }, { 0, 0 } };
    CLevelFiles b = { 9, { &f3, &i3 }, { &f2, &i2 }, { &f4, &i4 } };
    CLevelFiles* r = &(a = b);
    ok &= r == &a && a.LevelID == 9;
    ok &= a.Topology.Data == &f3 && a.Topology.PInfo == &i3 && i3.RefCount == 2;
    ok &= i1.RefCount == 0 && g_deleted == 1 && g_freedFiles == 1;      // the old topology block died
    ok &= a.Navigation.PInfo == &i2 && i2.RefCount == 2;               // same block: untouched
    ok &= a.Thing.Data == &f4 && a.Thing.PInfo == &i4 && i4.RefCount == 2;
    CLevelFiles c = { 1, { 0, 0 }, { 0, 0 }, { 0, 0 } };
    a = c;                                                              // release everything
    ok &= a.LevelID == 1 && a.Topology.PInfo == 0 && a.Thing.Data == 0;
    ok &= i3.RefCount == 1 && i2.RefCount == 1 && i4.RefCount == 1 && g_deleted == 1;
    if (!ok) { std::printf("006c2940_TEST FAIL\n"); return 1; }
    std::printf("006c2940_TEST PASS\n");
    return 0;
}