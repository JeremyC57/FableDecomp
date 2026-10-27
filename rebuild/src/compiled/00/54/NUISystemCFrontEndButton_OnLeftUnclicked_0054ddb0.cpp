// NUISystem::CFrontEndButton::OnLeftUnclicked() -- when the button's def asks for it, run the PActionsOnLeftUnclicked action list
// Leaf: ?OnLeftUnclicked@CFrontEndButton@NUISystem@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054DDB0, 81 bytes, thiscall, plain `ret`.
// Identity (proven): slot 147 of vtable 0x01249554 (RTTI .?AVCFrontEndButton@NUISystem@@; CNavButton's vtable has its
//   own handlers there), Ego_r.pdb ?OnLeftUnclicked@CFrontEndButton@NUISystem@@MAEXXZ; the list read is +0x17C = Ego_r.pdb
//   CClickable::PActionsOnLeftUnclicked (Ego_r offset - 0x30: the 12-byte STLport containers before it); slots 146 / 147 / 148 read
//   PActionsOnLeftClicked / PActionsOnLeftUnclicked / PActionsOnRightClicked in that order.
// Callees: CComponent::GetDef() (vtable slot 108, +0x1B0; 0x00530EC0), CComponent::ExecuteActions (slot 131, +0x20C;
//   0x005309B0), the def's Destroy (slot 1) when the temporary's count drops to zero.
// Shape (family M129, representative 0x0054DD50): `if (GetDef()->Flag) ExecuteActions(PActionsOnLeftUnclicked);` -- the CDefPointer<CComponentDef>
//   temporary lives in the `push ecx` word, the flag byte (def +0x221) is held in bl across its release (`--count; if (0)
//   Destroy()`).
// Header debt: CComponent / CClickable / CFrontEndButton modelled locally (virtual slots as placeholders; the def's flag at
//   +0x221 is unnamed: no CComponentDef layout in the PDB layouts).
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    void Release() { if (--m_refCount == 0) Destroy(); }
    long m_refCount;                                        // +0x4
};
struct CComponentDef : public CDefPointeeBase {
    unsigned char _pad008[0x221 - 8];
    bool _unnamed_0x221;                                    // +0x221
};
template <class T> class CDefPointer {
public:
    CDefPointer(const CDefPointer& other);
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
    T* Object;
};
template <class T> class CCountedPointer { public: T* Data; void* Info; };
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list { public: void* _M_node; };
}
namespace NUISystem {
class CAction;
typedef std::list<CCountedPointer<CAction> > CActionList;
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer<CComponentDef> GetDef();            // slot 108 (+0x1B0); 0x00530EC0
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void ExecuteActions(CActionList* actions);       // slot 131 (+0x20C); 0x005309B0
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145();
    virtual void OnLeftClicked();                           // slot 146
    virtual void OnLeftUnclicked();                         // slot 147
    virtual void OnRightClicked();                          // slot 148
    unsigned char _pad004[0x174 - 4];
    CActionList* PActionsOnLeftClicked;                     // +0x174 (CClickable)
    CActionList* PActionsOnLeftHeld;                        // +0x178
    CActionList* PActionsOnLeftUnclicked;                   // +0x17C
    long StateBeforeRightClicked;                           // +0x180
    bool WasRightClicked;                                   // +0x184
    CActionList* PActionsOnRightClicked;                    // +0x188
};
class CFrontEndButton : public CComponent {
protected:
    virtual void OnLeftUnclicked();
};
void CFrontEndButton::OnLeftUnclicked()
{
    if (GetDef()->_unnamed_0x221)
        ExecuteActions(PActionsOnLeftUnclicked);
}
}