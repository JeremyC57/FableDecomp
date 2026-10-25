// CTavernGameCoinBaseDef::operator=(const CTavernGameCoinBaseDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then CoinIDensity, CoinRadius, CoinEdgeRestitution, CoinEdgeFriction, CoinSurfaceFriction, CoinDefIndex, ArrowDefIndex memberwise
// Leaf: ??4CTavernGameCoinBaseDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E10C6, 63 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CTavernGameCoinBaseDef::operator=(const CTavernGameCoinBaseDef&) -- proven; evidence: its only caller is the landed CTavernGameCoinBaseDef::Copy 0x004E10C1
//   (a `jmp` to it: `*this = *static_cast<const CTavernGameCoinBaseDef*>(other)`); RTTI of vtable 0x0123D44C gives CTavernGameCoinBaseDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CTavernGameCoinBaseDef's seven members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CTavernGameCoinBaseDef@@QAEAAV0@ABV0@@Z. Family G058 (seven def copy assignments; catalogue "CThingBuildingDef::operator=" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
class CDefClassBase {                                      // retail 0x25 bytes (+ tail padding)
public:
    virtual ~CDefClassBase();
    CDefClassBase& operator=(const CDefClassBase& other);  // 0x00431F10
    unsigned char _body[0x25 - 4];
};
class CSubDefClassBase : public CDefClassBase {           // no members: its implicit operator= inlines to the base call
};
class CTavernGameCoinBaseDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    float CoinIDensity;                      // +0x28
    float CoinRadius;                        // +0x2c
    float CoinEdgeRestitution;               // +0x30
    float CoinEdgeFriction;                  // +0x34
    float CoinSurfaceFriction;               // +0x38
    long CoinDefIndex;                       // +0x3c
    long ArrowDefIndex;                      // +0x40
};
void CTavernGameCoinBaseDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CTavernGameCoinBaseDef*>(other);
}