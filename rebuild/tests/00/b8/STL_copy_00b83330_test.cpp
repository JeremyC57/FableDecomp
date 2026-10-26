// Behaviour test for 0x00B83330 std::__copy<CCountedPointer<CParticleSimulation>*, ..., int>: counted-pointer
// assignment over [first, last) (release the old info block at zero references, adopt and bump the new one), returning
// the advanced destination.
#include <cstdio>
static int g_deletes = 0;
static void __fastcall DeleteData(void*) { ++g_deletes; }
class CCPPointerInfo { public: long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
struct CParticleSimulationPtr {
    void* Data; CCPPointerInfo* Info;
    CParticleSimulationPtr& operator=(const CParticleSimulationPtr& other)
    {
        CCPPointerInfo* info = other.Info; void* data = other.Data;
        if (Info != info) {
            if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
            Data = data; Info = info; if (info) ++info->RefCount;
        }
        return *this;
    }
};
struct random_access_iterator_tag { random_access_iterator_tag() {} };
static CParticleSimulationPtr* __copy(CParticleSimulationPtr* first, CParticleSimulationPtr* last, CParticleSimulationPtr* result, const random_access_iterator_tag&, int*)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    CCPPointerInfo* shared = new CCPPointerInfo; shared->RefCount = 1; shared->DeleteFunc = DeleteData; shared->Data = 0;
    CCPPointerInfo* old = new CCPPointerInfo; old->RefCount = 1; old->DeleteFunc = DeleteData; old->Data = 0;
    CParticleSimulationPtr src[2]; src[0].Data = 0; src[0].Info = shared; src[1].Data = 0; src[1].Info = 0;
    CParticleSimulationPtr dst[2]; dst[0].Data = 0; dst[0].Info = old; dst[1].Data = 0; dst[1].Info = 0;
    CParticleSimulationPtr* e = __copy(src, src + 2, dst, random_access_iterator_tag(), (int*)0);
    if (e != dst + 2 || dst[0].Info != shared || shared->RefCount != 2 || g_deletes != 1 || dst[1].Info != 0
        || __copy(src, src, dst, random_access_iterator_tag(), (int*)0) != dst) { std::printf("00b83330_TEST FAIL\n"); return 1; }
    std::printf("00b83330_TEST PASS\n");
    return 0;
}
