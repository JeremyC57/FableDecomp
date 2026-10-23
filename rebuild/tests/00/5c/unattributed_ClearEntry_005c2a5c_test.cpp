#include <cstdio>
static int g_released = 0;
namespace N_005c04bf { struct CCountedPointer { void* m_pObject; void* m_pBlock; CCountedPointer() : m_pObject(0), m_pBlock(0) { } ~CCountedPointer() { ++g_released; m_pObject = 0; m_pBlock = 0; } }; } using N_005c04bf::CCountedPointer;
struct CSoundPair { long m_sound; CCountedPointer m_pSample; long m_volume; CSoundPair() : m_sound(0), m_pSample(), m_volume(0) { } };
struct SSoundPairEntry { unsigned long first; CSoundPair second; SSoundPairEntry() : first(0), second() { } };
static long g_index = -1; static unsigned long g_sum = 1;
struct CSoundPairArray { SSoundPairEntry* first; SSoundPairEntry* last;
    void SetAt(long index, const SSoundPairEntry& e) { g_index = index; g_sum = e.first + e.second.m_sound + (unsigned long)e.second.m_pSample.m_pObject + (unsigned long)e.second.m_pSample.m_pBlock + e.second.m_volume; } };
struct CSoundPairTable_005c2a5c { void* __vftable; CSoundPairArray* m_pEntries; void ClearEntry(long index); };
void CSoundPairTable_005c2a5c::ClearEntry(long index) { SSoundPairEntry entry; m_pEntries->SetAt(index, entry); }
int main()
{
    CSoundPairArray arr = { 0, 0 }; CSoundPairTable_005c2a5c t; t.m_pEntries = &arr;
    t.ClearEntry(6);
    if (g_index != 6 || g_sum != 0 || g_released != 1) { std::printf("005c2a5c_TEST FAIL\n"); return 1; }
    std::printf("005c2a5c_TEST PASS\n");
    return 0;
}