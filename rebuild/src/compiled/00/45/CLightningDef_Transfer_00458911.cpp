// CLightningDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (Range, LifeTime, MaxNumberOfBranches, NeedLOSToBranch, DamagePeriod, DamagePerCycle, NodeDefIndex)
// Leaf: ?Transfer@CLightningDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458911, 90 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CLightningDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CLightningDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the seven transferred offsets and their callee
//   overloads match Ego_r.pdb CLightningDef's members and types exactly (retail offset = Ego_r offset - 4: the TLC parent def base is 0x3c bytes). Family G061 (seven def Transfers of seven fields; the G057 recipe).
// Callees: CPersistContext::Transfer(unsigned long&) 0x00431020, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061,
//   Transfer(bool&) 0x0043314A (each bound under that name by landed defs).
// Shape: context in edi and this in esi for the whole body; the last field's address is `add esi, N` in place.
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(unsigned long& value);                   // 0x00431020
    void Transfer(long& value);                            // 0x00431102
    void Transfer(float& value);                           // 0x00431061
    void Transfer(bool& value);                            // 0x0043314A
    template <class T> void Transfer(T& value);            // enum / class instances in this TU
};
class CLightningDef {                                              // retail fields from +0x3c
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                    // +0x04 def base body
    float Range;                             // +0x3c
    float LifeTime;                          // +0x40
    long MaxNumberOfBranches;                // +0x44
    bool NeedLOSToBranch;                    // +0x48
    float DamagePeriod;                      // +0x4c
    float DamagePerCycle;                    // +0x50
    long NodeDefIndex;                       // +0x54
};
void CLightningDef::Transfer(CPersistContext& context)
{
    context.Transfer(Range);
    context.Transfer(LifeTime);
    context.Transfer(MaxNumberOfBranches);
    context.Transfer(NeedLOSToBranch);
    context.Transfer(DamagePeriod);
    context.Transfer(DamagePerCycle);
    context.Transfer(NodeDefIndex);
}