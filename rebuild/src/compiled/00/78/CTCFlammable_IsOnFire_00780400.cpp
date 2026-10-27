// CTCFlammable::IsOnFire() const -- whether the flammable component's fire resistance is used up
// Leaf: ?IsOnFire@CTCFlammable@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00780400, 11 bytes, plain `ret`.
// Identity: Ego_r.pdb `?IsOnFire@CTCFlammable@@QBE_NXZ` (manifest label); callers include the landed
//   CThingFilter_IsOnFire::operator() 0x006A8A80 and CGameScriptInterface::IsThingOnFire 0x0089BD90.
// Layout: as the landed CTCFlammable::ExtinguishFire 0x00780C80: PDef +0x0c, FirePoints +0x10 (12-byte STLport vector),
//   Burning +0x1c, FireDamageAfterFrame +0x20, FireResistance +0x24, FireStrength +0x28.
// Shape: `xor eax,eax ; test ; setle al`.
class CThing;

class CTCFlammable {
public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    void*         PDef;                         // +0x0c CDefPointer<const CFlammableDef>
    unsigned char FirePoints[0xc];              // +0x10 vector<NFire::CFirePoint> (STLport, 12 bytes; not touched here)
    bool          Burning;                      // +0x1c
    long          FireDamageAfterFrame;         // +0x20
    long          FireResistance;               // +0x24
    long          FireStrength;                 // +0x28
    bool IsOnFire() const;
};

bool CTCFlammable::IsOnFire() const
{
    return FireResistance <= 0;
}