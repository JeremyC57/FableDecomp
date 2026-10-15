#include <cstdio>
#include <cstdlib>
// Behaviour test for CArray<C3DGroup2>::AddDefault (0x00AE8490): same logic over a copy of the model.
static int g_frees = 0;
static void tracked_free(void* p) { ++g_frees; std::free(p); }
struct C3DVertexBlend2 { long v; };
struct CArrayVertexBlend2 {
    C3DVertexBlend2* m_pFirst; C3DVertexBlend2* m_pLast; C3DVertexBlend2* m_pEnd;
    ~CArrayVertexBlend2() { if (m_pFirst) tracked_free(m_pFirst); }
};
struct C3DGroup2 {
    long BoneIndex; CArrayVertexBlend2 VertexBlends;
    C3DGroup2() { BoneIndex = -1; VertexBlends.m_pFirst = 0; VertexBlends.m_pLast = 0; VertexBlends.m_pEnd = 0; }
};
struct CArrayGroup2 {
    long bones[4]; int count;
    void push_back(const C3DGroup2& value) { bones[count++] = value.BoneIndex; }
    void AddDefault() { push_back(C3DGroup2()); }
};
int main()
{
    CArrayGroup2 a; a.count = 0;
    a.AddDefault(); a.AddDefault();
    if (a.count != 2 || a.bones[0] != -1 || a.bones[1] != -1 || g_frees != 0) { std::printf("00ae8490_TEST FAIL\n"); return 1; }
    std::printf("00ae8490_TEST PASS\n");
    return 0;
}