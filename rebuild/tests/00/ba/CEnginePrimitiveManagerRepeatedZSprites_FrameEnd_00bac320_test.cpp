#include <cstdio>
static unsigned long g_count = 99, g_fill = 99; static int g_calls = 0;
struct CRetailVector_uint { unsigned long* m_pFirst; unsigned long* m_pLast; unsigned long* m_pEnd; void resize(unsigned long count, const unsigned long& fill) { g_count = count; g_fill = fill; ++g_calls; } };
class CEnginePrimitiveManagerRepeatedZSprites { public: unsigned char _pad[0x80]; CRetailVector_uint m_indices; unsigned char _pad8c[0x34]; long m_count;
    void FrameEnd() { unsigned long fill = 0; m_indices.resize(0, fill); m_count = 0; } };
int main() {
    CEnginePrimitiveManagerRepeatedZSprites* m = (CEnginePrimitiveManagerRepeatedZSprites*)new char[sizeof(CEnginePrimitiveManagerRepeatedZSprites)]; m->m_count = 9; m->FrameEnd();
    if (g_count != 0 || g_fill != 0 || g_calls != 1 || m->m_count != 0) { std::printf("00bac320_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00bac320_TEST PASS\n"); return 0;
}