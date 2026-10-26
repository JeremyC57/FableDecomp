#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SFreeArea; static long g_pushed = 0; static SFreeArea* g_value = (SFreeArea*)1;
struct CHandleVector { unsigned char _pad[4]; SFreeArea** m_pFirst; SFreeArea** m_pLast; SFreeArea** m_pEnd; void PushBack(SFreeArea* const& value) { ++g_pushed; g_value = value; } long Size() { if (m_pFirst == 0) return 0; return m_pLast - m_pFirst; } SFreeArea*& At(long index) { return m_pFirst[index]; } };
class CHandleBasedMemoryPool { public: unsigned char _pad[0x43c]; CHandleVector m_handles; long m_firstFreeHandle;
    long InternalAllocateHandle() { long index = m_firstFreeHandle; if (index != -1) { SFreeArea*& slot = m_handles.At(index); m_firstFreeHandle = (long)(size_t)slot; slot = (SFreeArea*)-1; return index; } long count = m_handles.Size(); SFreeArea* empty = 0; m_handles.PushBack(empty); return count; } };

int main() {
    CHandleBasedMemoryPool p; std::memset(&p, 0, sizeof p); SFreeArea* slots[4] = { (SFreeArea*)1, (SFreeArea*)7, (SFreeArea*)-1, (SFreeArea*)3 }; p.m_handles.m_pFirst = slots; p.m_handles.m_pLast = slots + 4; p.m_firstFreeHandle = 1;
    if (p.InternalAllocateHandle() != 1 || p.m_firstFreeHandle != 7 || slots[1] != (SFreeArea*)-1 || g_pushed) { std::printf("00c28a80_TEST FAIL\n"); return 1; } p.m_firstFreeHandle = -1; if (p.InternalAllocateHandle() != 4 || g_pushed != 1 || g_value != 0) { std::printf("00c28a80_TEST FAIL\n"); return 1; }
    p.m_handles.m_pFirst = 0; if (p.InternalAllocateHandle() != 0 || g_pushed != 2) { std::printf("00c28a80_TEST FAIL\n"); return 1; }
    std::printf("00c28a80_TEST PASS\n"); return 0;
}