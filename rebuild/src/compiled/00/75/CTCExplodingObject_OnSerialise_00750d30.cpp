// CTCExplodingObject::OnSerialise(CPersistContext&, bool) -- persist MaxDamage, Radius (default 0), FireDamage (default 10), TriggeredOnCreatureProximity (default false) and TriggerRadius (default 0)
// Leaf: OnSerialise
//
// Retail 0x00750D30, 157 bytes (`ret 8`). Vtable 0x01269674 (CTCExplodingObject) slot 1, the
// CTCBase::OnSerialise(CPersistContext&, bool) override (Ego_r.pdb `?OnSerialise@CTCBase@@MAEXAAV
// CPersistContext@@_N@Z`); the catalog "CTCExplodingObject::vfunc_1" is the slot name. Callees:
// CPersistContext::Transfer<float> @ 0x00410620, Transfer<long> @ 0x00410BE0, Transfer<bool> @
// 0x004045C0 (each `(name, value&, const default&)`). Keys 0x01269728 "MaxDamage", 0x01269720 "Radius",
// 0x01269714 "FireDamage", 0x012696F4 "TriggeredOnCreatureProximity", 0x012696E4 "TriggerRadius".
// Layout = Ego_r.pdb CTCExplodingObject (0x30, no retail shift): MaxDamage +0xc, Radius +0x10,
// FireDamage +0x14, TriggeredOnCreatureProximity +0x1c, TriggerRadius +0x20.
// Shape: the landed CTCCameraPointFixedPoint slot-33 body (0x0075D140): literal defaults bound to the
// const references, homed in the frame word (first) and then the dead context-argument slot; the
// unused bool parameter only shows in `ret 8`.
struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
    void Transfer(const char* name, long& value, const long& defaultValue);     // 0x00410BE0
    void Transfer(const char* name, bool& value, const bool& defaultValue);     // 0x004045C0
};

struct CThing;

struct CTCExplodingObject {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    float   MaxDamage;                                      // +0x0c
    float   Radius;                                         // +0x10
    long    FireDamage;                                     // +0x14
    long    CountdownTimer;                                 // +0x18
    bool    TriggeredOnCreatureProximity;                   // +0x1c
    float   TriggerRadius;                                  // +0x20
    void OnSerialise(CPersistContext& context, bool loading);
};

void CTCExplodingObject::OnSerialise(CPersistContext& context, bool loading)
{
    context.Transfer("MaxDamage", MaxDamage, 0.0f);
    context.Transfer("Radius", Radius, 0.0f);
    context.Transfer("FireDamage", FireDamage, 10L);
    context.Transfer("TriggeredOnCreatureProximity", TriggeredOnCreatureProximity, false);
    context.Transfer("TriggerRadius", TriggerRadius, 0.0f);
}