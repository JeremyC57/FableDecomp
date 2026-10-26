// NUISystem::CList::ScrollUpProcess() -- scroll the list one child up: run the scroll-up action if one is set, else (unless the list is busy) move the selection, refresh the child states and play the move or end-of-list sound
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053753C, 570 bytes; CList vtable 0x012462E4 slot 182; frame-pointer size TU.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRefCounted {
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CUISound;
struct CUIIconsDef : CDefBase {
    unsigned char _pad_0x08[0x18C];
    CUISound*     ListMoveSound;                     // +0x194
    unsigned char _pad_0x198[0x0C];
    CUISound*     ListEndSound;                      // +0x1A4
};
struct CIconsDefPointer {
    CUIIconsDef* Object;
    CIconsDefPointer() {}
    __forceinline ~CIconsDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CUIIconsDef* operator->() const { return Object; }
};

namespace NUISystem {
class CAction;
struct CCPPointerInfo;
struct CActionPointer {                              // CCountedPointer<NUISystem::CAction>
    CAction*        Data;
    CCPPointerInfo* Info;
};
class CManager { public:
    virtual void m000(); virtual void m001(); virtual void m002(); virtual void m003(); virtual void m004();
    virtual void m005(); virtual void m006(); virtual void m007(); virtual void m008(); virtual void m009();
    virtual void m010(); virtual void m011(); virtual void m012(); virtual void m013();
    virtual void PerformAction(const CActionPointer& action);              // slot 14 (+0x38)
    virtual void m015(); virtual void m016(); virtual void m017(); virtual void m018(); virtual void m019();
    virtual void m020(); virtual void m021(); virtual void m022(); virtual void m023(); virtual void m024();
    virtual void m025(); virtual void m026(); virtual void m027(); virtual void m028(); virtual void m029();
    virtual void m030(); virtual void m031(); virtual void m032(); virtual void m033(); virtual void m034();
    virtual void m035(); virtual void m036(); virtual void m037(); virtual void m038(); virtual void m039();
    virtual void PlayUISound(CUISound* const& sound, bool flag);            // slot 40 (+0xA0)
    virtual void m41();
    virtual CIconsDefPointer GetIconsDef();                                // slot 42 (+0xA8)
    static CManager* __fastcall GetInstance();                             // 0x0041E5F2
};
struct CStateHolder {                                // the component's second base (+4)
    virtual void s00(); virtual void s01(); virtual void s02();
    virtual void AddState(long state);               // slot 3 (+0x0C)
    virtual void RemoveState(long state);            // slot 4 (+0x10)
};
struct CComponentBase {
    virtual void c000(); virtual void c001(); virtual void c002(); virtual void c003(); virtual void c004();
    virtual void c005(); virtual void c006(); virtual void c007(); virtual void c008(); virtual void c009();
    virtual void c010(); virtual void c011(); virtual void c012(); virtual void c013(); virtual void c014();
    virtual void c015(); virtual void c016(); virtual void c017(); virtual void c018(); virtual void c019();
    virtual void c020(); virtual void c021(); virtual void c022(); virtual void c023(); virtual void c024();
    virtual void c025(); virtual void c026(); virtual void c027(); virtual void c028(); virtual void c029();
    virtual void c030(); virtual void c031(); virtual void c032(); virtual void c033(); virtual void c034();
    virtual void c035(); virtual void c036(); virtual void c037(); virtual void c038(); virtual void c039();
    virtual void c040(); virtual void c041(); virtual void c042(); virtual void c043();
    virtual bool IsInState(long state);              // slot 44 (+0xB0)
    virtual bool IsEnteringState(long state);        // slot 45 (+0xB4)
    virtual void c046(); virtual void c047();
    virtual void ChangeState(long state);            // slot 48 (+0xC0)
    virtual void c049(); virtual void c050(); virtual void c051(); virtual void c052(); virtual void c053();
    virtual void c054(); virtual void c055(); virtual void c056(); virtual void c057(); virtual void c058();
    virtual void c059(); virtual void c060(); virtual void c061(); virtual void c062(); virtual void c063();
    virtual void c064(); virtual void c065(); virtual void c066(); virtual void c067(); virtual void c068();
    virtual void c069(); virtual void c070(); virtual void c071(); virtual void c072(); virtual void c073();
    virtual void c074(); virtual void c075(); virtual void c076(); virtual void c077(); virtual void c078();
    virtual void c079(); virtual void c080(); virtual void c081(); virtual void c082(); virtual void c083();
    virtual void c084(); virtual void c085(); virtual void c086(); virtual void c087(); virtual void c088();
    virtual void c089(); virtual void c090(); virtual void c091(); virtual void c092(); virtual void c093();
    virtual void c094(); virtual void c095(); virtual void c096(); virtual void c097(); virtual void c098();
    virtual void c099(); virtual void c100(); virtual void c101(); virtual void c102(); virtual void c103();
    virtual void c104(); virtual void c105(); virtual void c106(); virtual void c107(); virtual void c108();
    virtual void c109(); virtual void c110(); virtual void c111(); virtual void c112(); virtual void c113();
    virtual void c114(); virtual void c115(); virtual void c116(); virtual void c117(); virtual void c118();
    virtual void c119(); virtual void c120(); virtual void c121(); virtual void c122(); virtual void c123();
    virtual void c124(); virtual void c125(); virtual void c126(); virtual void c127(); virtual void c128();
    virtual void c129(); virtual void c130(); virtual void c131(); virtual void c132(); virtual void c133();
    virtual void c134(); virtual void c135(); virtual void c136(); virtual void c137(); virtual void c138();
    virtual void c139(); virtual void c140(); virtual void c141(); virtual void c142(); virtual void c143();
    virtual void c144(); virtual void c145(); virtual void c146(); virtual void c147(); virtual void c148();
    virtual void c149(); virtual void c150(); virtual void c151(); virtual void c152(); virtual void c153();
    virtual void c154(); virtual void c155(); virtual void c156(); virtual void c157(); virtual void c158();
    virtual void c159(); virtual void c160(); virtual void c161(); virtual void c162(); virtual void c163();
    virtual void c164(); virtual void c165();
    virtual void ScrollSelectionDown(long a, bool b); // slot 166 (+0x298, 0x0053605F)
    virtual void c167();
    virtual void ScrollSelectionUp(long a, bool b);   // slot 168 (+0x2A0, 0x00536088)
    virtual bool AreThereAnyDeletions();              // slot 169 (+0x2A4, 0x005362F2)
    virtual void c170(); virtual void c171();
    virtual bool IsReversed();                        // slot 172 (+0x2B0, 0x0053CE50)
};
class CComponent : public CComponentBase, public CStateHolder { public:
    unsigned char _pad_0x08[0x28];
    float         Time;                              // +0x30
};
template <class T> struct Vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
class CList : public CComponent { public:
    unsigned char       _pad_0x34[0x128];
    long                SelectedChild;               // +0x15C
    float               LastInputTime;               // +0x160
    Vector<CComponent*> ScrollingChildren;           // +0x164
    Vector<CComponent*> NonScrollingChildren;        // +0x170
    unsigned char       _pad_0x17c[0x38];
    CActionPointer      CPActionOnScrollUp;          // +0x1B4
    CActionPointer      CPActionOnScrollDown;        // +0x1BC
    void ScrollUpProcess();
};

void CList::ScrollUpProcess()
{
    if (ScrollingChildren.size() > 0) {
        if (CPActionOnScrollUp.Data) {
            CManager::GetInstance()->PerformAction(CPActionOnScrollUp);
            return;
        }
        if (AreThereAnyDeletions())
            return;
        long previous = SelectedChild;
        ScrollingChildren[previous]->RemoveState(2);
        ScrollingChildren[SelectedChild]->RemoveState(3);
        ScrollingChildren[SelectedChild]->RemoveState(4);
        ScrollingChildren[SelectedChild]->RemoveState(5);
        if (IsReversed())
            ScrollSelectionDown(0, true);
        else
            ScrollSelectionUp(0, true);
        ScrollingChildren[SelectedChild]->AddState(2);
        ScrollingChildren[SelectedChild]->AddState(3);
        ScrollingChildren[SelectedChild]->AddState(4);
        ScrollingChildren[SelectedChild]->AddState(5);
        for (unsigned int i = 0; i < NonScrollingChildren.size(); ++i) {
            if (NonScrollingChildren[i]->IsInState(SelectedChild + 8) || NonScrollingChildren[i]->IsEnteringState(SelectedChild + 8))
                NonScrollingChildren[i]->ChangeState(SelectedChild + 8);
        }
        if (SelectedChild != previous) {
            CManager* owner = CManager::GetInstance();
            CManager::GetInstance()->PlayUISound(owner->GetIconsDef()->ListMoveSound, true);
        } else {
            CManager* owner = CManager::GetInstance();
            CManager::GetInstance()->PlayUISound(owner->GetIconsDef()->ListEndSound, true);
        }
    }
    LastInputTime = Time;
}
}