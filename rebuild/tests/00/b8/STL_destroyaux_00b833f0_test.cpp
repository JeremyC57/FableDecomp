// Behaviour test for 0x00B833F0 std::__destroy_aux<CCountedPointer<CParticleSimulation>*>: every element's counted
// pointer releases its info block (deleting the object at zero references) and is nulled.
#include <cstdio>
static int g_deletes = 0;
static void __fastcall DeleteData(void*) { ++g_deletes; }
struct CCPPointerInfo { long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
struct CParticleSimulationPtr {
    void* Data; CCPPointerInfo* Info;
    ~CParticleSimulationPtr()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
struct __false_type { __false_type() {} };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
static void __destroy_aux(CParticleSimulationPtr* first, CParticleSimulationPtr* last, const __false_type&)
{
    for (; first != last; ++first) _Destroy(&*first);
}
int main()
{
    CCPPointerInfo* once = new CCPPointerInfo; once->RefCount = 1; once->DeleteFunc = DeleteData; once->Data = 0;
    CCPPointerInfo* twice = new CCPPointerInfo; twice->RefCount = 2; twice->DeleteFunc = DeleteData; twice->Data = 0;
    static unsigned char storage[3 * sizeof(CParticleSimulationPtr)];
    CParticleSimulationPtr* a = (CParticleSimulationPtr*)storage;
    a[0].Data = 0; a[0].Info = once; a[1].Data = 0; a[1].Info = twice; a[2].Data = 0; a[2].Info = 0;
    __destroy_aux(a, a + 3, __false_type());
    if (g_deletes != 1 || twice->RefCount != 1 || a[0].Info != 0 || a[1].Info != 0 || a[1].Data != 0) { std::printf("00b833f0_TEST FAIL\n"); return 1; }
    __destroy_aux(a, a, __false_type());
    std::printf("00b833f0_TEST PASS\n");
    return 0;
}
