// NFire::CFirePoint::AddFireDamage(long damage) -- take fire damage off the fire point's resistance; while some resistance is left the point smoulders
// Leaf: ?AddFireDamage@CFirePoint@NFire@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00780800, 26 bytes, `ret 4`.
// Identity: Ego_r.pdb `?AddFireDamage@CFirePoint@NFire@@QAEXJ@Z` (void(long)). The CTCFlammable TU holds the CFirePoint
//   members (landed SetAsBurning 0x00780540, Reset 0x00780700); the only CFirePoint void(long) in the PDB name set is
//   AddFireDamage, and the callee 0x007805D0 is CFirePoint::SetAsSmoldering (Ego_r.pdb `?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ`,
//   private: it creates the singe effect into PSingeEffect +0x24). No direct callers were found for this body (the two
//   .rdata hits are data words). Catalog row was the placeholder `_gapscan::sub_00780800`.
// Layout: Ego_r.pdb NFire::CFirePoint (0x2c, also the retail element stride of CTCFlammable::FirePoints): HelperName +0x00
//   (CCharString), HelperIndex +0x04, EffectIndex +0x08, Resistance +0x0c, Burning +0x10, FireDamageAfterFrame +0x14,
//   PParentThing +0x18, PEffect +0x1c, PSingeEffect +0x24 (CIntelligentPointer<CThing>, 8 bytes each).
// Callees: NFire::CFirePoint::SetAsSmoldering() @ 0x007805D0 (call, not a tail jump: this function pops a stack argument).
// Shape: the new resistance is stored and tested from one register (`sub edx,eax ; mov eax,edx ; test eax,eax ; mov
//   [ecx+0Ch],edx ; jle`).
class CThing;

namespace NFire {
class CFirePoint {
public:
    void*         HelperName;                   // +0x00 CCharString
    long          HelperIndex;                  // +0x04
    long          EffectIndex;                  // +0x08
    long          Resistance;                   // +0x0c
    bool          Burning;                      // +0x10
    long          FireDamageAfterFrame;         // +0x14
    CThing*       PParentThing;                 // +0x18
    unsigned char PEffect[8];                   // +0x1c CIntelligentPointer<CThing>
    unsigned char PSingeEffect[8];              // +0x24 CIntelligentPointer<CThing>

    void AddFireDamage(long damage);
private:
    void SetAsSmoldering();                     // 0x007805D0
};

void CFirePoint::AddFireDamage(long damage)
{
    Resistance -= damage;
    if (Resistance > 0)
        SetAsSmoldering();
}
}