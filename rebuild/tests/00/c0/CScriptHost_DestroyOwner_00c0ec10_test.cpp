// behaviour test for 0x00C0EC10 CScriptHost::DestroyOwner: delete every script the owner holds (Find by owner), then the owner itself
#include <cstdio>

static int g_ownersDeleted = 0, g_scriptsDeleted = 0;

struct CScriptOwnerThing {
    virtual ~CScriptOwnerThing() { ++g_ownersDeleted; }
};

struct CScript { CScriptOwnerThing* m_pOwner; };

struct CScriptList {
    CScript* m_items[8];
    int m_count;
    CScriptList() : m_count(0) { }
    virtual ~CScriptList() { }
    virtual void Remove(CScript* item)
    {
        for (int i = 0; i < m_count; ++i)
            if (m_items[i] == item) { for (int j = i + 1; j < m_count; ++j) m_items[j - 1] = m_items[j]; --m_count; return; }
    }
    virtual void Add(CScript* item) { m_items[m_count++] = item; }
    virtual CScript* Find(CScriptOwnerThing* const& key)
    {
        for (int i = 0; i < m_count; ++i) if (m_items[i]->m_pOwner == key) return m_items[i];
        return 0;
    }
};

struct CScriptHost {
    CScriptList m_scripts;
    void DeleteScript(CScript* script) { m_scripts.Remove(script); ++g_scriptsDeleted; }   // stand-in for 0x00C0EAD0
    void DestroyOwner(CScriptOwnerThing* owner);
};

void CScriptHost::DestroyOwner(CScriptOwnerThing* owner)
{
    CScript* script = m_scripts.Find(owner);
    while (script) {
        DeleteScript(script);
        script = m_scripts.Find(owner);
    }
    delete owner;
}

int main()
{
    CScriptOwnerThing* a = new CScriptOwnerThing;
    CScriptOwnerThing* b = new CScriptOwnerThing;
    CScript s[4] = { { a }, { b }, { a }, { a } };
    CScriptHost host;
    for (int i = 0; i < 4; ++i) host.m_scripts.Add(&s[i]);
    host.DestroyOwner(a);
    if (g_scriptsDeleted != 3 || g_ownersDeleted != 1 || host.m_scripts.m_count != 1 || host.m_scripts.m_items[0] != &s[1]) { std::printf("00c0ec10_TEST FAIL\n"); return 1; }
    host.DestroyOwner(b);
    if (g_scriptsDeleted != 4 || g_ownersDeleted != 2 || host.m_scripts.m_count != 0) { std::printf("00c0ec10_TEST FAIL\n"); return 1; }
    host.DestroyOwner(0);                                         // delete of null is a no-op
    if (g_ownersDeleted != 2) { std::printf("00c0ec10_TEST FAIL\n"); return 1; }
    std::printf("00c0ec10_TEST PASS\n");
    return 0;
}