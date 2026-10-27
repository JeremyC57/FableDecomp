// CThingDrainLifeShotDef::Transfer(CPersistContext& context) -- persist the def's own members: MaxAngle, MaxDistance, MaxAngleToTurnDegs, InitialSpeed, SpeedRange
// Leaf: ?Transfer@CThingDrainLifeShotDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DEA13, 68 bytes, thiscall, `ret 4`.
// Identity (proven): slot 18 of the CThingDrainLifeShotDef vtable 0x0123CCA4 (rtti-vtables); FableWin.pdb ?Transfer@CThingDrainLifeShotDef@@UAEXAAVCPersistContext@@@Z;
//   the transferred members are Ego_r.pdb's CThingDrainLifeShotDef fields at their retail offsets (retail = Ego_r.pdb).
// Callees: Transfer<float> 0x00431061 (MaxAngle), Transfer<float> 0x00431061 (MaxDistance), Transfer<float> 0x00431061 (MaxAngleToTurnDegs), Transfer<float> 0x00431061 (InitialSpeed), Transfer<float> 0x00431061 (SpeedRange).
// Shape (family K094, representative 0x00457F13): one CPersistContext::Transfer<T>(T&) call per member, the context kept in edi;
//   size TU (every member address formed in eax).
#pragma optimize("s", on)
class CPersistContext {
public:
    template <class T> void Transfer(T& value);          // per-type helpers (out of line)
};
class CSubDefClassBase {                                            // retail 0x28 bytes (members not touched here)
public:
    virtual ~CSubDefClassBase();
    unsigned char _body[0x28 - 4];
};
class CThingDrainLifeShotDef : public CSubDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    float               MaxDistance;                       // +0x28
    float               MaxAngle;                          // +0x2C
    float               MaxAngleToTurnDegs;                // +0x30
    float               InitialSpeed;                      // +0x34
    float               SpeedRange;                        // +0x38
};
void CThingDrainLifeShotDef::Transfer(CPersistContext& context)
{
    context.Transfer(MaxAngle);
    context.Transfer(MaxDistance);
    context.Transfer(MaxAngleToTurnDegs);
    context.Transfer(InitialSpeed);
    context.Transfer(SpeedRange);
}