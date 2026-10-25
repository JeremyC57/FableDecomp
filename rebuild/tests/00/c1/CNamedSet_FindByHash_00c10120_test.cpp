#include <cstdio>
class CNamedEntry { public: unsigned long key; };
static CNamedEntry g_entries[3] = { { 5 }, { 7 }, { 9 } };
struct CKeyed {
    virtual CNamedEntry* Find(const unsigned long& key) { for (int i = 0; i < 3; ++i) if (g_entries[i].key == key) return &g_entries[i]; return 0; }
};
class CNamedSet : public CKeyed {
public:
    unsigned char _pad[0x0c];
    CKeyed m_index;
    CNamedEntry* FindById(unsigned long id) { return Find(id); }
    CNamedEntry* FindByHash(unsigned long hash) { return m_index.Find(hash); }
};
int main()
{
    CNamedSet set;
    CNamedEntry* a = set.FindByHash(7);
    CNamedEntry* b = set.FindByHash(8);
    if (a != &g_entries[1] || b != 0) { std::printf("00c10120_TEST FAIL\n"); return 1; }
    std::printf("00c10120_TEST PASS\n");
    return 0;
}
