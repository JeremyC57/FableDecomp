// NUISystem::CTextSlider::RevertValue() -- restore the original selection, push it into every action's target and re-run the actions
// Retail: Fable.exe (TLC, VC7.1) @ 0x005491A0, 135 bytes (+9 int3). __fastcall, `ret`.
// Callees: CFrontEndManager::GetInstance() 0x0041E5F2 (static); the front-end manager's virtual slot 14 (+0x38), called with
//          each action (descriptive name ExecuteAction).
// Shape: `SelectedChild = PreviouslySelectedChild = StartSelectedChild = OriginalSelectedChild;` (stores +0x164, +0x160,
//        +0x15C); `for (unsigned i = 0; i < Actions.size(); ++i)` with operator[] as `*(begin() + i)` (the scaled index
//        kept in ebx for the call argument); each action's target selection (action +4 -> [0] -> +8 -> +8, descriptive
//        names) takes OriginalSelectedChild; Move is set after the loop.
// Layout: retail NUISystem::CTextSlider = Ego_r.pdb minus 0x30 up to Actions (+0x16C, 12-byte STLport vector) and minus 0x34
//         after it; OriginalSelectedChild +0x194.
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;                                     // +0
    CCPPointerInfo* Info;                        // +4
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes; PDB 16)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
template <class T> struct CRetailList;
struct CComponent;
namespace NUISystem {
class CAction;
struct CComponentInterface {
    virtual void v000();
};
struct CDrawableInterface {
    virtual void v000();
    unsigned char _pad04[0x10];
};
struct CDraggableInterface {
    virtual void v000();
};
class CDraggableInto : public CComponentInterface, public CDrawableInterface, public CDraggableInterface { public:
    unsigned char _pad1c[0x15c - 0x1c];
    CDraggableInto(long id);                     // 0x0052CC50
};
class CTextSlider : public CDraggableInto { public:            // vtables 0x01248A8C / 0x01248A68 / 0x01248A60 (.?AVCTextSlider@NUISystem@@)
    unsigned long SelectedChild;                 // +0x15c
    unsigned long PreviouslySelectedChild;       // +0x160
    unsigned long StartSelectedChild;            // +0x164
    bool Move;                                   // +0x168
    CRetailVector<CCountedPointer<CAction> > Actions;          // +0x16c
    CRetailList<CCountedPointer<CAction> >* PActionsOnBack;    // +0x178
    CRetailList<CCountedPointer<CAction> >* PActionsOnSelect;  // +0x17c
    float LastInputTime;                         // +0x180
    float InputDelay;                            // +0x184
    long PDefAction;                             // +0x188 EActionType
    CComponent* PLeftArrow;                      // +0x18c
    CComponent* PRightArrow;                     // +0x190
    unsigned long OriginalSelectedChild;         // +0x194
    CComponent* PAcceptButton;                   // +0x198
    unsigned long MaxChild;                      // +0x19c
    CTextSlider(long id);
    virtual void v000();
    void Initialize();                           // 0x00549B20
    void RevertValue();
};
}
struct CSliderValue { unsigned char _pad[8]; unsigned long Selected; };          // +0x08
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };           // +0x08
struct CSliderTargetRef { CSliderTarget* Target; };                             // +0x00
namespace NUISystem {
class CAction { public:
    virtual void v00();
    CSliderTargetRef* PTarget;                   // +0x04
};
}
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& action);   // slot 14 (+0x38)
};
namespace NUISystem {
void CTextSlider::RevertValue()
{
    SelectedChild = PreviouslySelectedChild = StartSelectedChild = OriginalSelectedChild;
    for (unsigned int i = 0; i < Actions.size(); ++i) {
        Actions[i]->PTarget->Target->Value->Selected = OriginalSelectedChild;
        CFrontEndManager::GetInstance()->ExecuteAction(Actions[i]);
    }
    Move = true;
}
}