// Behaviour test for 0x00698840 CGameCameraManager::PreparePendingMode.
#include <cstdio>
struct M;
static int g_destroyed = 0;
class CCameraModeDef { public: int m_count; int Mode; void Destroy() { ++g_destroyed; } };
struct CDefPointer {
    CCameraModeDef* m_p;
    CDefPointer(CCameraModeDef* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    CCameraModeDef* operator->() const { return m_p; }
    ~CDefPointer() { if (m_p) { --m_p->m_count; if (m_p->m_count == 0) m_p->Destroy(); } }
};
struct CBase { int kind; };
struct CBaseInit {
    CBase made; M* lastManager; CCameraModeDef* lastDef;
    CBase* AllocMain(M* manager, CDefPointer& def) { lastManager = manager; lastDef = def.m_p; return &made; }
};
struct Node { int first; CBaseInit* second; };
struct CModeMap {
    Node nodes[2];
    Node* find(const int& key) { for (int i = 0; i < 2; ++i) if (nodes[i].first == key) return &nodes[i]; return 0; }
};
struct CPending { CBase* p; void Reset(CBase* o) { p = o; } };
struct M {
    CPending PendingMode; CModeMap RegisteredModes;
    void PreparePendingMode(CDefPointer def)
    {
        int mode = def->Mode;
        Node* it = RegisteredModes.find(mode);
        PendingMode.Reset(it->second->AllocMain(this, def));
    }
};
int main()
{
    CBaseInit i1, i2; i1.made.kind = 1; i2.made.kind = 2;
    M m; m.PendingMode.p = 0;
    m.RegisteredModes.nodes[0].first = 3; m.RegisteredModes.nodes[0].second = &i1;
    m.RegisteredModes.nodes[1].first = 7; m.RegisteredModes.nodes[1].second = &i2;
    CCameraModeDef* def = new CCameraModeDef; def->m_count = 0; def->Mode = 7;
    {
        CDefPointer p(def);
        m.PreparePendingMode(p);
        if (def->m_count != 1) { std::printf("00698840_TEST FAIL\n"); return 1; }
    }
    if (m.PendingMode.p != &i2.made || i2.lastManager != &m || i2.lastDef != def || g_destroyed != 1) { std::printf("00698840_TEST FAIL\n"); return 1; }
    delete def;
    std::printf("00698840_TEST PASS\n");
    return 0;
}