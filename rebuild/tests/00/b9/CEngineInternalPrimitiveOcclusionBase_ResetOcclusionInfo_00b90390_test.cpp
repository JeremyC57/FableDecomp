#include <cstdio>
static int g_deleted = 0;
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { ++g_deleted; } unsigned long IVRefCount; };
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } }
    CIVCountedPointer& operator=(CIVCountedPointeeBase* o) { Release(); Object = o; if (o) ++o->IVRefCount; return *this; }
};
struct COcc { unsigned char _pad[0x48]; CIVCountedPointer OcclusionInfo[2]; void ResetOcclusionInfo(long layer) { if (layer != -1) OcclusionInfo[layer] = 0; } };
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 2; b.IVRefCount = 1;
    COcc o; o.OcclusionInfo[0].Object = &a; o.OcclusionInfo[1].Object = &b;
    o.ResetOcclusionInfo(-1);
    if (o.OcclusionInfo[0].Object != &a || o.OcclusionInfo[1].Object != &b) { std::printf("00b90390_TEST FAIL\n"); return 1; }
    o.ResetOcclusionInfo(0);
    if (o.OcclusionInfo[0].Object != 0 || a.IVRefCount != 1 || g_deleted != 0) { std::printf("00b90390_TEST FAIL\n"); return 1; }
    o.ResetOcclusionInfo(1);
    if (o.OcclusionInfo[1].Object != 0 || b.IVRefCount != 0 || g_deleted != 1) { std::printf("00b90390_TEST FAIL\n"); return 1; }
    std::printf("00b90390_TEST PASS\n"); return 0;
}