// NUISystem::CSlider::ChangeState(unsigned long state) -- deactivate: unregister as a front-end observer; activate: register
//   and observe the four input events; state 5: refresh, remember the value and fade the accept button; state 0: on a cancel,
//   restore the start value and re-run the actions when it moved. Then the base state change, and a slider whose range is
//   empty is made unselectable.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054A990, 592 bytes, `ret 4`.
// Leaf: ChangeState
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall); manager AddObserver (+0x08), RemoveObserver (+0x14),
//   ExecuteAction (+0x38), GetLastInputResult (+0x84, descriptive); CObserver::ObserveEvent (+0x0C) and
//   IsObservingEvent (+0x08, descriptive); the slider's virtual slot 150 (+0x258, descriptive UpdateValue); the accept
//   button's GetColour (+0x38) / ChangeColour (+0x98) (descriptive); CChangingStateComponent::ChangeState 0x0052CF40.
//   Pool: 0.0001f 0x0129BA3C.
// Layout: as the landed NUISystem::CSlider RevertValue 0x0054ABE0 / Update 0x0054BA20 (Ego_r.pdb minus 0x30): Min +0x18C,
//   Max +0x194, Value +0x1A4, Move +0x1AC, OldValue +0x1AD, StartValue +0x1B5 (pack(1)), Actions +0x1C0, PreviousValue
//   +0x1E0, PAcceptButton +0x1E8; CComponent byte +0x87 (descriptive Selectable). Bases as the landed
//   CScrollingViewport::ChangeState 0x0053EBF0 (primary +0, CObserver +4, interface +0x18).
#include <math.h>
struct C2DVector { float X, Y; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& c) : B(c.B), G(c.G), R(c.R), A(c.A) {}
};
struct CCPPointerInfo { long Count; };
template <class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct IPrimary_0054a990 { virtual void v00(); };
struct CObserver {
    virtual void a00(); virtual void a01();
    virtual bool IsObservingEvent(long eventType);       // slot 2 (+0x08)
    virtual void ObserveEvent(long eventType);           // slot 3 (+0x0c)
    unsigned char _pad04[0x10];
};
struct ISecondaryB_0054a990 { virtual void b00(); };
struct CComponentPrimary_0054a990 {
    virtual void c00(); virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06(); virtual void c07(); virtual void c08(); virtual void c09(); virtual void c10(); virtual void c11();
    virtual void c12(); virtual void c13();
    virtual CRGBColour GetColour() const;                // slot 14 (+0x38)
    virtual void c15(); virtual void c16(); virtual void c17(); virtual void c18(); virtual void c19(); virtual void c20();
    virtual void c21(); virtual void c22(); virtual void c23(); virtual void c24(); virtual void c25(); virtual void c26();
    virtual void c27(); virtual void c28(); virtual void c29(); virtual void c30(); virtual void c31(); virtual void c32();
    virtual void c33(); virtual void c34(); virtual void c35(); virtual void c36(); virtual void c37();
    virtual void ChangeColour(const CRGBColour& colour, float time, bool linear);   // slot 38 (+0x98)
};
struct CComponent : CComponentPrimary_0054a990, CObserver {
};
struct CChangingStateComponent : IPrimary_0054a990, CObserver, ISecondaryB_0054a990 {
    unsigned char _pad1c[0x87 - 0x1c];
    bool Selectable;                                     // +0x87
    unsigned char _pad88[0x188 - 0x88];
    void ChangeState(unsigned long state);               // 0x0052CF40
};
namespace NUISystem { struct CAction; }
struct CFrontEndManager {
    virtual void v000(); virtual void v001();
    virtual void AddObserver(CObserver* observer);       // slot 2 (+0x08)
    virtual void v003(); virtual void v004();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013();
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& action);   // slot 14 (+0x38)
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032();
    virtual long GetLastInputResult();                   // slot 33 (+0x84)
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CSliderValue { unsigned char _pad[8]; float Value; };
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };
struct CSliderTargetRef { CSliderTarget* Target; };
namespace NUISystem {
struct CAction {
    virtual void v00();
    CSliderTargetRef* PTarget;                           // +0x04
};
static __forceinline bool Differs(const C2DVector& a, const C2DVector& b)
{
    return !(fabs(a.X - b.X) <= 0.0001f) || !(fabs(a.Y - b.Y) <= 0.0001f);
}
#pragma pack(push, 1)
struct CSlider : CChangingStateComponent {
    unsigned char _pad188[0x18c - 0x188];
    C2DVector     Min;                           // +0x18c
    C2DVector     Max;                           // +0x194
    C2DVector     Step;                          // +0x19c
    C2DVector     Value;                         // +0x1a4
    bool          Move;                          // +0x1ac
    C2DVector     OldValue;                      // +0x1ad
    C2DVector     StartValue;                    // +0x1b5
    unsigned char _pad1bd[3];
    CRetailVector<CCountedPointer<CAction> > Actions;   // +0x1c0
    unsigned char _pad1cc[0x1e0 - 0x1cc];
    C2DVector     PreviousValue;                 // +0x1e0
    CComponent*   PAcceptButton;                 // +0x1e8
    virtual void d00(); virtual void d01(); virtual void d02(); virtual void d03(); virtual void d04(); virtual void d05();
    virtual void d06(); virtual void d07(); virtual void d08(); virtual void d09(); virtual void d10(); virtual void d11();
    virtual void d12(); virtual void d13(); virtual void d14(); virtual void d15(); virtual void d16(); virtual void d17();
    virtual void d18(); virtual void d19(); virtual void d20(); virtual void d21(); virtual void d22(); virtual void d23();
    virtual void d24(); virtual void d25(); virtual void d26(); virtual void d27(); virtual void d28(); virtual void d29();
    virtual void d30(); virtual void d31(); virtual void d32(); virtual void d33(); virtual void d34(); virtual void d35();
    virtual void d36(); virtual void d37(); virtual void d38(); virtual void d39(); virtual void d40(); virtual void d41();
    virtual void d42(); virtual void d43(); virtual void d44(); virtual void d45(); virtual void d46(); virtual void d47();
    virtual void d48(); virtual void d49(); virtual void d50(); virtual void d51(); virtual void d52(); virtual void d53();
    virtual void d54(); virtual void d55(); virtual void d56(); virtual void d57(); virtual void d58(); virtual void d59();
    virtual void d60(); virtual void d61(); virtual void d62(); virtual void d63(); virtual void d64(); virtual void d65();
    virtual void d66(); virtual void d67(); virtual void d68(); virtual void d69(); virtual void d70(); virtual void d71();
    virtual void d72(); virtual void d73(); virtual void d74(); virtual void d75(); virtual void d76(); virtual void d77();
    virtual void d78(); virtual void d79(); virtual void d80(); virtual void d81(); virtual void d82(); virtual void d83();
    virtual void d84(); virtual void d85(); virtual void d86(); virtual void d87(); virtual void d88(); virtual void d89();
    virtual void d90(); virtual void d91(); virtual void d92(); virtual void d93(); virtual void d94(); virtual void d95();
    virtual void d96(); virtual void d97(); virtual void d98(); virtual void d99(); virtual void d100(); virtual void d101();
    virtual void d102(); virtual void d103(); virtual void d104(); virtual void d105(); virtual void d106(); virtual void d107();
    virtual void d108(); virtual void d109(); virtual void d110(); virtual void d111(); virtual void d112(); virtual void d113();
    virtual void d114(); virtual void d115(); virtual void d116(); virtual void d117(); virtual void d118(); virtual void d119();
    virtual void d120(); virtual void d121(); virtual void d122(); virtual void d123(); virtual void d124(); virtual void d125();
    virtual void d126(); virtual void d127(); virtual void d128(); virtual void d129(); virtual void d130(); virtual void d131();
    virtual void d132(); virtual void d133(); virtual void d134(); virtual void d135(); virtual void d136(); virtual void d137();
    virtual void d138(); virtual void d139(); virtual void d140(); virtual void d141(); virtual void d142(); virtual void d143();
    virtual void d144(); virtual void d145(); virtual void d146(); virtual void d147(); virtual void d148();
    virtual void UpdateValue();                  // slot 150 (+0x258)
    void ChangeState(unsigned long state);
};
#pragma pack(pop)

void CSlider::ChangeState(unsigned long state)
{
    switch (state) {
    case 0:
        if (CFrontEndManager::GetInstance()->GetLastInputResult() == 1) {
            OldValue = Value;
            Value = StartValue;
            if (Differs(OldValue, Value)) {
                for (unsigned int i = 0; i < Actions.size(); ++i) {
                    Actions[i]->PTarget->Target->Value->Value = Value.X;
                    CFrontEndManager::GetInstance()->ExecuteAction(Actions[i]);
                }
                Move = true;
            }
        }
        UpdateValue();
        StartValue = Value;
        CChangingStateComponent::ChangeState(state);
        break;
    case 3:
        CFrontEndManager::GetInstance()->AddObserver(this);
        ObserveEvent(5);
        ObserveEvent(4);
        ObserveEvent(0x14);
        ObserveEvent(0x15);
        CChangingStateComponent::ChangeState(state);
        break;
    case 5:
        UpdateValue();
        PreviousValue = StartValue = Value;
        if (PAcceptButton && !PAcceptButton->IsObservingEvent(0x19)) {
            CRGBColour colour = PAcceptButton->GetColour();
            colour.A = 0x80;
            PAcceptButton->ChangeColour(colour, 0.2f, false);
        }
        CChangingStateComponent::ChangeState(state);
        break;
    default:
        CFrontEndManager::GetInstance()->RemoveObserver(this);
        CChangingStateComponent::ChangeState(state);
        break;
    }
    if (Max.X == Min.X)
        Selectable = false;
}
}