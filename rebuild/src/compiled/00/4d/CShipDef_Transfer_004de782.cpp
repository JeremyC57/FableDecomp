// CShipDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (SightingFrequency, SightingOffset, DaysVisible, Swell, Pitch, Roll, WaveLength)
// Leaf: ?Transfer@CShipDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE782, 90 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CShipDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CShipDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the seven transferred offsets and their callee
//   overloads match Ego_r.pdb CShipDef's members and types exactly (retail offset = Ego_r offset). Family G061 (seven def Transfers of seven fields; the G057 recipe).
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
class CShipDef {                                              // retail fields from +0x28
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x24];                    // +0x04 def base body
    long SightingFrequency;                  // +0x28
    long SightingOffset;                     // +0x2c
    long DaysVisible;                        // +0x30
    float Swell;                             // +0x34
    float Pitch;                             // +0x38
    float Roll;                              // +0x3c
    float WaveLength;                        // +0x40
};
void CShipDef::Transfer(CPersistContext& context)
{
    context.Transfer(SightingFrequency);
    context.Transfer(SightingOffset);
    context.Transfer(DaysVisible);
    context.Transfer(Swell);
    context.Transfer(Pitch);
    context.Transfer(Roll);
    context.Transfer(WaveLength);
}