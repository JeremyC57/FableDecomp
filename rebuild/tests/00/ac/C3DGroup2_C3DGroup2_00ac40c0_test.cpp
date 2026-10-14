#include <cstdio>
struct CArrayVB { void* m_pData; unsigned long m_count; unsigned long m_capacity; CArrayVB() : m_pData(0), m_count(0), m_capacity(0) { } };
struct C3DGroup2 { long BoneIndex; CArrayVB VertexBlends; C3DGroup2() : BoneIndex(-1) { } };
int main()
{
    C3DGroup2 g;
    if (sizeof(g) != 16 || g.BoneIndex != -1 || g.VertexBlends.m_pData != 0 || g.VertexBlends.m_count != 0 || g.VertexBlends.m_capacity != 0) { std::printf("00ac40c0_TEST FAIL\n"); return 1; }
    std::printf("00ac40c0_TEST PASS\n"); return 0;
}