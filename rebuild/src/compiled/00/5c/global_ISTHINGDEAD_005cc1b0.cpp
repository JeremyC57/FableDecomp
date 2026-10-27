// IS_THING_DEAD(const CIntelligentPointer<CThing>& thing) -- true when the pointer is empty or its thing is dead
// Leaf: ??$IS_THING_DEAD@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CC1B0, 36 bytes, `__fastcall` (the reference in ecx), plain `ret`.
// Identity (proven): FableWin.pdb ??$IS_THING_DEAD@V?$CIntelligentPointer@VCThing@@@@@@...; CTCInventoryWeapons::CheckForSoldWieldedWeapons 0x005CB780 calls it (4x) on PActiveMeleeWeapon +0x134 / PActiveRangedWeapon +0x141, which Ego_r.pdb types CIntelligentPointer<CThing>; FableWin.pdb and Ego_r.pdb have ??$IS_THING_DEAD@V?$CIntelligentPointer@VCThing@@@@@@...; it is also the 2nd of the nine bodies, matching FableWin.pdb's 2nd instantiation by address (<CThing>).
//   The manifest's "IS_THING_ALIVE<CIntelligentPointer<CThingPhysical const> >" is propagated (the body is true for null / dead).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice).
// Shape (family M155, representative 0x005CC1B0): the landed 0x00935A10 body -- `if (thing.GetPItem() != 0) { if
//   (!thing.GetPItem()->IsDead()) return false; } return true;`; the thing's dead bit is +0x91 bit 0. Size TU.
#pragma optimize("s", on)
class CBaseObject;
class CBaseObjectPointer;
class CThing {
public:
    unsigned char _pad[0x91];
    unsigned char Flags;                                  // +0x91 bit 0: dead
    bool IsDead() const { return (Flags & 1) != 0; }
};
class CBaseIntelligentPointer {                           // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                            // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObject* GetPItem() const;                        // 0x00A01B50
private:
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> bool __fastcall IS_THING_DEAD(const T& thing)
{
    if (thing.GetPItem() != 0) {
        if (!thing.GetPItem()->IsDead())
            return false;
    }
    return true;
}
template bool __fastcall IS_THING_DEAD(const CIntelligentPointer<CThing>& thing);