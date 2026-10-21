// NUISystem::CFrontEndManager::DeactivateAllEditBoxes(CComponent* component) -- recursively deactivate every edit box under a component (default: the top screen)
// Leaf: ?DeactivateAllEditBoxes@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@@Z
// Retail: Fable.exe @ 0x005952D8, 126 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: if !component: component = m_stackScreens.top() (deque::back 0x0059B039, out of line; labelled
// CTCTrophy::OnShowTrophyInitialise by folding), and return if that is null. If component->GetType() (slot 65) == 0x25 the
// edit box gets slot 150 (SetActive(false), as 0x0059630E). Then, for i < GetChildren().size() (slot 53, Ego_r.pdb
// ?GetChildren@CComponent@NUISystem@@UAEAAV?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@...@std@@XZ, re-read every
// pass), recurse on the child's raw pointer (8-byte CCountedPointer elements).
// Layout: retail CFrontEndManager m_stackScreens at +0x20 (Ego_r.pdb +0x2c). CComponent / CEditBox slots as the matched
// 0x0059630E / 0x00596379 rows.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
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