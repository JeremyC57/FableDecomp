// CTCHitLocations::GetPCombatFirstHitLocationPosInside(const C3DVector& pos) -- among the enabled hit locations whose physical primitive contains the position, returns the one with the lowest priority value (the first one on ties), or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008153C0, length 86 bytes (+ int3 padding).
// Callee: NHitLocations::CPhysicalPrimitiveBase::IsPosInside(const CThing&, const C3DVector&) const,
//         virtual slot 9 (`call [edx+24h]`; Ego_r PDB overrides for Mesh/Sphere/Cylinder).
// Ego_r PDB: ?GetPCombatFirstHitLocationPosInside@CTCHitLocations@@QAEPAVCHitLocation@NHitLocations@@ABVC3DVector@@@Z.
// Layout: HitLocations is the retail 12-byte std::vector<pair<NHitLocations::CHitLocation, long> > at
// +0x0c (0x1c-byte elements: CHitLocation is 0x18 -- POwnerThing +0, PDef +4, PArmourDef +8, the two
// response actions, Enabled +0x14 -- and the priority long at +0x18). The CHitLocationDef's
// PhysicalPrimitive counted pointer sits at retail +0x3c (PDB +0x40).
// Shape: best pointer in ebx (0), best priority in ebp (0x7FFFFFFF); the end pointer is re-read
// after every element because the loop body makes a virtual call; `!best || priority < bestPriority`
// gives `test ebx,ebx ; je take ; cmp [esi+18h],ebp ; jge next`.
struct CThing;
struct C3DVector;

namespace NHitLocations {

struct CPhysicalPrimitiveBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsPosInside(const CThing& owner, const C3DVector& pos) const;   // slot 9
};

struct CHitLocationDef {
    unsigned char           _pad_0x00[0x3c];
    CPhysicalPrimitiveBase* PhysicalPrimitive;   // +0x3c CPhysicalPrimitiveInit::PPrimitive (PDB +0x40)
};

struct CHitLocation {
    CThing*                POwnerThing;         // +0x00
    const CHitLocationDef* PDef;                // +0x04 CDefPointer<const CHitLocationDef>
    void*                  PArmourDef;          // +0x08
    long                   HitResponseAction;   // +0x0c
    long                   BlockResponseAction; // +0x10
    bool                   Enabled;             // +0x14
};

}

struct SHitLocationEntry {                        // std::pair<NHitLocations::CHitLocation, long>
    NHitLocations::CHitLocation first;
    long                        second;          // +0x18 priority
};

struct CTCHitLocations {
    unsigned char      _pad_0x00[0x0c];
    SHitLocationEntry* HitLocations_First;       // +0x0c
    SHitLocationEntry* HitLocations_Last;        // +0x10
    SHitLocationEntry* HitLocations_End;         // +0x14
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationPosInside(const C3DVector& pos);
};

NHitLocations::CHitLocation* CTCHitLocations::GetPCombatFirstHitLocationPosInside(const C3DVector& pos)
{
    NHitLocations::CHitLocation* best = 0;
    long bestPriority = 0x7fffffff;
    for (SHitLocationEntry* it = HitLocations_First; it != HitLocations_Last; ++it) {
        if (it->first.Enabled && it->first.PDef->PhysicalPrimitive->IsPosInside(*it->first.POwnerThing, pos)) {
            if (best == 0 || it->second < bestPriority) {
                bestPriority = it->second;
                best = &it->first;
            }
        }
    }
    return best;
}