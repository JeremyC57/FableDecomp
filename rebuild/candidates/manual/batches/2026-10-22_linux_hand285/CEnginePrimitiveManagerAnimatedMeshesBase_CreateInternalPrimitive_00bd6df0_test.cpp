#include <cstdio>
#include <cstdlib>
struct SNode { unsigned char m_color; SNode* m_parent; SNode* m_left; SNode* m_right; };
struct CPrim {
    long m_type; unsigned long m_flags; long m_mesh; void* m_sim; unsigned char m_theme[3];
    SNode* m_header; unsigned long m_count; long m_renderCount; long m_renderIndex; bool m_hasMorph;
    CPrim(long type, unsigned long flags, bool morph)
        : m_type(type), m_flags(flags), m_mesh(0), m_sim(0), m_header(0), m_renderCount(0), m_renderIndex(-1), m_hasMorph(morph)
    {
        m_theme[0] = m_theme[1] = m_theme[2] = 0;
        m_header = (SNode*)std::malloc(sizeof(SNode));
        m_count = 0;
        m_header->m_color = 0; m_header->m_parent = 0; m_header->m_left = m_header; m_header->m_right = m_header;
    }
    ~CPrim() { std::free(m_header); }
    bool Valid() const
    {
        return m_mesh == 0 && m_sim == 0 && m_theme[2] == 0 && m_count == 0 && m_header->m_left == m_header
            && m_header->m_right == m_header && m_header->m_parent == 0 && m_renderCount == 0 && m_renderIndex == -1;
    }
};
CPrim* Create(long type)
{
    switch (type) {
    case 9: return new CPrim(9, 0xa, false); case 0xb: return new CPrim(0xb, 0xa, true); case 0xd: return new CPrim(0xd, 8, false);
    }
    return 0;
}
int main()
{
    int bad = 0; CPrim* p = 0;
    p = Create(9); if (!p || p->m_flags != 0xa || p->m_hasMorph || !p->Valid()) bad = 1; delete p; p = Create(0xb); if (!p || p->m_flags != 0xa || !p->m_hasMorph || !p->Valid()) bad = 1; delete p; p = Create(0xd); if (!p || p->m_flags != 8 || !p->Valid()) bad = 1; delete p; if (Create(0xa) || Create(0xc) || Create(0x25)) bad = 1;
    if (bad) { std::printf("00bd6df0_TEST FAIL\n"); return 1; }
    std::printf("00bd6df0_TEST PASS\n");
    return 0;
}
