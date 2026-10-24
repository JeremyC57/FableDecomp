// behaviour test for 0x00C0EB40 CScriptHost::DeleteAllWatchers: delete the first Watcher of the list until the list is empty
#include <cstdio>

struct CWatcher { int m_id; };

struct CWatcherList {
    CWatcher* m_items[8];
    int m_count;
    CWatcherList() : m_count(0) { }
    virtual ~CWatcherList() { }
    virtual void Remove(CWatcher* item)
    {
        for (int i = 0; i < m_count; ++i)
            if (m_items[i] == item) { for (int j = i + 1; j < m_count; ++j) m_items[j - 1] = m_items[j]; --m_count; return; }
    }
    virtual void Add(CWatcher* item) { m_items[m_count++] = item; }
    virtual CWatcher* GetFirst() { return m_count ? m_items[0] : 0; }
};

static int g_deleted = 0, g_lastId = -1;

struct CScriptHost {
    CWatcherList m_watchers;
    void DeleteWatcher(CWatcher* item) { m_watchers.Remove(item); ++g_deleted; g_lastId = item->m_id; }   // stand-in for the out-of-line delete
    void DeleteAllWatchers();
};

void CScriptHost::DeleteAllWatchers()
{
    CWatcher* item = m_watchers.GetFirst();
    while (item) {
        DeleteWatcher(item);
        item = m_watchers.GetFirst();
    }
}

int main()
{
    CWatcher items[3] = { { 1 }, { 2 }, { 3 } };
    CScriptHost host;
    host.DeleteAllWatchers();
    if (g_deleted != 0) { std::printf("00c0eb40_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) host.m_watchers.Add(&items[i]);
    host.DeleteAllWatchers();
    if (g_deleted != 3 || g_lastId != 3 || host.m_watchers.m_count != 0) { std::printf("00c0eb40_TEST FAIL\n"); return 1; }
    std::printf("00c0eb40_TEST PASS\n");
    return 0;
}