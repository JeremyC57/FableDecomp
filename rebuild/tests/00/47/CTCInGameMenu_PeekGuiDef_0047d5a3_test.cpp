#include <cstdio>
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v000();
    virtual void DestroyDef();                           // slot 1 (+4): called when the count drops to zero
};
struct CRefCounted {
    long m_refCount;                                     // +4
};
class CDefPointeeBase : public CDefVirtual, public CRefCounted { public:
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
    T& operator*() const { return *m_pObject; }
};
class CPlayerGuiDef : public CDefPointeeBase {
};
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
    bool GetDef(const CCharString& type_name, long index, CDefPointer<CPlayerGuiDef>& out);   // 0x004858B7
};
void* FindDef_PlayerGui();                                // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<CPlayerGuiDef>& out)
{
    void* def = FindDef_PlayerGui();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
class CTCInGameMenu {
private:
    const CPlayerGuiDef& PeekGuiDef() const;
    friend struct CTCInGameMenu_Test;                    // behaviour-test hook
};
const CPlayerGuiDef& CTCInGameMenu::PeekGuiDef() const
{
    CDefPointer<CPlayerGuiDef> def;
    {
        const CCharString name("PLAYER_GUI");
        CGameDefinitionManager::Get().GetDef(name, 1, def);
    }
    return *def;
}

static int g_destroyed, g_strings_alive, g_strings_made;
void CDefVirtual::v000() {}
void CDefVirtual::DestroyDef() {}
static const char* g_last_text;
CCharString::CCharString(const char* text, long length) { PStringData = 0; g_last_text = text; ++g_strings_alive; ++g_strings_made; }
CCharString::~CCharString() { --g_strings_alive; }
struct TestDef : CPlayerGuiDef {
    void v000() {}
    void DestroyDef() { ++g_destroyed; }
};
static TestDef g_def;
void* FindDef_PlayerGui() { ++g_def.m_refCount; return &g_def; }
static CGameDefinitionManager g_manager;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_manager; }
struct CTCInGameMenu_Test {
    static const CPlayerGuiDef& Peek(const CTCInGameMenu& m) { return m.PeekGuiDef(); }
};
int main()
{
    CTCInGameMenu menu;
    g_def.m_refCount = 2;   // held elsewhere: the temporary's reference is dropped without destroying
    const CPlayerGuiDef& d = CTCInGameMenu_Test::Peek(menu);
    bool ok = &d == &g_def && g_def.m_refCount == 2 && g_destroyed == 0 && g_strings_alive == 0 && g_strings_made == 1
           && g_last_text[0] == 'P' && g_last_text[7] == 'G';
    g_def.m_refCount = 0;   // only the temporary holds it: dropped to zero, destroyed
    const CPlayerGuiDef& e = CTCInGameMenu_Test::Peek(menu);
    ok = ok && &e == &g_def && g_def.m_refCount == 0 && g_destroyed == 1;
    std::printf(ok ? "0047d5a3_TEST PASS\n" : "0047d5a3_TEST FAIL\n");
    return ok ? 0 : 1;
}