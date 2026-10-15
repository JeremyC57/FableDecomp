// Behaviour test for 0x00B82330 (see src/00b82330.cpp): AddSimulation growing the array and assigning the counted pointer.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00b82330_TEST FAIL\n"); return 1; } } while (0)

static int g_destroyed = 0, g_deleted = 0;
static void DestroyObject(void*) { ++g_destroyed; }
struct SCountBlock { int m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CCountedPointer {
    void* m_pObject; SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount; void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); ++g_deleted; } }
            m_pObject = object; m_pCount = count;
            if (count) ++count->m_references;
        }
        return *this;
    }
};
struct CSimulationArray {
    CCountedPointer items[8]; unsigned int count;
    unsigned int size() const { return count; }
    void resize(unsigned int n, const CCountedPointer& v) { for (unsigned int i = count; i < n; ++i) items[i] = v; if (n > count) count = n; }
};
struct CEnginePrimitiveSimulationInfo {
    CSimulationArray Simulations;
    void AddSimulation(unsigned int index, const CCountedPointer& sim)
    {
        Simulations.resize(Simulations.size() > index + 1 ? Simulations.size() : index + 1, CCountedPointer());
        Simulations.items[index] = sim;
    }
};
int main()
{
    CEnginePrimitiveSimulationInfo info; info.Simulations.count = 0;
    int objA, objB; SCountBlock ba = { 1, DestroyObject, &objA }, bb = { 1, DestroyObject, &objB };
    CCountedPointer a; a.m_pObject = &objA; a.m_pCount = &ba;
    CCountedPointer b; b.m_pObject = &objB; b.m_pCount = &bb;
    info.AddSimulation(2, a);
    CHECK(info.Simulations.count == 3 && info.Simulations.items[2].m_pObject == &objA && ba.m_references == 2 && info.Simulations.items[0].m_pObject == 0);
    info.AddSimulation(0, b);
    CHECK(info.Simulations.count == 3 && info.Simulations.items[0].m_pObject == &objB && bb.m_references == 2);
    ba.m_references = 1;                             // the array holds the last reference to A
    info.AddSimulation(2, b);
    CHECK(g_destroyed == 1 && g_deleted == 1 && info.Simulations.items[2].m_pObject == &objB && bb.m_references == 3);
    std::printf("00b82330_TEST PASS\n");
    return 0;
}