#include <cstdio>
enum FRONTEND_SCREEN { FRONTEND_SCREEN_22 = 22 };
namespace NUISystem {
class CComponent;
class CRedefinerList {
public:
    static bool __fastcall CanAcceptChanges();           // 0x00556540 (Ego_r.pdb SI: static __fastcall)
};
}
namespace std {
template <class T> class deque {
public:
    unsigned char _body[0x28];
    T& back();                                           // 0x0059B039 for NUISystem::CComponent*
};
template <class T, class Seq = deque<T> > class stack {
public:
    Seq c;
    T& top() { return c.back(); }
};
template <class K, class V> class map {
public:
    unsigned char _body[0x10];
    V& operator[](const K& key);                         // 0x0059B5D7 for FRONTEND_SCREEN -> CComponent*
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                                   _pad00[0x20];
    std::stack<CComponent*>                         m_stackScreens;     // +0x20
    unsigned char                                   m_vComponents[0xc]; // +0x48 vector<CComponent*>
    std::map<FRONTEND_SCREEN, CComponent*>          m_mapScreens;       // +0x54
    bool CanAcceptOptionChanges();
};
bool CFrontEndManager::CanAcceptOptionChanges()
{
    if (m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_22])
        return CRedefinerList::CanAcceptChanges();
    return true;
}
}

static NUISystem::CComponent* g_top; static NUISystem::CComponent* g_mapval; static FRONTEND_SCREEN g_key; static bool g_redef; static int g_calls;
bool __fastcall NUISystem::CRedefinerList::CanAcceptChanges() { ++g_calls; return g_redef; }
template <> NUISystem::CComponent*& std::deque<NUISystem::CComponent*>::back() { return g_top; }
template <> NUISystem::CComponent*& std::map<FRONTEND_SCREEN, NUISystem::CComponent*>::operator[](const FRONTEND_SCREEN& k) { g_key = k; return g_mapval; }
int main()
{
    static double mem[0x80 / 8];
    NUISystem::CFrontEndManager* m = (NUISystem::CFrontEndManager*)mem;
    NUISystem::CComponent* a = (NUISystem::CComponent*)mem; NUISystem::CComponent* b = (NUISystem::CComponent*)(mem + 1);
    g_top = a; g_mapval = b;
    bool ok = m->CanAcceptOptionChanges() == true && g_calls == 0 && g_key == 22;
    g_mapval = a; g_redef = false;
    ok = ok && m->CanAcceptOptionChanges() == false && g_calls == 1;
    g_redef = true;
    ok = ok && m->CanAcceptOptionChanges() == true && g_calls == 2;
    std::printf(ok ? "005966f2_TEST PASS\n" : "FAIL\n");
    return 0;
}