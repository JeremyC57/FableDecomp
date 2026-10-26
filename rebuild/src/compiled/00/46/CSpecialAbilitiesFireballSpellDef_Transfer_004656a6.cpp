// CSpecialAbilitiesFireballSpellDef::Transfer(CPersistContext& context) -- persist the fireball spell def: inclination, release delay, per-level stamina cost and targetting arc angle
// Leaf: ?Transfer@CSpecialAbilitiesFireballSpellDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004656A6, 57 bytes (virtual, `ret 4`; unaligned entry, def module).
// Callees (typed CPersistContext transfers, catalog labels propagated): float 0x00431061, vector<float> 0x00464CBF
//   (the instantiations the landed CSpecialAbilitiesPhysicalShieldDef::Transfer 0x00465390 calls).
// Layout: Ego_r.pdb CSpecialAbilitiesFireballSpellDef (0x5c) re-based on the retail bytes: the special-abilities def
//   base ends at +0x3c (PDB +0x40) and the STLport vector is 12 bytes (PDB 16), so ReleaseDelay +0x3c, Inclination +0x40,
//   StaminaCost +0x44, TargettingArcAngle +0x50. No generated engine header for the class.
// Pragma: #pragma optimize("s", on) -- a size TU: every member address goes through eax.
// Shape: one transfer per member in the def's TRANSFER list order (Inclination before ReleaseDelay); the last
//   member's address is the natural `add esi,50h ; push esi`.
#pragma optimize("s", on)

template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

class CPersistContext {
public:
    template <class T> void Transfer(T& value);                // 0x00431061
    void Transfer(CStlVector<float>& value);    // 0x00464CBF
};

class CSpecialAbilitiesFireballSpellDef {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                  // CSpecialAbilitiesDef data (+0x04..+0x3b)
    float ReleaseDelay;                         // +0x3c
    float Inclination;                          // +0x40
    CStlVector<float> StaminaCost;              // +0x44
    float TargettingArcAngle;                   // +0x50
};

void CSpecialAbilitiesFireballSpellDef::Transfer(CPersistContext& context)
{
    context.Transfer(Inclination);
    context.Transfer(ReleaseDelay);
    context.Transfer(StaminaCost);
    context.Transfer(TargettingArcAngle);
}