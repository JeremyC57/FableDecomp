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
    case 0x25: return new CPrim(0x25, 0x80000000u, false); case 0x26: return new CPrim(0x26, 0x80000000u, true);
    }
    return 0;
}
int main()
{
    int bad = 0; CPrim* p = 0;
    p = Create(0x25); if (!p || p->m_type != 0x25 || p->m_hasMorph || !p->Valid()) bad = 1; delete p; p = Create(0x26); if (!p || p->m_type != 0x26 || !p->m_hasMorph || !p->Valid()) bad = 1; delete p; if (Create(9) || Create(0x24) || Create(0x27)) bad = 1;
    if (bad) { std::printf("00bd6b90_TEST FAIL\n"); return 1; }
    std::printf("00bd6b90_TEST PASS\n");
    return 0;
}