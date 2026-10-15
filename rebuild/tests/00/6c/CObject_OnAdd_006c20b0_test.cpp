#include <cstdio>
struct EditMap { long changed[8]; int n; void SetMapAsChanged(long m, bool c) { if (c) changed[n++] = m; } };
struct Loader { EditMap* m_pEditWorldMap; };
struct Entry { long MapIndex; unsigned char pad[24]; };
struct Obj {
    Loader* m_pLevelLoader; Entry* b; Entry* e;
    unsigned int size() { return (unsigned int)(e - b); }
    void OnAdd(Loader* l) { m_pLevelLoader = l; for (unsigned int i = 0; i < size(); ++i) m_pLevelLoader->m_pEditWorldMap->SetMapAsChanged(b[i].MapIndex, true); }
};
int main()
{
    EditMap em; em.n = 0; Loader l; l.m_pEditWorldMap = &em;
    Entry es[3]; es[0].MapIndex = 4; es[1].MapIndex = 9; es[2].MapIndex = 2;
    Obj o; o.m_pLevelLoader = 0; o.b = es; o.e = es + 3;
    o.OnAdd(&l);
    if (o.m_pLevelLoader != &l || em.n != 3 || em.changed[0] != 4 || em.changed[1] != 9 || em.changed[2] != 2) { std::printf("006c20b0_TEST FAIL\n"); return 1; }
    std::printf("006c20b0_TEST PASS\n");
    return 0;
}