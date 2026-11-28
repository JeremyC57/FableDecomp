#ifndef FABLE_ENGINE_CTCHITLOCATIONS_H
#define FABLE_ENGINE_CTCHITLOCATIONS_H
#ifdef FABLE_RETAIL_HITLOCATIONS_BOUND
#include "engine/CTCBase.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
class CHitLocationsDef;
class CTCHitLocations:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_HIT_LOCATIONS;}
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCHitLocations(const CTCHitLocations & p0);
 CTCHitLocations(CThing & p0);
 virtual void OnInitialActivate();
 NHitLocations::CHitLocation * GetPPhysicsFirstHitLocationIntersectionWithFiniteLine(const C3DVector & p0, const C3DVector & p1, C3DVector & p2, C3DVector & p3, float p4);
 const NHitLocations::CHitLocation * GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector & p0, const C3DVector & p1, C3DVector & p2, C3DVector & p3, float p4) const;
 NHitLocations::CHitLocation * GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector & p0, const C3DVector & p1, C3DVector & p2, C3DVector & p3, float p4);
 const NHitLocations::CHitLocation * GetPCombatFirstHitLocationIntersectionWithTriangle(const std::vector<C3DVector,std::allocator<C3DVector> > & p0, C3DVector & p1, C3DVector & p2, float p3) const;
 NHitLocations::CHitLocation * GetPCombatFirstHitLocationIntersectionWithTriangle(const std::vector<C3DVector,std::allocator<C3DVector> > & p0, C3DVector & p1, C3DVector & p2, float p3);
 const NHitLocations::CHitLocation * GetPCombatFirstHitLocationPosInside(const C3DVector & p0) const;
 NHitLocations::CHitLocation * GetPCombatFirstHitLocationPosInside(const C3DVector & p0);
#ifdef FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
 void SetPDefaultHitLocation(NHitLocations::CHitLocation* location) { PDefaultHitLocation = location; }
#else
 void SetPDefaultHitLocation(NHitLocations::CHitLocation * p0);
#endif
 const NHitLocations::CHitLocation * GetPDefaultHitLocation() const;
#ifdef FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
 NHitLocations::CHitLocation* GetPDefaultHitLocation() { return PDefaultHitLocation; }
#else
 NHitLocations::CHitLocation * GetPDefaultHitLocation();
#endif
 NHitLocations::CHitLocation * GetPHitLocationWithDefName(const CCharString & p0);
 void GetBoundingSphere(C3DVector & p0, float & p1, bool p2) const;
private:
 std::vector<std::pair<NHitLocations::CHitLocation,long>,std::allocator<std::pair<NHitLocations::CHitLocation,long> > > HitLocations;
 NHitLocations::CHitLocation * PDefaultHitLocation;
 CDefPointer<CHitLocationsDef const > PDef;
 mutable C3DVector BoundingSphereOffsetFromPos;
 mutable float BoundingSphereRadius;
 mutable bool CachedBoundingSphere;
public:
 virtual ~CTCHitLocations();
 CTCHitLocations & operator=(const CTCHitLocations & p0);
};
FABLE_STATIC_ASSERT_AT(42, sizeof(CTCHitLocations)==52);
#ifdef FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
inline void NHitLocations::CHitLocation::SetEnabled(bool enabled)
{
    Enabled = enabled;
    if (IsDefaultLocation()) {
        CTCHitLocations* hit_locations;
        POwnerThing->GetTC(&hit_locations);
        if (enabled)
            hit_locations->SetPDefaultHitLocation(this);
        else if (hit_locations->GetPDefaultHitLocation() == this)
            hit_locations->SetPDefaultHitLocation(0);
    }
}
#endif
#else
// New header (chunk 724 round 3, row 0x008771A0). CTCHitLocations : CTCBase (RTTI vtable 0x012434E4). Ego_r TPI size 0x38 with a
// Dinkumware vector; retail STLport vector is 0xC: HitLocations +0x0C, PDefaultHitLocation +0x18, PDef +0x1C,
// BoundingSphereOffsetFromPos +0x20, BoundingSphereRadius +0x2C, CachedBoundingSphere +0x30 (sizeof 0x34).
// Interface type TCI_HIT_LOCATIONS = 127 (PeekTC tests UsingComponents bit 127 = `mov eax,[thing+2Ch] ; test eax,eax ; jns`).
// Methods: Ego_r.pdb spellings. The const GetPCombatFirstHitLocationIntersectionWithFiniteLine is the 5-byte `jmp 0x008168B0`
// body at 0x00816AA0 (the non-const overload; Ego_r 0x0080F330 follows the QAE 0x0080F130 the same way); GetBoundingSphere is
// the QBE reference form at 0x00815420 (`push 1 ; push &radius ; push &centre`).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class C3DVector;
namespace NHitLocations { class CHitLocation; }
class CTCHitLocations : public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCHITLOCATIONS_ORIGINAL_CTOR_PASS23
 // Retail opt-in (Codex pass23): original both-PDB CThing-reference constructor, declaration only.
 CTCHitLocations(CThing&);
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HIT_LOCATIONS; }
    void GetBoundingSphere(C3DVector&, float&, bool) const;          // 0x00815420
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector&, const C3DVector&,
        C3DVector&, C3DVector&, float);                                // 0x008168B0
    const NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector&, const C3DVector&,
        C3DVector&, C3DVector&, float) const;                          // 0x00816AA0
#ifdef FABLE_RETAIL_CTCHITLOCATIONS_FROMNAME
    // Retail opt-in (chunk Q2, row 0x008E0A10): the landed 0x00815820 (the first hit location whose def's instantiation name
    // equals `name`; no PDB carries the name: the established landed spelling, descriptive). Declaration only.
    NHitLocations::CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
#endif
#ifdef FABLE_RETAIL_CTCHITLOCATIONS_WITHDEFNAME
    // Retail opt-in (wf20, row 0x00972990): 0x00815820 under its PDB spelling (Ego_r.pdb 0x0080E4C0 / FableWin.pdb 0x024E6630
    // ?GetPHitLocationWithDefName@CTCHitLocations@@QAEPAVCHitLocation@NHitLocations@@ABVCCharString@@@Z): the first hit location
    // whose def's instantiation name equals the argument. Supersedes the descriptive GetPHitLocationFromName (NAME_FIXES.tsv).
    NHitLocations::CHitLocation* GetPHitLocationWithDefName(const CCharString& defName);   // 0x00815820
#endif
#ifdef FABLE_RETAIL_CTCHITLOCATIONS_POSINSIDE
    // e05 opt-in (egod lane, row 0x006AD700): Ego_r.pdb ?GetPCombatFirstHitLocationPosInside@CTCHitLocations@@QBEPBVCHitLocation@NHitLocations@@ABVC3DVector@@@Z
    // (0x0080EBA0) and the non-const QAE twin (0x0080E550). Retail: the const overload is the 5-byte `jmp 0x008153C0` at 0x00815870
    // (its only caller 0x006AD7D9), 0x008153C0 the non-const body. Declarations only.
    const NHitLocations::CHitLocation* GetPCombatFirstHitLocationPosInside(const C3DVector& pos) const;   // 0x00815870
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationPosInside(const C3DVector& pos);               // 0x008153C0
#endif
#ifdef FABLE_RETAIL_CTCHITLOCATIONS_TRIANGLE
    // Retail opt-in (agentC24, row 0x007BFE30 NThingShot::CTCShotDefinitionBase::UpdateCollision): Ego_r.pdb / FableWin.pdb
    // ?GetPCombatFirstHitLocationIntersectionWithTriangle@CTCHitLocations@@QAEPAVCHitLocation@NHitLocations@@ABV?$vector@VC3DVector@@...
    // (the non-const overload, out of line at 0x008169B0; the row passes a CArray<C3DVector>(3), the two out vectors and 0.25f).
    // Declaration only; parameter names descriptive.
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithTriangle(const std::vector<C3DVector>& triangle,
        C3DVector& hitPos, C3DVector& hitNormal, float tolerance);   // 0x008169B0
#endif
private:
    unsigned char HitLocations[0xC];                                  // +0x0C vector<pair<NHitLocations::CHitLocation,long> >
    NHitLocations::CHitLocation* PDefaultHitLocation;                 // +0x18
    unsigned char PDef[4];                                            // +0x1C CDefPointer<const CHitLocationsDef>
    float BoundingSphereOffsetFromPos[3];                             // +0x20 C3DVector
    float BoundingSphereRadius;                                       // +0x2C
    bool CachedBoundingSphere;                                        // +0x30
};
FABLE_STATIC_ASSERT_AT(82, sizeof(CTCHitLocations) == 0x34);
#endif
#endif
