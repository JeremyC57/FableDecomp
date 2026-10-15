// CNavigatorBase::CNavigatorBase(CNavigatorManager&, unsigned long, const CNavigatorExternalInterface&) -- register the navigator with its manager (CClass base), clone the external interface and create the steering agent over it
// Leaf: ??0CNavigatorBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A47110, 99 bytes (+ int3 padding; `ret 0Ch`). Vtable 0x0129D4A4.
// Callees: CClass::CClass(CClassFactory&, unsigned long, bool) @ 0x00A372D0, the interface's virtual slot 24 (+0x60, clone),
//   operator new @ 0x00BFEA1A (0x68 bytes), CSteeringAgent::CSteeringAgent(const CNavigatorExternalInterface*, bool) @ 0x00A1C700.
// Layout = Ego_r.pdb CNavigatorBase: PInterface +0x14, OnRoute +0x18, Interrupted +0x19, NavigationManager +0x1c, SteeringAgent +0x20.
// Shape: the flags and the manager are initialisers (stored right after the vtable); the cloned interface and the steering agent
//   are body assignments (the null-checked `new` stores 0 on its failure path); the agent ctor re-reads PInterface.
struct CClassFactory {
    virtual void v00();
};
struct CNavigatorManager : CClassFactory {
};
struct CClass {
    virtual void v00();
    unsigned char _pad04[0x10];
    CClass(CClassFactory& factory, unsigned long type, bool registerNow);   // 0x00A372D0
};
struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual CNavigatorExternalInterface* Clone() const;     // slot 24 (+0x60)
};
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A
struct CSteeringAgent {
    unsigned char _data[0x68];
    CSteeringAgent(const CNavigatorExternalInterface* iface, bool b);   // 0x00A1C700
};
struct CNavigatorBase : CClass {
    CNavigatorExternalInterface* PInterface;                // +0x14
    bool                         OnRoute;                   // +0x18
    bool                         Interrupted;               // +0x19
    CNavigatorManager*           NavigationManager;         // +0x1c
    CSteeringAgent*              SteeringAgent;             // +0x20
    CNavigatorBase(CNavigatorManager& manager, unsigned long type, const CNavigatorExternalInterface& iface);
    virtual void v00();
};

CNavigatorBase::CNavigatorBase(CNavigatorManager& manager, unsigned long type, const CNavigatorExternalInterface& iface)
    : CClass(manager, type, true), OnRoute(false), Interrupted(false), NavigationManager(&manager)
{
    PInterface = iface.Clone();
    SteeringAgent = new CSteeringAgent(PInterface, false);
}