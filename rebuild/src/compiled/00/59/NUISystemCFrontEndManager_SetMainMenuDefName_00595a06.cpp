// NUISystem::CFrontEndManager::SetMainMenuDefName(const CCharString&) -- replace the main-menu screen component when its definition name changes, creating and activating it if absent
// Leaf: ?SetMainMenuDefName@CFrontEndManager@NUISystem@@IAEXABVCCharString@@@Z (Ego_r.pdb 0x00601C5E, protected; modelled public)
// Retail: Fable.exe @ 0x00595A06, 211 bytes, `ret 4`.
// Body: if m_mapScreens[0] (map::operator[] 0x0059B5D7): if m_mapScreens[0]->GetDefName() (slot 84, CCharString by value)
// != name (CCharString::operator!= 0x0099E900; temporary destroyed by ~CCharString 0x0099EAE0): delete m_mapScreens[0]
// (virtual scalar-deleting dtor slot 0, flag 1) and set it to 0. Then if m_mapScreens[0] is null:
// m_mapScreens[0] = CManager::GetInstance() (0x0041E5F2)->CreateComponent(name, false) (0x0041DB1D);
// m_mapScreens[0]->Activate() (slot 43, +0xac).
// Layout: m_mapScreens +0x54 (as the matched 0x00597B20 / 0x0059686D). Slot names 43/84 not recovered from the PDB;
// slot 84 is a 0-arg method returning CCharString by value (hidden return pointer), slot 43 a 0-arg void.
// Opt: size TU (unaligned entry) with a frame pointer.
#pragma optimize("s", on)
#pragma optimize("y", off)
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