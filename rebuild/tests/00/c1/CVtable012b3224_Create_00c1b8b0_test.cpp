#include <cstdio>

// Behaviour model of CVtable_012b3224::Create (0x00C1B8B0): wrap the factory's object in a new handle over
// (this, object); NULL when the factory has none.
struct Handle {
    void* m_pSecond; void* m_pFirst;
    Handle(void* first, void* second) : m_pSecond(second), m_pFirst(first) { }
};
struct Factory {
    int m_object;
    void* Find(int key) { return key == 1 ? (void*)&m_object : 0; }
};
struct Creator {
    Factory* m_pFactory;
    Handle* Create(int key) { void* o = m_pFactory->Find(key); if (!o) return 0; return new Handle(this, o); }
};

int main()
{

    Factory f; Creator c; c.m_pFactory = &f;
    if (c.Create(2) != 0) { std::printf("00c1b8b0_TEST FAIL\n"); return 1; }
    Handle* h = c.Create(1);
    if (!h || h->m_pFirst != (void*)&c || h->m_pSecond != (void*)&f.m_object) { std::printf("00c1b8b0_TEST FAIL\n"); return 1; }
    delete h;
    std::printf("00c1b8b0_TEST PASS\n");
    return 0;
}