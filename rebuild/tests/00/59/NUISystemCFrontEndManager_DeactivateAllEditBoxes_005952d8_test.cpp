#include <cstdio>
class CCharString { public: void* PStringData; };
class CWideString { public: void* PStringData; };
template <class T> class CCountedPointer {
public:
    T*    Object;
    long* Count;
    operator T*() const { return Object; }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
template <class T> class deque {
public:
    unsigned char _body[0x28];
    T& back();                                           // 0x0059B039 for CComponent*
};
template <class T, class C = deque<T> > class stack {
public:
    C c;
    T& top() { return c.back(); }
};
}
enum FRONTEND_SCREEN { FRONTEND_SCREEN_25 = 25 };
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
    virtual void _v052();
    virtual std::vector<CCountedPointer<CComponent> >& GetChildren();   // slot 53 (+0xd4)
    virtual void _v054(); virtual void _v055(); virtual void _v056(); virtual void _v057();
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
    virtual void SetText(const CWideString& text);       // slot 143 (+0x23c)
    virtual void _v144(); virtual void _v145(); virtual void _v146();
    virtual void UpdateDisplayText();                    // slot 147 (+0x24c, name not confirmed)
    virtual void _v148(); virtual void _v149();
    virtual void SetActive(bool active);                 // slot 150 (+0x258, name not confirmed)
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                  _pad00[0x20];
    std::stack<CComponent*>        m_stackScreens;       // +0x20 (Ego_r.pdb +0x2c)
    void DeactivateAllEditBoxes(CComponent* component);
};
void CFrontEndManager::DeactivateAllEditBoxes(CComponent* component)
{
    if (!component)
    {
        component = m_stackScreens.top();
        if (!component)
            return;
    }
    if (component->GetType() == CComponent::ETYPE_EDIT_BOX)
        static_cast<CEditBox*>(component)->SetActive(false);
    for (unsigned long i = 0; i < component->GetChildren().size(); ++i)
        DeactivateAllEditBoxes(component->GetChildren()[i]);
}
}

void NUISystem::CComponent::_v000() {}
void NUISystem::CComponent::_v001() {}
void NUISystem::CComponent::_v002() {}
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
void NUISystem::CComponent::_v043() {}
void NUISystem::CComponent::_v044() {}
void NUISystem::CComponent::_v045() {}
void NUISystem::CComponent::_v046() {}
void NUISystem::CComponent::_v047() {}
void NUISystem::CComponent::_v048() {}
void NUISystem::CComponent::_v049() {}
void NUISystem::CComponent::_v050() {}
void NUISystem::CComponent::_v051() {}
void NUISystem::CComponent::_v052() {}
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
void NUISystem::CEditBox::_v066() {}
void NUISystem::CEditBox::_v067() {}
void NUISystem::CEditBox::_v068() {}
void NUISystem::CEditBox::_v069() {}
void NUISystem::CEditBox::_v070() {}
void NUISystem::CEditBox::_v071() {}
void NUISystem::CEditBox::_v072() {}
void NUISystem::CEditBox::_v073() {}
void NUISystem::CEditBox::_v074() {}
void NUISystem::CEditBox::_v075() {}
void NUISystem::CEditBox::_v076() {}
void NUISystem::CEditBox::_v077() {}
void NUISystem::CEditBox::_v078() {}
void NUISystem::CEditBox::_v079() {}
void NUISystem::CEditBox::_v080() {}
void NUISystem::CEditBox::_v081() {}
void NUISystem::CEditBox::_v082() {}
void NUISystem::CEditBox::_v083() {}
void NUISystem::CEditBox::_v084() {}
void NUISystem::CEditBox::_v085() {}
void NUISystem::CEditBox::_v086() {}
void NUISystem::CEditBox::_v087() {}
void NUISystem::CEditBox::_v088() {}
void NUISystem::CEditBox::_v089() {}
void NUISystem::CEditBox::_v090() {}
void NUISystem::CEditBox::_v091() {}
void NUISystem::CEditBox::_v092() {}
void NUISystem::CEditBox::_v093() {}
void NUISystem::CEditBox::_v094() {}
void NUISystem::CEditBox::_v095() {}
void NUISystem::CEditBox::_v096() {}
void NUISystem::CEditBox::_v097() {}
void NUISystem::CEditBox::_v098() {}
void NUISystem::CEditBox::_v099() {}
void NUISystem::CEditBox::_v100() {}
void NUISystem::CEditBox::_v101() {}
void NUISystem::CEditBox::_v102() {}
void NUISystem::CEditBox::_v103() {}
void NUISystem::CEditBox::_v104() {}
void NUISystem::CEditBox::_v105() {}
void NUISystem::CEditBox::_v106() {}
void NUISystem::CEditBox::_v107() {}
void NUISystem::CEditBox::_v108() {}
void NUISystem::CEditBox::_v109() {}
void NUISystem::CEditBox::_v110() {}
void NUISystem::CEditBox::_v111() {}
void NUISystem::CEditBox::_v112() {}
void NUISystem::CEditBox::_v113() {}
void NUISystem::CEditBox::_v114() {}
void NUISystem::CEditBox::_v115() {}
void NUISystem::CEditBox::_v116() {}
void NUISystem::CEditBox::_v117() {}
void NUISystem::CEditBox::_v118() {}
void NUISystem::CEditBox::_v119() {}
void NUISystem::CEditBox::_v120() {}
void NUISystem::CEditBox::_v121() {}
void NUISystem::CEditBox::_v122() {}
void NUISystem::CEditBox::_v123() {}
void NUISystem::CEditBox::_v124() {}
void NUISystem::CEditBox::_v125() {}
void NUISystem::CEditBox::_v126() {}
void NUISystem::CEditBox::_v127() {}
void NUISystem::CEditBox::_v128() {}
void NUISystem::CEditBox::_v129() {}
void NUISystem::CEditBox::_v130() {}
void NUISystem::CEditBox::_v131() {}
void NUISystem::CEditBox::_v132() {}
void NUISystem::CEditBox::_v133() {}
void NUISystem::CEditBox::_v134() {}
void NUISystem::CEditBox::_v135() {}
void NUISystem::CEditBox::_v136() {}
void NUISystem::CEditBox::_v137() {}
void NUISystem::CEditBox::_v138() {}
void NUISystem::CEditBox::_v139() {}
void NUISystem::CEditBox::_v140() {}
void NUISystem::CEditBox::_v141() {}
void NUISystem::CEditBox::_v144() {}
void NUISystem::CEditBox::_v145() {}
void NUISystem::CEditBox::_v146() {}
void NUISystem::CEditBox::_v148() {}
void NUISystem::CEditBox::_v149() {}
static int g_active[8]; static int g_activeArg = 1;
NUISystem::CComponent* NUISystem::CComponent::FindChild(CCharString) { return 0; }
struct CTestComp;
CWideString NUISystem::CEditBox::GetText() { CWideString w; w.PStringData = 0; return w; }
void NUISystem::CEditBox::SetText(const CWideString&) {}
void NUISystem::CEditBox::UpdateDisplayText() {}
struct CNode { NUISystem::CComponent::EType Type; int Id; std::vector<CCountedPointer<NUISystem::CComponent> > Kids; };
static CNode g_nodes[8];
static int IdOf(const NUISystem::CComponent* c);
NUISystem::CComponent::EType NUISystem::CComponent::GetType() const { return g_nodes[IdOf(this)].Type; }
std::vector<CCountedPointer<NUISystem::CComponent> >& NUISystem::CComponent::GetChildren() { return g_nodes[IdOf(this)].Kids; }
void NUISystem::CEditBox::SetActive(bool a) { ++g_active[IdOf(this)]; g_activeArg = a; }
static NUISystem::CEditBox g_objs[8];
static int IdOf(const NUISystem::CComponent* c) { return int((const NUISystem::CEditBox*)c - g_objs); }
static NUISystem::CComponent* g_top;
template <class T> T& std::deque<T>::back() { return *(T*)&g_top; }
int main()
{
    static CCountedPointer<NUISystem::CComponent> kids0[2], kids1[1];
    for (int i = 0; i < 8; ++i) { g_nodes[i].Type = (NUISystem::CComponent::EType)1; g_nodes[i].Kids._M_start = g_nodes[i].Kids._M_finish = 0; }
    // tree: 0 -> {1, 2}; 1 -> {3}; edit boxes: 0? no; 2 and 3 yes
    kids0[0].Object = &g_objs[1]; kids0[1].Object = &g_objs[2]; kids1[0].Object = &g_objs[3];
    g_nodes[0].Kids._M_start = kids0; g_nodes[0].Kids._M_finish = kids0 + 2;
    g_nodes[1].Kids._M_start = kids1; g_nodes[1].Kids._M_finish = kids1 + 1;
    g_nodes[2].Type = NUISystem::CComponent::ETYPE_EDIT_BOX; g_nodes[3].Type = NUISystem::CComponent::ETYPE_EDIT_BOX;
    static unsigned char raw[sizeof(NUISystem::CFrontEndManager)]; NUISystem::CFrontEndManager& m = *(NUISystem::CFrontEndManager*)raw;
    g_top = 0; m.DeactivateAllEditBoxes(0);
    bool ok = g_active[2] == 0 && g_active[3] == 0;
    g_top = &g_objs[0]; m.DeactivateAllEditBoxes(0);
    ok = ok && g_active[0] == 0 && g_active[1] == 0 && g_active[2] == 1 && g_active[3] == 1 && g_activeArg == 0;
    m.DeactivateAllEditBoxes(&g_objs[1]);
    ok = ok && g_active[2] == 1 && g_active[3] == 2;
    std::printf(ok ? "005952d8_TEST PASS\n" : "005952d8_TEST FAIL\n");
    return ok ? 0 : 1;
}