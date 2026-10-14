// NUISystem::CHoverable::CHoverable(long owner) -- the CChangingStateComponent base (0x0052CC50), the three vtables (0x0124BFB4 / 0x0124BF90 / 0x0124BF88), the members at +0x15c..+0x168 cleared, then the front-end manager (GetInstance @ 0x0041E5F2, `__fastcall`) registers this component's first interface (+4) through its virtual slot 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055BA20, length 96 bytes.
// Shape: the interface address in edi before the vtable stores, `mov eax,esi`, `ret 4`.
namespace NUISystem {
struct IComponentA {                               // the interface base at +0x04 (vtable, 0x10 bytes of state)
    virtual void va00();
    unsigned char _state[0x10];
};
struct IComponentB {                               // the interface base at +0x18 (vtable only)
    virtual void vb00();
};
struct CComponentRoot {                            // the primary base: vptr at +0
    virtual void v00();
};
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x15c - 0x1c];             // +0x1c .. +0x15c: the base's state
    CChangingStateComponent(long owner);           // 0x0052CC50 (labelled "CDraggableInto")
};
struct CFrontEndManager_Calls {
    virtual void v000(); virtual void v001();
    virtual void RegisterHoverable(IComponentA* component);   // slot 2 (+0x08)
    static CFrontEndManager_Calls* __fastcall GetInstance();   // 0x0041E5F2
};
struct CHoverable : CChangingStateComponent {      // vtables 0x0124BFB4 / 0x0124BF90 / 0x0124BF88
    virtual void v00();
    virtual void va00();
    virtual void vb00();
    long  m_15c;                                   // +0x15c
    bool  m_160;                                   // +0x160
    long  m_164;                                   // +0x164
    long  m_168;                                   // +0x168
    CHoverable(long owner);
};

CHoverable::CHoverable(long owner)
    : CChangingStateComponent(owner), m_15c(0), m_160(false), m_164(0), m_168(0)
{
    CFrontEndManager_Calls::GetInstance()->RegisterHoverable(this);
}
}
