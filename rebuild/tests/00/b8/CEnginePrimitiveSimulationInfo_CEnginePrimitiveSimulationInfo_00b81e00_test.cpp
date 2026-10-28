// Behaviour test for CEnginePrimitiveSimulationInfo::CEnginePrimitiveSimulationInfo (0x00B81E00): reference count 1, empty simulations array.
#include <cstdio>
struct Pointee { unsigned int IVRefCount; Pointee() : IVRefCount(1) {} };
struct SimInfo : Pointee { void* start; void* finish; void* eos; SimInfo() : start(0), finish(0), eos(0) {} };
int main()
{
    SimInfo s;
    if (s.IVRefCount != 1 || s.start != 0 || s.finish != 0 || s.eos != 0) { std::printf("00b81e00_TEST FAIL\n"); return 1; }
    std::printf("00b81e00_TEST PASS\n");
    return 0;
}