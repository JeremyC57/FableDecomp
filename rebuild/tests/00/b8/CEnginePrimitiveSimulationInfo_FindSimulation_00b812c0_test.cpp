// Behaviour test for CEnginePrimitiveSimulationInfo::FindSimulation (0x00B812C0): the simulation at the index, 0 past the end.
#include <cstdio>
struct Sim { int id; };
struct CP { Sim* Data; void* Info; };
struct SimInfo { CP* start; CP* finish;
    Sim* FindSimulation(unsigned int index) { if (index < (unsigned int)(finish - start)) return start[index].Data; return 0; } };
int main()
{
    Sim a, b; CP arr[2] = { { &a, 0 }, { &b, 0 } }; SimInfo s; s.start = arr; s.finish = arr + 2;
    if (s.FindSimulation(0) != &a || s.FindSimulation(1) != &b || s.FindSimulation(2) != 0 || s.FindSimulation(0xFFFFFFFFu) != 0) {
        std::printf("00b812c0_TEST FAIL\n"); return 1; }
    std::printf("00b812c0_TEST PASS\n");
    return 0;
}