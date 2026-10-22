// NUISystem::CMenuEntry::GoForward() -- (unless disabled, state 2) hand the UI and thing action parameters to the front-end
//   manager; when available, run the pre-actions, put every child into state 5 (noting a child menu, type 8), play the
//   front-end "forward" sound unless a child menu, pre-actions or actions exist, then run the actions; otherwise just play
//   the sound. Record the input time, and restart the global menu timer unless the parent menu's def suppresses it.
// Leaf: ?GoForward@CMenuEntry@NUISystem
// Retail: Fable.exe @ 0x0053D720, 592 bytes, plain `ret`.
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall) and its slots 24 (+0x60), 27 (+0x6c), 40 (+0xa0 play a
//   sound by def member) and 42 (+0xa8 GetDef by value); own slot 131 (+0x20c, run an action list); child component slots
//   48 (+0xc0), 65 (+0x104 GetType) and 108 (+0x1b0 GetDef by value); the time source 0x009E1BC0 (returns double) stored
//   to the double 0x013B8AA0.
// Layout: the landed ctor 0x0053DFE0 / dtor 0x0053E4E0 model (retail = PDB - 0x30): Parent +0xc8 (PDB +0xf8), the child
//   vector +0xb0, State +0x148, LastInputTime +0x194 from +0x30, IsAvailable +0x198, the parameter and action lists
//   +0x19c..+0x1b0; the front-end def's forward sound at +0x1a4; the parent menu def's flag byte at +0x20a.
// Shape: the defs are released through a CRefCounted base at +4 (as the landed InitializeRumbleActions 0x0053DA30); the
//   parent menu def is a conditional temporary of the `||` condition (retail's flag word at the frame bottom).
void operator delete(void* p);                   // 0x00BFE9BC

template <class T> struct _List_node {
    _List_node* _M_next;                         // +0
    _List_node* _M_prev;                         // +4
    T           _M_data;                         // +8
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) {}
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> struct CPodList {             // STLport list<T>
    _List_node<T>* _M_node;
    _List_iterator<T> begin() const { return _List_iterator<T>(_M_node->_M_next); }
    _List_iterator<T> end() const { return _List_iterator<T>(_M_node); }
};
template <class T> struct CRetailList { void* _M_node; };

struct CRefCounted {                             // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    CRefCounted Ref;                             // +0x04
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
struct CSoundName { long Value; };
struct CFrontEndManagerDef : CDefBase {
    unsigned char _pad08[0x1a4 - 8];
    CSoundName    ForwardSound;                  // +0x1a4
};
struct CMenuDef : CDefBase {
    unsigned char _pad08[0x20a - 8];
    bool          KeepTimer;                     // +0x20a (descriptive)
};
template <class T> struct CDefPointer {
    T* Data;
    ~CDefPointer()
    {
        if (Data)
            Data->ReleaseTemporary();
    }
    T* operator->() const { return Data; }
};

namespace NUISystem {
struct CAction;
struct CActionParam_;
}

struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void UseThingActionParam(long param);   // slot 24 (+0x60)
    virtual void v025(); virtual void v026();
    virtual void UseUIActionParam(NUISystem::CActionParam_* param);   // slot 27 (+0x6c)
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void PlaySound(const CSoundName& sound, bool a);   // slot 40 (+0xa0)
    virtual void v041();
    virtual CDefPointer<CFrontEndManagerDef> GetDef();   // slot 42 (+0xa8)
};

namespace NUISystem {

struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void SetState(long state);   // slot 48 (+0xc0)
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual long GetType();   // slot 65 (+0x104)
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer<CMenuDef> GetDef();   // slot 108 (+0x1b0)
};
template <class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    T* operator->() const { return m_pObject; }
};
struct CComponentVector {                        // STLport vector<CCountedPointer<CComponent> > (12 bytes)
    CCountedPointer<CComponent>* _M_start;
    CCountedPointer<CComponent>* _M_finish;
    CCountedPointer<CComponent>* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCountedPointer<CComponent>& operator[](unsigned long i) { return _M_start[i]; }
};

struct CMenuEntry {
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
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void ExecuteActions(CRetailList<CAction>* actions);   // slot 131 (+0x20c)
    unsigned char             _pad04[0x30 - 4];
    long                      InputTime;              // +0x30
    unsigned char             _pad34[0xb0 - 0x34];
    CComponentVector          Children;               // +0xb0
    unsigned char             _padBC[0xc8 - 0xbc];
    CComponent*               Parent;                 // +0xc8
    unsigned char             _padCC[0x148 - 0xcc];
    long                      State;                  // +0x148
    unsigned char             _pad14C[0x194 - 0x14c];
    long                      LastInputTime;          // +0x194
    bool                      IsAvailable;            // +0x198
    CPodList<CActionParam_*>* PUIActionParams;        // +0x19c
    CPodList<long>*           PThingActionParams;     // +0x1a0
    CRetailList<CAction>*     PActions;               // +0x1a4
    CRetailList<CAction>*     PActionsOnSelected;     // +0x1a8
    CRetailList<CAction>*     PActionsOnBack;         // +0x1ac
    CRetailList<CAction>*     PPreActions;            // +0x1b0
    __forceinline void PlayForwardSound()
    {
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetDef()->ForwardSound, true);
    }
    void GoForward();
};

}

double GetTimeNow();                             // 0x009E1BC0
extern double g_MenuTimer;                       // 0x013B8AA0

namespace NUISystem {

void CMenuEntry::GoForward()
{
    if (State == 2)
        return;
    if (PUIActionParams) {
        for (_List_iterator<CActionParam_*> it = PUIActionParams->begin(); it != PUIActionParams->end(); ++it)
            CFrontEndManager::GetInstance()->UseUIActionParam(*it);
    }
    if (PThingActionParams) {
        for (_List_iterator<long> it = PThingActionParams->begin(); it != PThingActionParams->end(); ++it)
            CFrontEndManager::GetInstance()->UseThingActionParam(*it);
    }
    if (IsAvailable) {
        ExecuteActions(PPreActions);
        bool hasMenu = false;
        for (unsigned long i = 0; i < Children.size(); ++i) {
            if (Children[i]->GetType() == 8)
                hasMenu = true;
            Children[i]->SetState(5);
        }
        if (!hasMenu && !PPreActions && !PActions)
            PlayForwardSound();
        ExecuteActions(PActions);
    } else {
        PlayForwardSound();
    }
    LastInputTime = InputTime;
    CComponent* menu = Parent && Parent->GetType() == 8 ? Parent : 0;
    if (menu == 0 || menu->GetDef()->KeepTimer)
        g_MenuTimer = GetTimeNow();
}

}
