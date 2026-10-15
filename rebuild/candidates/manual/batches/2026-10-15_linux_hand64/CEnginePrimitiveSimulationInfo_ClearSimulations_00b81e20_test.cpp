#include <cstdio>
// Behaviour of CEnginePrimitiveSimulationInfo::ClearSimulations (0x00B81E20): erase(begin(), end()) -- the empty
// tail is moved onto the first element, every old element is destroyed, and the array ends at its start.
struct CEngineSimulationBase; struct SCountBlock;
struct CSimulationPointer { CEngineSimulationBase* Object; SCountBlock* Block; };
struct SMoveTag { SMoveTag() { } };
struct SAllocatorTag { SAllocatorTag() { } };
static int g_destroyed = 0;
static CSimulationPointer* Move_00b82ec0(CSimulationPointer* first, CSimulationPointer* last, CSimulationPointer* dest, const SMoveTag&, bool)
{
    for (; first != last; ++first, ++dest) *dest = *first;
    return dest;
}
static void DestroyRange_00b83580(CSimulationPointer* first, CSimulationPointer* last, const SAllocatorTag&)
{
    for (; first != last; ++first) { first->Object = 0; first->Block = 0; ++g_destroyed; }
}
struct CEnginePrimitiveSimulationInfo {
    void* m_vtable; long m_refCount;
    CSimulationPointer* m_simulationsFirst; CSimulationPointer* m_simulationsLast; CSimulationPointer* m_simulationsEnd;
    void ClearSimulations()
    {
        CSimulationPointer* last = m_simulationsLast;
        CSimulationPointer* first = m_simulationsFirst;
        CSimulationPointer* kept = Move_00b82ec0(last, last, first, SMoveTag(), false);
        DestroyRange_00b83580(kept, m_simulationsLast, SAllocatorTag());
        m_simulationsLast = kept;
    }
};
int main()
{
    CSimulationPointer items[4]; int dummy;
    for (int i = 0; i < 4; ++i) { items[i].Object = (CEngineSimulationBase*)&dummy; items[i].Block = 0; }
    CEnginePrimitiveSimulationInfo info; info.m_simulationsFirst = items; info.m_simulationsLast = items + 3; info.m_simulationsEnd = items + 4;
    info.ClearSimulations();
    if (info.m_simulationsLast != items || g_destroyed != 3 || items[0].Object != 0 || items[2].Object != 0 || items[3].Object == 0 || info.m_simulationsEnd != items + 4) { std::printf("00b81e20_TEST FAIL\n"); return 1; }
    std::printf("00b81e20_TEST PASS\n");
    return 0;
}
