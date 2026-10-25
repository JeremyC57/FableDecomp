// CTavernGameCoinBaseDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (CoinIDensity, CoinRadius, CoinEdgeRestitution, CoinEdgeFriction, CoinSurfaceFriction, CoinDefIndex, ArrowDefIndex)
// Leaf: ?Transfer@CTavernGameCoinBaseDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEC2C, 90 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CTavernGameCoinBaseDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CTavernGameCoinBaseDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the seven transferred offsets and their callee
//   overloads match Ego_r.pdb CTavernGameCoinBaseDef's members and types exactly (retail offset = Ego_r offset). Family G061 (seven def Transfers of seven fields; the G057 recipe).
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
class CTavernGameCoinBaseDef {                                              // retail fields from +0x28
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x24];                    // +0x04 def base body
    float CoinIDensity;                      // +0x28
    float CoinRadius;                        // +0x2c
    float CoinEdgeRestitution;               // +0x30
    float CoinEdgeFriction;                  // +0x34
    float CoinSurfaceFriction;               // +0x38
    long CoinDefIndex;                       // +0x3c
    long ArrowDefIndex;                      // +0x40
};
void CTavernGameCoinBaseDef::Transfer(CPersistContext& context)
{
    context.Transfer(CoinIDensity);
    context.Transfer(CoinRadius);
    context.Transfer(CoinEdgeRestitution);
    context.Transfer(CoinEdgeFriction);
    context.Transfer(CoinSurfaceFriction);
    context.Transfer(CoinDefIndex);
    context.Transfer(ArrowDefIndex);
}