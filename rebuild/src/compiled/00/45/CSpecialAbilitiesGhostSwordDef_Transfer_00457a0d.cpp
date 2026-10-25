// CSpecialAbilitiesGhostSwordDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (TimeOfSummon[0], TimeOfSummon[1], TimeOfSummon[2], TimeOfSummon[3], FadeInTime, FadeOutTime, CreationDistance)
// Leaf: ?Transfer@CSpecialAbilitiesGhostSwordDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00457A0D, 90 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CSpecialAbilitiesGhostSwordDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CSpecialAbilitiesGhostSwordDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the seven transferred offsets and their callee
//   overloads match Ego_r.pdb CSpecialAbilitiesGhostSwordDef's members and types exactly (retail offset = Ego_r offset - 4: the TLC def base is 0x3c bytes). Family G061 (seven def Transfers of seven fields; the G057 recipe).
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
class CSpecialAbilitiesGhostSwordDef {                                              // retail fields from +0x3c
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                    // +0x04 def base body
    float TimeOfSummon[4];                   // +0x3c
    float FadeInTime;                        // +0x4c
    float FadeOutTime;                       // +0x50
    float CreationDistance;                  // +0x54
};
void CSpecialAbilitiesGhostSwordDef::Transfer(CPersistContext& context)
{
    context.Transfer(TimeOfSummon[0]);
    context.Transfer(TimeOfSummon[1]);
    context.Transfer(TimeOfSummon[2]);
    context.Transfer(TimeOfSummon[3]);
    context.Transfer(FadeInTime);
    context.Transfer(FadeOutTime);
    context.Transfer(CreationDistance);
}