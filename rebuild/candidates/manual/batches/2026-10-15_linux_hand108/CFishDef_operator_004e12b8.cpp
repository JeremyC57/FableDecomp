// CFishDef::operator=(const CFishDef&) -- def copy: CDefClassBase part, then twelve scalar fish parameters
// Leaf: ??4CFishDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E12B8, 93 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment).
// Identification: reached through the incremental-link thunk 0x004E12B3 (manifest "CFishDef::Copy", vtable 0x0123D808);
//   PDB CFishDef (0x58) = CDefClassBase + MinLength, MaxLength, MinWeight, MaxWeight, MinSpeed, MaxSpeed, BasePullSeconds,
//   MaxRandomPullSeconds, BasePullWaitSeconds, MaxRandomPullWaitSeconds (floats) +0x28..+0x4c, Level +0x50, InfoText +0x54.
//   Byte-identical to CBettingDef::operator= 0x004E1918.
// Shape: memberwise, floats moved through eax like dwords.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CFishDef : CDefClassBase {
    float MinLength;                    // +0x28
    float MaxLength;                    // +0x2c
    float MinWeight;                    // +0x30
    float MaxWeight;                    // +0x34
    float MinSpeed;                     // +0x38
    float MaxSpeed;                     // +0x3c
    float BasePullSeconds;              // +0x40
    float MaxRandomPullSeconds;         // +0x44
    float BasePullWaitSeconds;          // +0x48
    float MaxRandomPullWaitSeconds;     // +0x4c
    long Level;                         // +0x50
    long InfoText;                      // +0x54
    CFishDef& operator=(const CFishDef& other);
};

CFishDef& CFishDef::operator=(const CFishDef& other)
{
    CDefClassBase::operator=(other);
    MinLength = other.MinLength;
    MaxLength = other.MaxLength;
    MinWeight = other.MinWeight;
    MaxWeight = other.MaxWeight;
    MinSpeed = other.MinSpeed;
    MaxSpeed = other.MaxSpeed;
    BasePullSeconds = other.BasePullSeconds;
    MaxRandomPullSeconds = other.MaxRandomPullSeconds;
    BasePullWaitSeconds = other.BasePullWaitSeconds;
    MaxRandomPullWaitSeconds = other.MaxRandomPullWaitSeconds;
    Level = other.Level;
    InfoText = other.InfoText;
    return *this;
}
