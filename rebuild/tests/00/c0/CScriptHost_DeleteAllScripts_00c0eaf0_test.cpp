// behaviour test for 0x00C0EAF0 CScriptHost::DeleteAllScripts: delete the first Script of the list until the list is empty
#include <cstdio>

struct CScript { int m_id; };

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
    virtual CScript* GetFirst() { return m_count ? m_items[0] : 0; }
};

static int g_deleted = 0, g_lastId = -1;

struct CScriptHost {
    CScriptList m_scripts;
    void DeleteScript(CScript* item) { m_scripts.Remove(item); ++g_deleted; g_lastId = item->m_id; }   // stand-in for the out-of-line delete
    void DeleteAllScripts();
};

void CScriptHost::DeleteAllScripts()
{
    CScript* item = m_scripts.GetFirst();
    while (item) {
        DeleteScript(item);
        item = m_scripts.GetFirst();
    }
}

int main()
{
    CScript items[3] = { { 1 }, { 2 }, { 3 } };
    CScriptHost host;
    host.DeleteAllScripts();
    if (g_deleted != 0) { std::printf("00c0eaf0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) host.m_scripts.Add(&items[i]);
    host.DeleteAllScripts();
    if (g_deleted != 3 || g_lastId != 3 || host.m_scripts.m_count != 0) { std::printf("00c0eaf0_TEST FAIL\n"); return 1; }
    std::printf("00c0eaf0_TEST PASS\n");
    return 0;
}