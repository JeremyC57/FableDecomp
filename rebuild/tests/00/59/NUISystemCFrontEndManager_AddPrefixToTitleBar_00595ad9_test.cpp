#include <cstdio>
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};
class CWideString;
enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };
namespace std {
template <class K, class V> class map {
public:
    void*        _M_header;
    unsigned int _M_node_count;
    unsigned int _M_key_compare;
    V& operator[](const K& key);                         // 0x0059B5D7 for <FRONTEND_SCREEN, CComponent*>
};
}
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();
    virtual void _slot01(); virtual void _slot02();
    virtual CComponent* FindChild(CCharString name);     // slot 3 (+0x0c)
};
class CText : public CComponent {
public:
    void ResetToDefaultValueWithPrefix(const CWideString& prefix);   // 0x0054F580
};
class CFrontEndManager {
public:
    unsigned char                            _pad00[0x54];
    std::map<FRONTEND_SCREEN, CComponent*>   m_mapScreens;   // +0x54
protected:
    void AddPrefixToTitleBar(FRONTEND_SCREEN screen, const CCharString& title_bar_name, const CWideString& prefix);
    friend struct CFrontEndManager_Test;                 // behaviour-test hook
};
void CFrontEndManager::AddPrefixToTitleBar(FRONTEND_SCREEN screen, const CCharString& title_bar_name, const CWideString& prefix)
{
    if (m_mapScreens[screen])
    {
        CText* title = static_cast<CText*>(m_mapScreens[screen]->FindChild(title_bar_name));
        if (title)
            title->ResetToDefaultValueWithPrefix(prefix);
    }
}
}


class CWideString { public: int Id; };
static int g_copies, g_index_calls, g_find; static const CWideString* g_prefix; static void* g_found_name;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
NUISystem::CComponent::~CComponent() {}
void NUISystem::CComponent::_slot01() {} void NUISystem::CComponent::_slot02() {}
NUISystem::CComponent* NUISystem::CComponent::FindChild(CCharString name) { return 0; }
void NUISystem::CText::ResetToDefaultValueWithPrefix(const CWideString& prefix) { g_prefix = &prefix; }
class CScreenTest : public NUISystem::CComponent {
public:
    NUISystem::CText* Title;
    virtual NUISystem::CComponent* FindChild(CCharString name) { ++g_find; g_found_name = name.PStringData; return Title; }
};
static NUISystem::CComponent* g_slots[4];
template <> NUISystem::CComponent*& std::map<FRONTEND_SCREEN, NUISystem::CComponent*>::operator[](const FRONTEND_SCREEN& key) { ++g_index_calls; return g_slots[key]; }
namespace NUISystem {
struct CFrontEndManager_Test {
    static void Run(CFrontEndManager& m, int s, const CCharString& n, const CWideString& p) { m.AddPrefixToTitleBar((FRONTEND_SCREEN)s, n, p); }
};
}
int main()
{
    static NUISystem::CFrontEndManager m;
    NUISystem::CText title; CScreenTest screen; screen.Title = &title; CScreenTest empty; empty.Title = 0;
    g_slots[1] = &screen; g_slots[2] = &empty;
    CCharString name(*(CCharString*)&g_slots); name.PStringData = (CCharStringData*)0x1234; g_copies = 0;
    CWideString prefix; prefix.Id = 7;
    NUISystem::CFrontEndManager_Test::Run(m, 1, name, prefix);
    bool ok = g_prefix == &prefix && g_index_calls == 2 && g_find == 1 && g_copies == 1 && g_found_name == (void*)0x1234;
    g_prefix = 0; NUISystem::CFrontEndManager_Test::Run(m, 2, name, prefix); ok = ok && g_prefix == 0 && g_find == 2;
    g_index_calls = 0; NUISystem::CFrontEndManager_Test::Run(m, 0, name, prefix); ok = ok && g_index_calls == 1 && g_find == 2;
    if (ok) std::printf("00595ad9_TEST PASS\n");
    return 0;
}