#ifndef FABLE_ENGINE_NHITLOCATIONS_CHITLOCATION_H
#define FABLE_ENGINE_NHITLOCATIONS_CHITLOCATION_H
#ifdef FABLE_RETAIL_HITLOCATIONS_BOUND
#include "engine/EActionRegisteredType.h"
#include "engine/CHitLocationDef.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_map.h"
class CArmourDef;class CHitParameters;
namespace NHitLocations{class CHitLocation {
public:
#ifndef FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
 CHitLocation(const NHitLocations::CHitLocation & p0);
#endif
#ifdef FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
 CHitLocation(CThing& owner, const CDefPointer<const CHitLocationDef>& def,
              const CDefPointer<const CArmourDef>& armour)
     : POwnerThing(&owner), PDef(def), PArmourDef(armour), HitResponseAction(ACTION_NULL),
       BlockResponseAction(ACTION_NULL), Enabled(true) {}
#else
 CHitLocation(CThing & p0, const CDefPointer<CHitLocationDef const > & p1, const CDefPointer<CArmourDef const > & p2);
#endif
#ifdef FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
 void Init() { SetEnabled(PDef->EnableOnCreate); }
#else
 void Init();
#endif
 void ProcessHitLocationCode(CThing & p0, const CHitParameters & p1);
 bool GetFirstIntersectionWithFiniteLine(const C3DVector & p0, const C3DVector & p1, C3DVector & p2, C3DVector & p3, float p4) const;
 bool GetFirstIntersectionWithTriangle(const std::vector<C3DVector,std::allocator<C3DVector> > & p0, C3DVector & p1, C3DVector & p2, float p3) const;
 bool IsPosInside(const C3DVector & p0) const;
 C3DVector GetClosestPointToPos2D(const C3DVector & p0, C3DVector & p1) const;
 void GetBoundingSphere(C3DVector& centre,float& radius)const{PDef->PhysicalPrimitive.PPrimitive->GetBoundingSphere(*POwnerThing,centre,radius);}
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
 const CDefPointer<CArmourDef const > & GetArmourDef()const{return PArmourDef;}
#else
 const CDefPointer<CArmourDef const > & GetArmourDef() const;
#endif
 void SetArmourDef(const CDefPointer<CArmourDef const > & p0);
 void ResetDefaultArmourDef();
#ifdef FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
 bool IsDefaultLocation() const { return PDef->Default; }
#else
 bool IsDefaultLocation() const;
#endif
 CDefString GetHitLocationDefName() const;
 bool IsToPlayGetHitResponse() const;
 bool IsToCauseDecapitionOnHit() const;
 EActionRegisteredType GetHitResponseActionType() const;
 EActionRegisteredType GetHitKnockdownActionType() const;
 EActionRegisteredType GetHitKnockdownDeathActionType() const;
 EActionRegisteredType GetHitResponseToEpicSpellsActionType() const;
 bool IsCombatCollision() const;
 bool IsPhysicsCollision() const;
 bool IsDefendableAgainstShots() const;
 bool IsDefendableAgainstMelee() const;
 bool IsEnabled() const;
 void SetEnabled(bool p0);
 static void FABLE_FASTCALL StatRegisterClass();
private:
 CThing * POwnerThing;
 CDefPointer<CHitLocationDef const > PDef;
 CDefPointer<CArmourDef const > PArmourDef;
 EActionRegisteredType HitResponseAction;
 EActionRegisteredType BlockResponseAction;
 bool Enabled;
 static std::map<enum EHitLocationCodelet,void (__fastcall*)(CThing &,CHitParameters const &),std::less<enum EHitLocationCodelet>,std::allocator<std::pair<enum EHitLocationCodelet const ,void (__fastcall*)(CThing &,CHitParameters const &)> > > RegisteredHitLocationCodelets;
#ifdef FABLE_RETAIL_CHITLOCATION_TREGISTEREDHITLOCATIONCODELETMAP
 // Opt-in (agentNF69, rows 0x008162F0 StatRegisterClass / 0x00815E80 ProcessHitLocationCode): the nested typedef of the static above,
 // Ego_d.pdb LF_NESTTYPE NHitLocations::CHitLocation::TRegisteredHitLocationCodeletMap (docs/journal/2026-11/EGOD_FINDINGS.md section 1).
 typedef std::map<enum EHitLocationCodelet,void (__fastcall*)(CThing &,CHitParameters const &)> TRegisteredHitLocationCodeletMap;
#endif
public:
#ifdef FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
 ~CHitLocation() {}
#else
 ~CHitLocation();
#endif
 NHitLocations::CHitLocation & operator=(const NHitLocations::CHitLocation & p0);
};}
FABLE_STATIC_ASSERT_AT(54, sizeof(NHitLocations::CHitLocation)==24);
#else
#include "engine/EActionRegisteredType.h"
class CThing;
// Original TPI0x25311; native815290/2a0/2b0 access PDef and enum fields.
namespace NHitLocations {
class CHitLocation {
    CThing* POwnerThing;
    unsigned char PDef[4],PArmourDef[4];
    EActionRegisteredType HitResponseAction,BlockResponseAction;
    bool Enabled;
public:
    EActionRegisteredType GetHitResponseActionType() const;
    EActionRegisteredType GetHitKnockdownActionType() const;
    EActionRegisteredType GetHitKnockdownDeathActionType() const;
#ifdef FABLE_RETAIL_CHITLOCATION_SETENABLED
    // Retail opt-in (chunk Q2, row 0x008E0A10): Ego_r.pdb ?SetEnabled@CHitLocation@NHitLocations@@QAEX_N@Z (0x00816390). Declaration only.
    void SetEnabled(bool enabled);                                       // 0x00816390
#endif
#ifdef FABLE_RETAIL_CHITLOCATION_STATREGISTERCLASS
    // Retail opt-in (row 0x004EE23F CTCBase::RegisterTCs): Ego_r.pdb / FableWin.pdb ?StatRegisterClass@CHitLocation@NHitLocations@@SIXXZ
    // (public static __fastcall), retail 0x008162F0 (between the NHitLocations primitives and CHitLocation::SetEnabled 0x00816390, as Ego_r
    // 0x0080ED20 follows CTCTargetingPlayer::StatRegisterClass); RegisterTCs calls it with the other classes' static setup. Declaration only.
    static void __fastcall StatRegisterClass();                          // 0x008162F0
#endif
};
}
FABLE_STATIC_ASSERT_AT(81, sizeof(NHitLocations::CHitLocation)==0x18);
#endif
#endif
