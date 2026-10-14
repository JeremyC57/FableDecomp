#include <cstdio>
struct CRenderable_00b32610 { int m_n; CRenderable_00b32610() : m_n(0) { } virtual void Render() { ++m_n; } };
struct SRenderEntry { CRenderable_00b32610* m_pObject; long m_key; };
struct CRetailEntryVector_00b32610 { SRenderEntry* m_pFirst; SRenderEntry* m_pLast; SRenderEntry* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; } };
struct CEnginePrimitiveRenderer { unsigned char _pad[0x274]; CRetailEntryVector_00b32610 m_nonsorted; void RenderNonsortedPrimitives() { for (unsigned long i = 0; i < m_nonsorted.GetCount(); ++i) m_nonsorted.m_pFirst[i].m_pObject->Render(); } };
int main() {
    CEnginePrimitiveRenderer* r = (CEnginePrimitiveRenderer*)new char[sizeof(CEnginePrimitiveRenderer)]; CRenderable_00b32610 a, b; SRenderEntry es[2] = { { &a, 0 }, { &b, 0 } };
    r->m_nonsorted.m_pFirst = es; r->m_nonsorted.m_pLast = es + 2; r->RenderNonsortedPrimitives(); if (a.m_n != 1 || b.m_n != 1) { std::printf("00b32610_TEST FAIL\n"); return 1; }
    r->m_nonsorted.m_pLast = es; r->RenderNonsortedPrimitives(); if (a.m_n != 1) { std::printf("00b32610_TEST FAIL\n"); return 1; }
    delete[] (char*)r;
    std::printf("00b32610_TEST PASS\n"); return 0;
}
