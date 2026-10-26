// NUISystem::CScrollingViewport::ChangeState(unsigned long) -- (un)register the viewport and its scroll bars as front-end observers on activate/deactivate
// Leaf: ChangeState
// Retail 0x0053EBF0, 336 bytes, ret 4. Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall), the manager's
// AddObserver (slot 2, +0x08) / RemoveObserver (slot 5, +0x14), CObserver::ObserveEvent (slot 3, +0x0C, virtual here),
// CChangingStateComponent::ChangeState 0x0052CF40. Global: the active scrolling viewport 0x013B8AAC.
// Layout: as the landed ??_G 0x0053FC30 / ctor 0x0053F120 -- three bases (primary +0, CObserver +4, interface +0x18),
// PScrollBar +0x188, PScrollBarOutside +0x18C (components whose CObserver base is also at +4).
// Shape: a plain switch (VC's `sub 0 ; dec ; sub 2` chain, default arm inline after the tests); passing `this` to the
// manager converts to the CObserver base with VC's null test (`test esi,esi ; lea edi,[esi+4] / xor edi,edi`), the
// scroll-bar pointers' conversions fold into their `if` tests; state 0 is constant-propagated into the base call; the
// last ObserveEvent of the activate arm becomes a `jmp [eax+0Ch]` tail call.
struct IViewportPrimary {
    virtual void v00();
};
struct CObserver {
    virtual void a00(); virtual void a01(); virtual void a02();
    virtual void ObserveEvent(long eventType);           // slot 3 (+0x0c)
    unsigned char _pad04[0x10];
};
struct IViewportSecondaryB {
    virtual void b00();
};
struct CChangingStateComponent : IViewportPrimary, CObserver, IViewportSecondaryB {
    unsigned char _pad1c[0x188 - 0x1c];
    void ChangeState(unsigned long state);               // 0x0052CF40
};
struct CFrontEndManager {
    virtual void v000(); virtual void v001();
    virtual void AddObserver(CObserver* observer);       // slot 2 (+0x08)
    virtual void v003(); virtual void v004();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CScrollBarComponent : IViewportPrimary, CObserver {
};

namespace NUISystem {
class CScrollingViewport;
extern CScrollingViewport* g_pActiveScrollingViewport;  // 0x013B8AAC

class CScrollingViewport : public CChangingStateComponent { public:
    CScrollBarComponent* PScrollBar;                     // +0x188
    CScrollBarComponent* PScrollBarOutside;              // +0x18C
    bool  Changed;                                       // +0x190
    float OffsetToUseAbove;                              // +0x194
    float OffsetToUseUnder;                              // +0x198
    void ChangeState(unsigned long state);
};

void CScrollingViewport::ChangeState(unsigned long state)
{
    switch (state) {
    case 0:
        CFrontEndManager::GetInstance()->RemoveObserver(this);
        if (g_pActiveScrollingViewport == this)
            g_pActiveScrollingViewport = 0;
        CChangingStateComponent::ChangeState(state);
        if (PScrollBar)
            CFrontEndManager::GetInstance()->RemoveObserver(PScrollBar);
        if (PScrollBarOutside)
            CFrontEndManager::GetInstance()->RemoveObserver(PScrollBarOutside);
        break;
    case 1:
    case 3:
        CFrontEndManager::GetInstance()->AddObserver(this);
        ObserveEvent(6);
        ObserveEvent(7);
        ObserveEvent(0x19);
        if (g_pActiveScrollingViewport == 0)
            g_pActiveScrollingViewport = this;
        CChangingStateComponent::ChangeState(state);
        if (PScrollBar) {
            CFrontEndManager::GetInstance()->AddObserver(PScrollBar);
            PScrollBar->ObserveEvent(0x19);
        }
        if (PScrollBarOutside) {
            CFrontEndManager::GetInstance()->AddObserver(PScrollBarOutside);
            PScrollBarOutside->ObserveEvent(0x19);
        }
        break;
    default:
        CChangingStateComponent::ChangeState(state);
        break;
    }
}
}