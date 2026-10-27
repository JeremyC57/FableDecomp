// CSpecialAbilitiesDrunkennessDef::Transfer(CPersistContext& context) -- persist the def's own members: DrunkenGraphic, MaxAlpha, USpeed, VSpeed, RotSpeed
// Leaf: ?Transfer@CSpecialAbilitiesDrunkennessDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00457F13, 68 bytes, thiscall, `ret 4`.
// Identity (proven): slot 18 of the CSpecialAbilitiesDrunkennessDef vtable 0x012347AC (rtti-vtables); FableWin.pdb ?Transfer@CSpecialAbilitiesDrunkennessDef@@UAEXAAVCPersistContext@@@Z;
//   the transferred members are Ego_r.pdb's CSpecialAbilitiesDrunkennessDef fields at their retail offsets (retail = Ego_r.pdb - 4).
// Callees: Transfer<long> 0x00431102 (DrunkenGraphic), Transfer<long> 0x00431102 (MaxAlpha), Transfer<float> 0x00431061 (USpeed), Transfer<float> 0x00431061 (VSpeed), Transfer<float> 0x00431061 (RotSpeed).
// Shape (family K094, representative 0x00457F13): one CPersistContext::Transfer<T>(T&) call per member, the context kept in edi;
//   size TU (every member address formed in eax).
#pragma optimize("s", on)
class CPersistContext {
public:
    template <class T> void Transfer(T& value);          // per-type helpers (out of line)
};
class CParentDefClassBase {                                            // retail 0x3C bytes (members not touched here)
public:
    virtual ~CParentDefClassBase();
    unsigned char _body[0x3C - 4];
};
class CSpecialAbilitiesDrunkennessDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    long                DrunkenGraphic;                    // +0x3C
    long                MaxAlpha;                          // +0x40
    float               USpeed;                            // +0x44
    float               VSpeed;                            // +0x48
    float               RotSpeed;                          // +0x4C
};
void CSpecialAbilitiesDrunkennessDef::Transfer(CPersistContext& context)
{
    context.Transfer(DrunkenGraphic);
    context.Transfer(MaxAlpha);
    context.Transfer(USpeed);
    context.Transfer(VSpeed);
    context.Transfer(RotSpeed);
}