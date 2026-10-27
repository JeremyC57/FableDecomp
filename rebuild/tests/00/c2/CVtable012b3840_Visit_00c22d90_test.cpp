#include <cstdio>

// Behaviour model of the find-by-key visitor (0x00C22D90): remember the entry whose key word (+0x20) equals the key.
struct Entry { unsigned char _pad[0x20]; int m_key; };
struct Visitor {
    Entry* m_pFound; int m_key;
    void Visit(Entry* e) { if (e->m_key == m_key) m_pFound = e; }
};

int main()
{

    Entry e1; e1.m_key = 5; Entry e2; e2.m_key = 7; Entry e3; e3.m_key = 9;
    Visitor v; v.m_pFound = 0; v.m_key = 7;
    v.Visit(&e1); if (v.m_pFound) { std::printf("00c22d90_TEST FAIL\n"); return 1; }
    v.Visit(&e2); if (v.m_pFound != &e2) { std::printf("00c22d90_TEST FAIL\n"); return 1; }
    v.Visit(&e3); if (v.m_pFound != &e2) { std::printf("00c22d90_TEST FAIL\n"); return 1; }
    std::printf("00c22d90_TEST PASS\n");
    return 0;
}