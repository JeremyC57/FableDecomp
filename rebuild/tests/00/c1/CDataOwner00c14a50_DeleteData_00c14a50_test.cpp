// Behaviour test for 00c14a50: DeleteData -- unregisters the data object from both registries (A first, then B) and deletes it through its virtual destructor
#include <cstdio>

static char g_log[16];
static int g_pos = 0;
static void Log(char c) { g_log[g_pos++] = c; g_log[g_pos] = 0; }

class CData {
public:
    virtual ~CData() { Log('D'); }
};
class CDerivedData : public CData {
public:
    ~CDerivedData() { Log('d'); }
};

struct CRegistryA { CData* m_removed; void Remove(CData* data) { m_removed = data; Log('A'); } };
struct CRegistryB { CData* m_removed; void Remove(CData* data) { m_removed = data; Log('B'); } };

struct CDataOwner {
    CRegistryA* m_pRegistryA;
    CRegistryB* m_pRegistryB;
    void DeleteData(CData* data)
    {
        m_pRegistryA->Remove(data);
        m_pRegistryB->Remove(data);
        delete data;
    }
};

int main()
{
    CRegistryA a = { 0 };
    CRegistryB b = { 0 };
    CDataOwner owner = { &a, &b };
    CData* data = new CDerivedData;
    owner.DeleteData(data);
    if (a.m_removed != data || b.m_removed != data) { std::printf("00c14a50_TEST FAIL\n"); return 1; }
    const char* expect = "ABdD";           // registries in order, then the most-derived destructor chain
    for (int i = 0; expect[i] || g_log[i]; ++i)
        if (expect[i] != g_log[i]) { std::printf("00c14a50_TEST FAIL\n"); return 1; }
    std::printf("00c14a50_TEST PASS\n");
    return 0;
}