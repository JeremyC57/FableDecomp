#include <cstdio>
struct CEngineStaticMapFileBlockManager { int id; };
static CEngineStaticMapFileBlockManager* g_cleared = 0;
struct CQuadTreeElement_Retail {
    unsigned char _body[0x4c];
    void ClearCache(CEngineStaticMapFileBlockManager* manager) { g_cleared = manager; }
};
struct SBlockEntry16 { unsigned char _body[0x10]; };
struct CRetailVector_SBlockEntry16 {
    SBlockEntry16* first; SBlockEntry16* last;
    SBlockEntry16* erase(SBlockEntry16* eraseFirst, SBlockEntry16* eraseLast)
    {
        SBlockEntry16* d = eraseFirst;
        for (SBlockEntry16* s = eraseLast; s != last; ++s, ++d) *d = *s;
        last = d;
        return eraseFirst;
    }
    void clear() { erase(first, last); }
};
struct CBlockCacheOwner_00bdf0d0 {
    virtual void v00() { }
    CEngineStaticMapFileBlockManager* m_pManager;
    unsigned char                     _pad08[4];
    CQuadTreeElement_Retail           m_element;
    CRetailVector_SBlockEntry16       m_entries;
    void Reset();
};
void CBlockCacheOwner_00bdf0d0::Reset()
{
    m_element.ClearCache(m_pManager);
    m_entries.clear();
}
int main()
{
    CEngineStaticMapFileBlockManager mgr = { 3 };
    SBlockEntry16 items[4];
    CBlockCacheOwner_00bdf0d0 o; o.m_pManager = &mgr; o.m_entries.first = items; o.m_entries.last = items + 4;
    o.Reset();
    if (g_cleared != &mgr || o.m_entries.last != items || reinterpret_cast<unsigned char*>(&o.m_entries) - reinterpret_cast<unsigned char*>(&o) != 0x58) { std::printf("00bdf0d0_TEST FAIL\n"); return 1; }
    std::printf("00bdf0d0_TEST PASS\n");
    return 0;
}
