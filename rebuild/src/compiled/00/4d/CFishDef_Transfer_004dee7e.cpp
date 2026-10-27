// CFishDef::Transfer(CPersistContext& context) -- persist every member without member names
// Leaf: ?Transfer@CFishDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DEE7E, 145 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): slot 18 of vtable 0x0123D7BC (RTTI .?AVCFishDef@@; Ego_r.pdb ?Transfer@CFishDef@@UAEXAAVCPersistContext@@@Z);
//   the 12 Transfer calls take, in call order, the members at +0x28, +0x2c, +0x30, +0x34, +0x38, +0x3c, +0x40, +0x44, +0x48, +0x4c, +0x50, +0x54 with the per-type callees 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431102, 0x00431102, which are
//   Ego_r.pdb CFishDef's members MinLength, MaxLength, MinWeight, MaxWeight, MinSpeed, MaxSpeed, BasePullSeconds, MaxRandomPullSeconds, BasePullWaitSeconds, MaxRandomPullWaitSeconds, Level, InfoText.
// Shape (family M053, representative 0x0046923D): `context.Transfer(Member);` per member; `optimize("s", on)` (each member
//   address formed with `lea eax,[esi+N]`, the last as `add esi,N`).
// Header debt: the def base is modelled by its retail size (pack(1), vptr + members) with the virtual Transfer only.
#pragma optimize("s", on)
class CPersistContext {
public:
    template <class T> void Transfer(T& value);
};
#pragma pack(push, 1)
class CDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x25 - 4];
};
#pragma pack(pop)
class CFishDef : public CDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    float                    MinLength;                     // +0x28
    float                    MaxLength;                     // +0x2c
    float                    MinWeight;                     // +0x30
    float                    MaxWeight;                     // +0x34
    float                    MinSpeed;                      // +0x38
    float                    MaxSpeed;                      // +0x3c
    float                    BasePullSeconds;               // +0x40
    float                    MaxRandomPullSeconds;          // +0x44
    float                    BasePullWaitSeconds;           // +0x48
    float                    MaxRandomPullWaitSeconds;      // +0x4c
    long                     Level;                         // +0x50
    long                     InfoText;                      // +0x54
};
void CFishDef::Transfer(CPersistContext& context)
{
    context.Transfer(MinLength);
    context.Transfer(MaxLength);
    context.Transfer(MinWeight);
    context.Transfer(MaxWeight);
    context.Transfer(MinSpeed);
    context.Transfer(MaxSpeed);
    context.Transfer(BasePullSeconds);
    context.Transfer(MaxRandomPullSeconds);
    context.Transfer(BasePullWaitSeconds);
    context.Transfer(MaxRandomPullWaitSeconds);
    context.Transfer(Level);
    context.Transfer(InfoText);
}