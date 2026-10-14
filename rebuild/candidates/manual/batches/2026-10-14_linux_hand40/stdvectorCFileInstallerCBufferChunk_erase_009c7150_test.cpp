#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CBufferChunk_009c7150 { long m_a; long m_b; };
struct CRetailAllocator_009c7150 { CRetailAllocator_009c7150() { } CRetailAllocator_009c7150(const CRetailAllocator_009c7150&) { } ~CRetailAllocator_009c7150() { } };
static CBufferChunk_009c7150* g_destroyFirst = 0; static CBufferChunk_009c7150* g_destroyLast = 0;
static void DestroyRange_009c6d60(CBufferChunk_009c7150* f, CBufferChunk_009c7150* l, const CRetailAllocator_009c7150&) { g_destroyFirst = f; g_destroyLast = l; }
struct CRetailVector_CBufferChunk { CBufferChunk_009c7150* m_pFirst; CBufferChunk_009c7150* m_pLast; CBufferChunk_009c7150* m_pEnd;
    static CBufferChunk_009c7150* Copy(CBufferChunk_009c7150* f, CBufferChunk_009c7150* l, CBufferChunk_009c7150* result) { for (long n = l - f; n > 0; --n) { *result = *f; ++f; ++result; } return result; }
    CBufferChunk_009c7150* erase(CBufferChunk_009c7150* first, CBufferChunk_009c7150* last) { CBufferChunk_009c7150* newEnd = Copy(last, m_pLast, first); DestroyRange_009c6d60(newEnd, m_pLast, CRetailAllocator_009c7150()); m_pLast = newEnd; return first; } };
int main() {
    CBufferChunk_009c7150 a[5]; for (int i = 0; i < 5; ++i) { a[i].m_a = i; a[i].m_b = 10 + i; } CRetailVector_CBufferChunk v; v.m_pFirst = a; v.m_pLast = a + 5; v.m_pEnd = a + 5;
    CBufferChunk_009c7150* r = v.erase(a + 1, a + 3); if (r != a + 1 || v.m_pLast != a + 3 || a[1].m_a != 3 || a[1].m_b != 13 || a[2].m_a != 4 || g_destroyFirst != a + 3 || g_destroyLast != a + 5) { std::printf("009c7150_TEST FAIL\n"); return 1; }
    std::printf("009c7150_TEST PASS\n"); return 0;
}
