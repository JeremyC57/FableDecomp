// Behaviour test for 0x00728F80: resize(n) forwards n and a default record (empty item vector) to resize(n, value), then destroys it.
#include <cstdio>
static unsigned int g_n = 0; static int g_emptyValue = 0; static int g_dtors = 0;
struct CItemVector { void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { } ~CItemVector() { ++g_dtors; } };
struct SKeyedItems { long m_key; CItemVector m_items; SKeyedItems() { } };
struct CKeyedVectorVector {
    void resize(unsigned int newSize, const SKeyedItems& value) { g_n = newSize; g_emptyValue = (!value.m_items._M_start && !value.m_items._M_finish && !value.m_items._M_end_of_storage); }
    void resize(unsigned int newSize) { resize(newSize, SKeyedItems()); }
};
int main()
{
    CKeyedVectorVector v;
    v.resize(3);
    if (g_n != 3 || !g_emptyValue || g_dtors != 1) { std::printf("00728f80_TEST FAIL\n"); return 1; }
    std::printf("00728f80_TEST PASS\n");
    return 0;
}