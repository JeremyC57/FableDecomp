// NUISystem::CScrollBarOutside::GetDataFromDef() -- add the def's two configured actions (AddActionOnLeftClickedAbove / AddActionOnLeftClickedUnder)
// Leaf: ?GetDataFromDef@CScrollBarOutside@NUISystem@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00559230, 292 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?GetDataFromDef@CScrollBarOutside@NUISystem@@QAEXXZ; called twice by NUISystem::CScrollBarOutside's constructors 0x00559360; the two virtual calls are slots 150 / 151, CScrollBarOutside's own AddActionOnLeftClickedAbove / AddActionOnLeftClickedUnder (Ego_r.pdb, in declaration order).
// Callees: CComponent::GetDef() (slot 108, +0x1B0), operator new 0x00BFEA1A, NUISystem::CAction::CAction() 0x0042BE50,
//   CCountedPointer<NUISystem::CAction>::CCountedPointer(CAction*) 0x0042AA29, the two AddActionOn... virtuals (slots 150 / 151),
//   operator delete 0x00BFE9BC; the count block's and the def's deleters.
// Shape (family M131, representative 0x00559230): `CDefPointer<CComponentDef> def = GetDef(); if (def->A) { CCountedPointer<CAction>
//   action(new CAction); action->ActionType = def->A; AddActionOnLeftClickedAbove(action); }` and the same for B; each action released (`--refs;
//   if (refs == 0)`, re-read), then the def (`if (--m_refCount == 0) Destroy()`).
// Header debt: the control / CComponent are modelled locally (virtual slots as placeholders); the def's action-type words at
//   +0x100 / +0xFC are unnamed (no def layout in the PDB layouts).
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    void Release() { if (--m_refCount == 0) Destroy(); }
    long m_refCount;                                        // +0x4
};
struct CComponentDef : public CDefPointeeBase {
    unsigned char _pad008[0xFC - 8];
    long _unnamed_0xfc;                                  // +0xFC
    long _unnamed_0x100;                                  // +0x100
};
template <class T> class CDefPointer {
public:
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
    T* Object;
};
class CCPPointerInfo { public:
    unsigned long refs;                                     // +0x0
    void (__fastcall* destroy)(void*);                      // +0x4
    void* object;                                           // +0x8
};
template <class T> class CCountedPointer {
public:
    CCountedPointer(T* data);                               // <NUISystem::CAction>: 0x0042AA29
    ~CCountedPointer()
    {
        if (Info) {
            --Info->refs;
            if (Info->refs == 0) {
                Info->destroy(Info->object);
                operator delete(Info);
            }
        }
    }
    T* operator->() const { return Data; }
    T* Data;                                                // +0x0
    CCPPointerInfo* Info;                                   // +0x4
};
namespace NUISystem {
enum EActionType { ACTION_TYPE_NONE = 0 };
class CAction {                                             // 0x10 bytes in retail
public:
    CAction();                                              // 0x0042BE50
    EActionType ActionType;                                 // +0x0
    void* UIActionParams;                                   // +0x4 (STLport list)
    void* ThingActionParams;                                // +0x8 (STLport list)
    bool Valid;                                             // +0xC
};
class CScrollBarOutside {
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
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void AddActionOnLeftClickedAbove(CCountedPointer<CAction>& action);   // slot 150
    virtual void AddActionOnLeftClickedUnder(CCountedPointer<CAction>& action);   // slot 151
    void GetDataFromDef();
};
void CScrollBarOutside::GetDataFromDef()
{
    CDefPointer<CComponentDef> def = GetDef();
    if (def->_unnamed_0x100) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = (EActionType)def->_unnamed_0x100;
        AddActionOnLeftClickedAbove(action);
    }
    if (def->_unnamed_0xfc) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = (EActionType)def->_unnamed_0xfc;
        AddActionOnLeftClickedUnder(action);
    }
}
}