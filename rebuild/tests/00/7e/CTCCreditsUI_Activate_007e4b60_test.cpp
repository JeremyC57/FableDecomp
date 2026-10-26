// Behaviour test for CTCCreditsUI::Activate (0x007E4B60): an active screen ignores the call; otherwise the part is stored,
// a component is created from it (not as the second flag), reset, put into state 1, appended (growing when full); Active set.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_strDtors = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* t) : m_pData(t) { }
    CCharString(const CCharString& o) : m_pData(o.m_pData) { }
    ~CCharString() { ++g_strDtors; }
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
};
static char g_log[32];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
namespace NUISystem {
class CComponent { public:
    const char* Def; long State;
    virtual void Reset() { Log('r'); }
    virtual void ChangeState(long state) { State = state; Log('s'); }
};
static CComponent g_made[4]; static int g_count = 0; static bool g_lastFlag = true;
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b) { g_lastFlag = b; g_made[g_count].Def = definition.m_pData; Log('c'); return &g_made[g_count++]; }
};
class CFrontEndManager : public CManager { public:
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
};
}
struct __true_type { __true_type() { } };
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
    {
        unsigned long oldSize = _M_finish - _M_start;
        unsigned long len = oldSize + (oldSize < fill_len ? fill_len : oldSize);
        T* newStart = (T*)std::malloc(len * sizeof(T));
        T* newFinish = newStart;
        for (T* p = _M_start; p != position; ++p, ++newFinish) *newFinish = *p;
        for (unsigned long i = 0; i < fill_len; ++i, ++newFinish) *newFinish = x;
        _M_start = newStart; _M_finish = newFinish; _M_end_of_storage = newStart + len;
        Log('o');
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
class CTCBase { public:
    virtual void v00() { }
    void* ParentThing;
};
class CTCCreditsUI : public CTCBase { public:
    bool Active;
    CRetailVector<NUISystem::CComponent*> Components;
    bool Quitting;
    CCharString CreditPart;
    CTCCreditsUI() : CreditPart("") { }
    void Activate(CCharString part);
};

void CTCCreditsUI::Activate(CCharString part)
{
    if (Active)
        return;
    CreditPart = part;
    NUISystem::CComponent* component = NUISystem::CFrontEndManager::GetInstance()->CreateComponent(CreditPart, false);
    component->Reset();
    component->ChangeState(1);
    Components.push_back(component);
    Active = true;
}

int main()
{
    int bad = 0;
    NUISystem::CComponent* store[1];
    CTCCreditsUI ui; ui.Active = false; ui.Quitting = false;
    ui.Components._M_start = store; ui.Components._M_finish = store; ui.Components._M_end_of_storage = store + 1;
    ui.Activate(CCharString("CREDITS_A"));
    if (std::strcmp(g_log, "crs") || !ui.Active || std::strcmp(ui.CreditPart.m_pData, "CREDITS_A") || NUISystem::g_lastFlag
        || store[0] != &NUISystem::g_made[0] || ui.Components._M_finish != store + 1 || NUISystem::g_made[0].State != 1
        || std::strcmp(NUISystem::g_made[0].Def, "CREDITS_A")) bad |= 1;
    g_log[0] = 0;
    ui.Activate(CCharString("CREDITS_B"));           // already active
    if (g_log[0] || std::strcmp(ui.CreditPart.m_pData, "CREDITS_A")) bad |= 2;
    ui.Active = false; g_log[0] = 0;
    ui.Activate(CCharString("CREDITS_B"));           // full vector: grows
    if (std::strcmp(g_log, "crso") || ui.Components._M_finish - ui.Components._M_start != 2 || ui.Components._M_start[1] != &NUISystem::g_made[1]
        || ui.Components._M_start[0] != &NUISystem::g_made[0]) bad |= 4;
    std::free(ui.Components._M_start);
    if (bad) { std::printf("007e4b60_TEST FAIL\n"); return 1; }
    std::printf("007e4b60_TEST PASS\n");
    return 0;
}