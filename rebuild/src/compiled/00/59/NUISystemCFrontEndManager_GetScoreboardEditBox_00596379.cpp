// NUISystem::CFrontEndManager::GetScoreboardEditBox(const CCharString& name) -- text of the named edit box on screen 25, or an empty string
// Leaf: ?GetScoreboardEditBox@CFrontEndManager@NUISystem@@IAE?AVCWideString@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x00596379, 98 bytes, `ret 8` (hidden result + name); Ego_r.pdb same symbol (protected).
// Body: screen = m_mapScreens[25] (map::operator[] 0x0059B5D7 on +0x54, the key a stack temporary); comp =
// screen->FindChild(name) (CComponent virtual slot 3, +0xc, CCharString by value: copy constructor 0x0099EC30 into the
// argument slot); if comp && comp->GetType() (slot 65, +0x104) == 0x25 return ((CEditBox*)comp)->GetText() (slot 142,
// +0x238, Ego_r.pdb ?GetText@CEditBox@NUISystem@@UAE?AVCWideString@@XZ); else CWideString() (0x0099AED0).
// Layout: retail CFrontEndManager m_mapScreens +0x54, as CanAcceptOptionChanges 0x005966F2. Slot 3 is one of the
// Ego_r.pdb FindChild / FindChildForward / FindChildNonRecursive (CCharString by value) virtuals -- which one is not
// confirmed; the FRONTEND_SCREEN / EType enumerator names for 25 and 0x25 are not recovered.
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
enum FRONTEND_SCREEN { FRONTEND_SCREEN_25 = 25 };
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& s);                   // 0x0099EC30
    ~CCharString();
};
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();                                       // 0x0099AED0
    CWideString(const CWideString& s);
    ~CWideString();
};
namespace NUISystem {
class CComponent {
public:
    enum EType { ETYPE_EDIT_BOX = 0x25 };                // value as compared (name not recovered)
    virtual void _v000(); virtual void _v001(); virtual void _v002();
    virtual CComponent* FindChild(CCharString name);     // slot 3 (+0x0c)
    virtual void _v004(); virtual void _v005(); virtual void _v006(); virtual void _v007(); virtual void _v008(); virtual void _v009();
    virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020(); virtual void _v021();
    virtual void _v022(); virtual void _v023(); virtual void _v024(); virtual void _v025(); virtual void _v026(); virtual void _v027();
    virtual void _v028(); virtual void _v029(); virtual void _v030(); virtual void _v031(); virtual void _v032(); virtual void _v033();
    virtual void _v034(); virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039();
    virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045();
    virtual void _v046(); virtual void _v047(); virtual void _v048(); virtual void _v049(); virtual void _v050(); virtual void _v051();
    virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055(); virtual void _v056(); virtual void _v057();
    virtual void _v058(); virtual void _v059(); virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063();
    virtual void _v064();
    virtual EType GetType() const;                       // slot 65 (+0x104)
};
class CEditBox : public CComponent {
public:
    virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069(); virtual void _v070(); virtual void _v071();
    virtual void _v072(); virtual void _v073(); virtual void _v074(); virtual void _v075(); virtual void _v076(); virtual void _v077();
    virtual void _v078(); virtual void _v079(); virtual void _v080(); virtual void _v081(); virtual void _v082(); virtual void _v083();
    virtual void _v084(); virtual void _v085(); virtual void _v086(); virtual void _v087(); virtual void _v088(); virtual void _v089();
    virtual void _v090(); virtual void _v091(); virtual void _v092(); virtual void _v093(); virtual void _v094(); virtual void _v095();
    virtual void _v096(); virtual void _v097(); virtual void _v098(); virtual void _v099(); virtual void _v100(); virtual void _v101();
    virtual void _v102(); virtual void _v103(); virtual void _v104(); virtual void _v105(); virtual void _v106(); virtual void _v107();
    virtual void _v108(); virtual void _v109(); virtual void _v110(); virtual void _v111(); virtual void _v112(); virtual void _v113();
    virtual void _v114(); virtual void _v115(); virtual void _v116(); virtual void _v117(); virtual void _v118(); virtual void _v119();
    virtual void _v120(); virtual void _v121(); virtual void _v122(); virtual void _v123(); virtual void _v124(); virtual void _v125();
    virtual void _v126(); virtual void _v127(); virtual void _v128(); virtual void _v129(); virtual void _v130(); virtual void _v131();
    virtual void _v132(); virtual void _v133(); virtual void _v134(); virtual void _v135(); virtual void _v136(); virtual void _v137();
    virtual void _v138(); virtual void _v139(); virtual void _v140(); virtual void _v141();
    virtual CWideString GetText();                       // slot 142 (+0x238)
};
}
namespace std {
template <class K, class V> class map {
public:
    unsigned char _body[0x10];
    V& operator[](const K& key);                         // 0x0059B5D7 for FRONTEND_SCREEN -> CComponent*
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                                   _pad00[0x54];
    std::map<FRONTEND_SCREEN, CComponent*>          m_mapScreens;       // +0x54
protected:
    CWideString GetScoreboardEditBox(const CCharString& name);
    friend struct CFrontEndManager_Test;                 // behaviour-test hook
};
CWideString CFrontEndManager::GetScoreboardEditBox(const CCharString& name)
{
    CComponent* comp = m_mapScreens[FRONTEND_SCREEN_25]->FindChild(name);
    if (comp && comp->GetType() == CComponent::ETYPE_EDIT_BOX)
        return static_cast<CEditBox*>(comp)->GetText();
    return CWideString();
}
}