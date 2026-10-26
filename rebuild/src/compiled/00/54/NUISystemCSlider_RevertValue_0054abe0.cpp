// NUISystem::CSlider::RevertValue() -- restore the value the slider had before editing, push it into every action's target and re-run the actions
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054ABE0, 167 bytes (+ int3 padding), `ret`.
// Leaf: RevertValue
// Callees: CFrontEndManager::GetInstance() @ 0x0041E5F2 (static `__fastcall`); the front-end manager's virtual slot 14
//   (+0x38), called with each action (descriptive name ExecuteAction, as in the landed twin).
// Shape: the C2DVector twin of the landed NUISystem::CTextSlider::RevertValue 0x005491A0:
//   `OldValue = StartValue = Value = PreviousValue;` (right to left: stores +0x1A4, +0x1B5, +0x1AD, the two words rotating
//   through eax/ecx/edx); `for (unsigned i = 0; i < Actions.size(); ++i)` with operator[] as `*(begin() + i)` (the scaled
//   index kept in ebx for the call argument); each action's target value (action +4 -> [0] -> +8 -> +8, descriptive
//   names) takes PreviousValue.X; Move is set after the loop.
// Layout: retail NUISystem::CSlider = Ego_r.pdb minus 0x30 up to Actions (as the landed ctor 0x0054C050): Value +0x1A4,
//   Move +0x1AC, OldValue +0x1AD, StartValue +0x1B5 (unaligned in the PDB too, hence pack(1)), Actions +0x1C0 (12-byte
//   STLport vector), PreviousValue +0x1E0.
class C2DVector { public: float X, Y; };
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
struct IPrimary_0054c050 { virtual void v00(); };
struct CObserver {                               // the secondary base at +4 (0x14 bytes)
    virtual void a00();
    unsigned char _pad04[0x10];
};
struct ISecondaryB_0054c050 { virtual void b00(); };
struct CSprite : IPrimary_0054c050, CObserver, ISecondaryB_0054c050 {
    CSprite(void* def);                          // 0x0041B800
};
struct CComponent;
namespace NUISystem {
class CAction;
#pragma pack(push, 1)
class CSlider : public CSprite { public:                       // vtables 0x01248D04 / 0x01248CE0 / 0x01248CD8
    unsigned char _pad01c[0x1a4 - 0x1c];
    C2DVector     Value;                         // +0x1a4 (PDB +0x1d4)
    bool          Move;                          // +0x1ac
    C2DVector     OldValue;                      // +0x1ad
    C2DVector     StartValue;                    // +0x1b5
    unsigned char _pad1bd[3];
    CRetailVector<CCountedPointer<CAction> > Actions;          // +0x1c0 (PDB +0x1f0)
    CRetailList<CCountedPointer<CAction> >*  PActionsOnBack;   // +0x1cc
    CRetailList<CCountedPointer<CAction> >*  PActionsOnSelect; // +0x1d0
    long          PDefAction;                    // +0x1d4 EActionType
    CComponent*   PLeftArrow;                    // +0x1d8
    CComponent*   PRightArrow;                   // +0x1dc
    C2DVector     PreviousValue;                 // +0x1e0
    CComponent*   PAcceptButton;                 // +0x1e8
    virtual void a00();
    virtual void b00();
    void RevertValue();
};
#pragma pack(pop)
}
struct CSliderValue { unsigned char _pad[8]; float Value; };                  // +0x08
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };          // +0x08
struct CSliderTargetRef { CSliderTarget* Target; };                            // +0x00
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
void CSlider::RevertValue()
{
    OldValue = StartValue = Value = PreviousValue;
    for (unsigned int i = 0; i < Actions.size(); ++i) {
        Actions[i]->PTarget->Target->Value->Value = PreviousValue.X;
        CFrontEndManager::GetInstance()->ExecuteAction(Actions[i]);
    }
    Move = true;
}
}