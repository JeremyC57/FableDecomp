// Behaviour test for 004384f0: src/004384f0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPlayerGui::PeekGlobalDef()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPlayerGui::PeekGlobalDef() -- look up the "GLOBAL" definition and return it without keeping a reference (null when absent)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004384F0, length 123 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0
// (`__fastcall`, static), the definition manager's name lookup @ 0x009ADE10 (a CDefPointer by value through the hidden
// pointer; manifest label GetPDefFromGlobalIndex), ~CCharString @ 0x0099EAE0, the def's virtual Destroy (slot 1).
// String 0x01231734 "GLOBAL".
// Shape: the result counted pointer starts null (esi); on a hit it takes a reference, the lookup temporary is
// released by hand and nulled; the name dies, then the result's reference is dropped and its raw pointer returned.
// The manager reference is a named local (Get() runs before the GetDef pushes); the hand release calls the def's own
// inline Release() member (`if (--m_count == 0) Destroy();`), which gives retail's `mov ecx,eax ; add eax,4 ; dec edx`
// form (the object copied to ecx for the virtual call, the count addressed through eax).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long m_count;                                           // +0x04
    void Release() { if (--m_count == 0) Destroy(); }
};

struct CDefPointer {
    CDefBase* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_count == 0)
            m_p->Destroy();
    }
    void Reset()                                            // the hand-written release of the lookup temporary
    {
        m_p->Release();
        m_p = 0;
    }
};

class CDefinitionManager { public:
    CDefPointer GetDef(const CCharString& name, bool flag); // 0x009ADE10
};

class CGameDefinitionManager { public:
    static CDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

class CPlayerGui { public:
    static CDefBase* PeekGlobalDef();
};

CDefBase* CPlayerGui::PeekGlobalDef()
{
    CDefPointer result;
    {
        CCharString name("GLOBAL", -1);
        CDefinitionManager& manager = CGameDefinitionManager::Get();
        CDefPointer def = manager.GetDef(name, true);
        if (def.m_p) {
            result.m_p = def.m_p;
            ++def.m_p->m_count;
            def.Reset();
        }
    }
    CDefBase* p = result.m_p;
    return p;
}

// ---- stubs and driver ----
static int g_names = 0, g_nameDtors = 0, g_lookups = 0, g_destroys = 0;
static const char* g_lastName = 0;
static bool g_lastFlag = false;
CCharString::CCharString(const char* text, long length) { ++g_names; g_lastName = text; PStringData = (void*)text; (void)length; }
CCharString::~CCharString() { ++g_nameDtors; }
void CDefBase::v00() {}
void CDefBase::Destroy() { ++g_destroys; }
static CDefBase* g_def = 0;
CDefPointer CDefinitionManager::GetDef(const CCharString& name, bool flag)
{
    ++g_lookups; g_lastFlag = flag;
    CDefPointer r;                       // VC7.1 copies a named return and destroys the local: take two references
    if (g_def && strcmp((const char*)name.PStringData, "GLOBAL") == 0) { r.m_p = g_def; g_def->m_count += 2; }
    return r;
}
static CDefinitionManager g_manager;
CDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_manager; }
int main()
{
    // absent: null, no destroy
    CHECK(CPlayerGui::PeekGlobalDef() == 0 && g_lookups == 1 && g_lastFlag && g_names == 1 && g_nameDtors == 1);
    // present with an owner reference: returned, count back to the owner's 1, not destroyed
    CDefBase def; def.m_count = 1; g_def = &def;
    CHECK(CPlayerGui::PeekGlobalDef() == &def && def.m_count == 1 && g_destroys == 0 && g_nameDtors == 2);
    // present but unowned: the lookup's and result's references drop to zero -> destroyed once
    def.m_count = 0;
    CHECK(CPlayerGui::PeekGlobalDef() == &def && def.m_count == 0 && g_destroys == 1);
    if (fails == 0) printf("004384f0_TEST PASS\n");
    return fails;
}
