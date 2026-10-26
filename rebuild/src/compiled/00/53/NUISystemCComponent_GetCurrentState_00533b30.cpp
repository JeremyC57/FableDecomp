// NUISystem::CComponent::GetCurrentState() -- the base component's current state is always its state 0 (created on first use)
// Leaf: ?GetCurrentState@CComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00533B30, 23 bytes (+9 int3 padding); vtable 0x0124608C slot 50.
// Identity: FableWin.pdb ?GetCurrentState@CComponent@NUISystem@@UAEPAVCUIState@2@XZ; the only override of slot 50,
//          CSprite 0x0052CF10, finds its own current-state index (+0x144) in the same map and returns the entry -- the
//          GetCurrentState shape -- while this base form indexes state 0.
// Callees: STLport map<unsigned long, CUIState>::operator[](const unsigned long&) @ 0x0042D5B1 (manifest
//          "CPersistTraits::Transfer" is a propagated label; the landed CSlider::GetValueFromProfile @ 0x0054BBB0 binds it
//          as operator[]).
// Layout: retail CComponent States (map, 12 bytes) +0x24, as the landed 0x0054BBB0 model; no engine/ header exists.
// Shape: the literal key is a frame temporary whose address is pushed before its zero store; the reference result is
//          returned as the pointer.
class C2DVector { public: float X, Y; };
struct CUIState {
    unsigned long StateChangeFlag;                 // +0x00
    long          StateChangeType;                 // +0x04
    C2DVector     Position;                        // +0x08
};
struct CStateMap {                                 // STLport map<unsigned long, CUIState> (12 bytes)
    void*         Header;
    unsigned long NodeCount;
    unsigned long Compare;
    CUIState& operator[](const unsigned long& key);   // 0x0042D5B1
};

namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();
    virtual CUIState* GetCurrentState();           // slot 50 in retail
    unsigned char _pad04[0x24 - 0x04];
    CStateMap     States;                          // +0x24
};

CUIState* CComponent::GetCurrentState()
{
    return &States[0];
}
}