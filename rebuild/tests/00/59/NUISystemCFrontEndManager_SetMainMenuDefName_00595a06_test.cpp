#include <cstdio>
class CCharString {
public:
    void* PData;
    ~CCharString();                                      // 0x0099EAE0
    bool operator!=(const CCharString& other) const;     // 0x0099E900
};
namespace std {
template <class K, class V> class map {
public:
    void* _body[3];
    V& operator[](const K& key);                         // 0x0059B5D7 for <FRONTEND_SCREEN, CComponent*>
};
}
enum FRONTEND_SCREEN { FRONTEND_SCREEN_MAIN_MENU = 0 };
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();                               // slot 0
    virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006();
    virtual void _v007(); virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012();
    virtual void _v013(); virtual void _v014(); virtual void _v015(); virtual void _v016(); virtual void _v017(); virtual void _v018();
    virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023(); virtual void _v024();
    virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030();
    virtual void _v031(); virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036();
    virtual void _v037(); virtual void _v038(); virtual void _v039(); virtual void _v040(); virtual void _v041(); virtual void _v042();
    virtual void Activate();                             // slot 43 (+0xac)
    virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047(); virtual void _v048(); virtual void _v049();
    virtual void _v050(); virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055();
    virtual void _v056(); virtual void _v057(); virtual void _v058(); virtual void _v059(); virtual void _v060(); virtual void _v061();
    virtual void _v062(); virtual void _v063(); virtual void _v064(); virtual void _v065(); virtual void _v066(); virtual void _v067();
    virtual void _v068(); virtual void _v069(); virtual void _v070(); virtual void _v071(); virtual void _v072(); virtual void _v073();
    virtual void _v074(); virtual void _v075(); virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079();
    virtual void _v080(); virtual void _v081(); virtual void _v082(); virtual void _v083();
    virtual CCharString GetDefName() const;              // slot 84 (+0x150)
};
class CManager {
public:
    CComponent* CreateComponent(const CCharString& defName, bool frontEnd);   // 0x0041DB1D
    static CManager* GetInstance();                      // 0x0041E5F2
};
class CFrontEndManager {
public:
    unsigned char                          _pad00[0x54];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;     // +0x54
    void SetMainMenuDefName(const CCharString& name);
};
void CFrontEndManager::SetMainMenuDefName(const CCharString& name)
{
    if (m_mapScreens[FRONTEND_SCREEN_MAIN_MENU])
    {
        if (m_mapScreens[FRONTEND_SCREEN_MAIN_MENU]->GetDefName() != name)
        {
            delete m_mapScreens[FRONTEND_SCREEN_MAIN_MENU];
            m_mapScreens[FRONTEND_SCREEN_MAIN_MENU] = 0;
        }
    }
    if (!m_mapScreens[FRONTEND_SCREEN_MAIN_MENU])
    {
        m_mapScreens[FRONTEND_SCREEN_MAIN_MENU] = CManager::GetInstance()->CreateComponent(name, false);
        m_mapScreens[FRONTEND_SCREEN_MAIN_MENU]->Activate();
    }
}
}

#include <stdio.h>
#include <string.h>
struct Str { const char* s; };
static Str g_strs[16]; static int g_nstr; static int g_dtors;
CCharString::~CCharString() { ++g_dtors; }
bool CCharString::operator!=(const CCharString& o) const { return strcmp(((Str*)PData)->s, ((Str*)o.PData)->s) != 0; }
void NUISystem::CComponent::_v001() {}
void NUISystem::CComponent::_v002() {}
void NUISystem::CComponent::_v003() {}
void NUISystem::CComponent::_v004() {}
void NUISystem::CComponent::_v005() {}
void NUISystem::CComponent::_v006() {}
void NUISystem::CComponent::_v007() {}
void NUISystem::CComponent::_v008() {}
void NUISystem::CComponent::_v009() {}
void NUISystem::CComponent::_v010() {}
void NUISystem::CComponent::_v011() {}
void NUISystem::CComponent::_v012() {}
void NUISystem::CComponent::_v013() {}
void NUISystem::CComponent::_v014() {}
void NUISystem::CComponent::_v015() {}
void NUISystem::CComponent::_v016() {}
void NUISystem::CComponent::_v017() {}
void NUISystem::CComponent::_v018() {}
void NUISystem::CComponent::_v019() {}
void NUISystem::CComponent::_v020() {}
void NUISystem::CComponent::_v021() {}
void NUISystem::CComponent::_v022() {}
void NUISystem::CComponent::_v023() {}
void NUISystem::CComponent::_v024() {}
void NUISystem::CComponent::_v025() {}
void NUISystem::CComponent::_v026() {}
void NUISystem::CComponent::_v027() {}
void NUISystem::CComponent::_v028() {}
void NUISystem::CComponent::_v029() {}
void NUISystem::CComponent::_v030() {}
void NUISystem::CComponent::_v031() {}
void NUISystem::CComponent::_v032() {}
void NUISystem::CComponent::_v033() {}
void NUISystem::CComponent::_v034() {}
void NUISystem::CComponent::_v035() {}
void NUISystem::CComponent::_v036() {}
void NUISystem::CComponent::_v037() {}
void NUISystem::CComponent::_v038() {}
void NUISystem::CComponent::_v039() {}
void NUISystem::CComponent::_v040() {}
void NUISystem::CComponent::_v041() {}
void NUISystem::CComponent::_v042() {}
void NUISystem::CComponent::_v044() {}
void NUISystem::CComponent::_v045() {}
void NUISystem::CComponent::_v046() {}
void NUISystem::CComponent::_v047() {}
void NUISystem::CComponent::_v048() {}
void NUISystem::CComponent::_v049() {}
void NUISystem::CComponent::_v050() {}
void NUISystem::CComponent::_v051() {}
void NUISystem::CComponent::_v052() {}
void NUISystem::CComponent::_v053() {}
void NUISystem::CComponent::_v054() {}
void NUISystem::CComponent::_v055() {}
void NUISystem::CComponent::_v056() {}
void NUISystem::CComponent::_v057() {}
void NUISystem::CComponent::_v058() {}
void NUISystem::CComponent::_v059() {}
void NUISystem::CComponent::_v060() {}
void NUISystem::CComponent::_v061() {}
void NUISystem::CComponent::_v062() {}
void NUISystem::CComponent::_v063() {}
void NUISystem::CComponent::_v064() {}
void NUISystem::CComponent::_v065() {}
void NUISystem::CComponent::_v066() {}
void NUISystem::CComponent::_v067() {}
void NUISystem::CComponent::_v068() {}
void NUISystem::CComponent::_v069() {}
void NUISystem::CComponent::_v070() {}
void NUISystem::CComponent::_v071() {}
void NUISystem::CComponent::_v072() {}
void NUISystem::CComponent::_v073() {}
void NUISystem::CComponent::_v074() {}
void NUISystem::CComponent::_v075() {}
void NUISystem::CComponent::_v076() {}
void NUISystem::CComponent::_v077() {}
void NUISystem::CComponent::_v078() {}
void NUISystem::CComponent::_v079() {}
void NUISystem::CComponent::_v080() {}
void NUISystem::CComponent::_v081() {}
void NUISystem::CComponent::_v082() {}
void NUISystem::CComponent::_v083() {}
static int g_deleted; static int g_activated; static int g_created; static void* g_createdName; static int g_createdFlag = -1;
NUISystem::CComponent::~CComponent() { ++g_deleted; }
void NUISystem::CComponent::Activate() { ++g_activated; }
static Str g_curName;
static int g_d0;
CCharString NUISystem::CComponent::GetDefName() const { CCharString r; r.PData = &g_curName; return r; }
static NUISystem::CComponent* g_comps[4]; static int g_ncomp;
NUISystem::CComponent* NUISystem::CManager::CreateComponent(const CCharString& n, bool f)
{ ++g_created; g_createdName = n.PData; g_createdFlag = f ? 1 : 0; return g_comps[g_ncomp++] = new NUISystem::CComponent; }
static NUISystem::CManager g_mgr;
NUISystem::CManager* NUISystem::CManager::GetInstance() { return &g_mgr; }
typedef NUISystem::CComponent* PC;
static PC g_map[16];
template <class K, class V> V& std::map<K, V>::operator[](const K& k) { return g_map[(int)k]; }
static NUISystem::CFrontEndManager g_fem;
int main()
{
    Str nm = { "MAIN" }; CCharString name; name.PData = &nm;
    g_map[0] = 0;
    g_fem.SetMainMenuDefName(name);   // absent: create + activate
    bool ok = g_created == 1 && g_map[0] == g_comps[0] && g_createdName == &nm && g_createdFlag == 0 && g_activated == 1 && g_deleted == 0;
    g_curName.s = "MAIN";
    g_fem.SetMainMenuDefName(name);   // same name: kept
    ok = ok && g_created == 1 && g_activated == 1 && g_deleted == 0 && g_map[0] == g_comps[0] && g_dtors >= 1;
    g_curName.s = "OTHER";
    g_fem.SetMainMenuDefName(name);   // different name: delete + recreate
    ok = ok && g_deleted == 1 && g_created == 2 && g_map[0] == g_comps[1] && g_activated == 2 && g_dtors >= 2;
    printf(ok ? "00595a06_TEST PASS\n" : "00595a06_TEST FAIL\n");
    name.PData = 0;
    return ok ? 0 : 1;
}