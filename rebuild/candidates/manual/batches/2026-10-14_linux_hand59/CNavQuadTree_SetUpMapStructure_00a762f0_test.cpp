#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C2DVector { float X, Y; };
#pragma pack(push, 1)
struct CNavQuadTree { long m_0; bool m_setUp; unsigned char _pad[2]; float m_originX, m_originY; unsigned char _p; long m_10, m_14, m_18; void SetUpMapStructure(const C2DVector* o, long a, long b, long c) { m_originX = o->X; m_originY = o->Y; m_10 = a; m_14 = b; m_18 = c; m_setUp = true; } };
#pragma pack(pop)

int main() {
    CNavQuadTree* t = (CNavQuadTree*)std::calloc(1, sizeof(CNavQuadTree)); C2DVector o; o.X = 1.5f; o.Y = 2.5f; t->SetUpMapStructure(&o, 3, 4, 5); if (!t->m_setUp || t->m_originX != 1.5f || t->m_originY != 2.5f || t->m_10 != 3 || t->m_14 != 4 || t->m_18 != 5) { std::printf("00a762f0_TEST FAIL\n"); return 1; } std::free(t);
    std::printf("00a762f0_TEST PASS\n"); return 0;
}
