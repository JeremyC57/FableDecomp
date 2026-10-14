#include <cstdio>
static int g_deleted = 0, g_order = 0, g_firstDeleted = -1;
struct CIVCountedPointeeBase { virtual ~CIVCountedPointeeBase() { } virtual void Delete() { if (g_firstDeleted < 0) g_firstDeleted = id; ++g_deleted; } unsigned long IVRefCount; int id; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; ~CIVCountedPointer() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } } };
struct CEngineInternalPrimitiveBase : CIVCountedPointeeBase { unsigned char _pad08[0x40]; ~CEngineInternalPrimitiveBase() { } };
struct COcclusionBase : CEngineInternalPrimitiveBase { CIVCountedPointer OcclusionInfo[2]; ~COcclusionBase() { } };
struct CDerived : COcclusionBase { };
int main()
{
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; a.id = 0; b.IVRefCount = 1; b.id = 1;
    {
        CDerived d; d.OcclusionInfo[0].Object = &a; d.OcclusionInfo[1].Object = &b;
    }
    if (g_deleted != 2 || g_firstDeleted != 1 || a.IVRefCount != 0 || b.IVRefCount != 0) { std::printf("00b9c660_TEST FAIL\n"); return 1; }
    std::printf("00b9c660_TEST PASS\n"); return 0;
}