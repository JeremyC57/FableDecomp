#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static long g_base = 0; struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct SEdgeStripState { unsigned short m_firstVertex; unsigned short m_vertexCount; long m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24; bool m_flag28; bool m_flag29;
    SEdgeStripState() { m_8 = 0; m_c = 0; m_10 = 0; m_14 = 0; m_18 = 0; m_1c = 0; m_20 = 0; m_24 = 0; m_firstVertex = 0; m_vertexCount = 0; m_flag28 = false; m_flag29 = false; } };
class CPatchTesselationEdgeStrip : public CBase { public: virtual void v00() { } SEdgeStripState m_state; CPatchTesselationEdgeStrip() : CBase(), m_state() { } };

int main()
{
    void* raw = std::malloc(sizeof(CPatchTesselationEdgeStrip)); std::memset(raw, 0xab, sizeof(CPatchTesselationEdgeStrip)); CPatchTesselationEdgeStrip* s = new (raw) CPatchTesselationEdgeStrip; SEdgeStripState& st = s->m_state;
    if (g_base != 1 || st.m_firstVertex || st.m_vertexCount || st.m_8 || st.m_c || st.m_10 || st.m_14 || st.m_18 || st.m_1c || st.m_20 || st.m_24 || st.m_flag28 || st.m_flag29) { std::printf("00bfc370_TEST FAIL\n"); return 1; } std::free(raw);
    std::printf("00bfc370_TEST PASS\n");
    return 0;
}