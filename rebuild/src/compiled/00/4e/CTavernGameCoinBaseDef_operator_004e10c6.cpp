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
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
#include "engine/CSubDefClassBase.h"                    // vtable 0x01231D54
// def vtable 0x0123D44C: slot 0 0x004D8F75 (implicit inline dtor); real overrides from the def vtable scan
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
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DEC2C
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D5991
};
void CTavernGameCoinBaseDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CTavernGameCoinBaseDef*>(other);
}