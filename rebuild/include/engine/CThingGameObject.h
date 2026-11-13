#ifndef FABLE_ENGINE_CTHINGGAMEOBJECT_H
#define FABLE_ENGINE_CTHINGGAMEOBJECT_H
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT
// Retail opt-in (Codex pass 11 consolidation 2): the retail CThingGameObject model (needs the retail CThing chain: FABLE_RETAIL_CCLASS, FABLE_RETAIL_DEF_CLASS_BASE); see below.
#include "engine/CThingPhysical.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
class CHitParameters;
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
namespace NHitLocations{class CHitLocation;}
#endif
#ifdef FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#endif
class CThingGameObject : public CThingPhysical {
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
// Evidence: both original PDB types and donor026fd4f0; native007c1000 accesses.
public:
void GetShotHitParameters(NHitLocations::CHitLocation*,CThing&,const C3DVector&,CHitParameters&)const;
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
// Evidence: both original PDB type records and donor OnHit/helper disassembly.
public:
bool IsGivingHeroStatChangesOnBeingHit()const;bool IsAddToComboMultiplierOnHit()const;
#endif
#ifdef FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
public:
 virtual bool DrawIsTargeted() const;
 virtual bool IsUsingTriggerRadius() const;
 virtual float GetTriggerRadius() const;
 virtual void SetTriggerRadius(float p0);
 virtual bool IsCover() const;
 virtual void OnHit(CHitParameters & p0);
 virtual bool IsGameObject() const;
 virtual bool IsIndestructible() const;
 virtual void ForceAnim(const CCharString & p0, bool p1);
 virtual void ForceAnim(long p0, bool p1);
 virtual long GetMaterialOnHitByMelee() const;
#endif

protected:
    CIntelligentPointer<CThing> PThingStandingOn;
private:
    unsigned char AddToComboMultiplierOnHit:1,AddToComboMultiplierOnHitOverrideSet:1,
        GiveHeroStatChangesOnBeingHit:1,GiveHeroStatChangesOnBeingHitOverrideSet:1;
public:
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_WEAPONTRAIL
    // Row 0x00839100: Ego_r.pdb / FableWin.pdb ?GetWeaponTrailGraphicIndex@CThingGameObject@@QBEJXZ. Declaration only.
    long GetWeaponTrailGraphicIndex() const;                             // 0x00839100
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_CTOR
    // Retail opt-in (chunk 1021, row 0x0083D460 CThingMarker::Create): FableWin.pdb ??0CThingGameObject@@QAE@K@Z (0x01D3EBD0; no
    // Ego_r.pdb or Fable.exe copy: inline, retail calls CThingPhysical(type) 0x005296B0, stores the CThingGameObject vtable 0x01244F7C
    // and default-constructs PThingStandingOn, 0x0083D481..0x0083D499). Body only: no layout change.
    CThingGameObject(unsigned long type) : CThingPhysical(type) { }
#endif
#ifndef FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
    virtual void OnHit(CHitParameters&);
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONCREATE
    // Retail opt-in (agentNF9, row 0x0082D6F0 CThingBuilding::OnCreate, which calls it by name): Ego_r.pdb / FableWin.pdb
    // ?OnCreate@CThingGameObject@@MAE_NXZ (protected virtual, CThingGameObject vtable 0x01244F7C slot 8 = 0x00838A40). Declaration only.
protected:
    virtual bool OnCreate();                                             // 0x00838A40
public:
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
    // Chunk 1010 (row 0x008930F0, via the CThingSearchTools GetPFirstThing body): FableWin.pdb ?StatGetTypeFlags@CThingGameObject@@SIKXZ
    // (static fastcall, inline in retail: the landed GetPFirstThing<..,CThingGameObject> 0x008ABDF0 tests the +0x6C flags against 7).
    static unsigned long __fastcall StatGetTypeFlags() { return 7; }
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_COMBOMULTIPLIER
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): Ego_r.pdb ?SetAddToComboMultiplierOnHit@CThingGameObject@@QAEX_N@Z,
    // out of line in retail (0x008389A0, `push 0 ; call`). Declaration only: no layout change.
    void SetAddToComboMultiplierOnHit(bool value);                       // 0x008389A0
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_STANDINGON
    // Nested opt-in (chunk 765, row 0x00529930): FableWin.pdb ?GetThingStandingOn@CThingGameObject@@UBEPBVCThing@@XZ (0x01D3E690) and
    // ?GetThingStandingOn@CThingGameObject@@UAEPAVCThing@@XZ (0x01D3E6B0, also Ego_r 0x00532720), overrides of CThing's pair (FableWin
    // 0x01CEE4F0 const / 0x01CEE500). Retail vtable 0x01244F7C: slot 39 = 0x00529930 (const, emitted first as in FableWin), slot 38 =
    // 0x00529940 (VC7.1 lays overloads out in reverse declaration order); both forward PThingStandingOn +0xC0 to GetPItem. The engine
    // CThing model declares no virtuals, so these are declarations only (no vtable is emitted from them here).
    virtual const CThing* GetThingStandingOn() const;                    // slot 39 0x00529930
    virtual CThing* GetThingStandingOn();                                // slot 38 0x00529940
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
    // Nested opt-in (chunk 779, row 0x00837E40 CThingTrackNode::ConstructFromLevelScript): Ego_r.pdb
    // ?ConstructFromLevelScript@CThingGameObject@@UAE_NPBD@Z (public virtual override of CThingPhysical's, FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT;
    // retail 0x00838930, CThingGameObject vtable 0x01244F7C slot 4, called by name from the CThingTrackNode / CThingHolySite / CThingObject
    // overrides) and ?ConstructFromParams@CThingGameObject@@IAE_NJABVC3DVector@@JKABVCCharString@@PAVCThingPhysical@@2_N@Z (protected,
    // retail 0x008388D0, bound by the landed CThingTrackNode::ConstructFromParams 0x00836C50). Declarations only: no layout change.
public:
    virtual bool ConstructFromLevelScript(const char* script);   // 0x00838930
protected:
    bool ConstructFromParams(long defIndex,const C3DVector& position,long a3,unsigned long a4,const CCharString& scriptName,
                             CThingPhysical* creator,CThingPhysical* originalCreator,bool a8);   // 0x008388D0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool DrawIsTargeted() const;  // slot 60, 0x00838990
public:
    virtual bool IsUsingTriggerRadius() const;  // slot 61, 0x00529830
public:
    virtual void SetTriggerRadius(float);  // slot 63, 0x00529850
public:
    virtual bool IsCover() const;  // slot 64, 0x00529860
public:
    virtual bool IsGameObject() const;  // slot 66, 0x00529870
public:
    virtual bool IsIndestructible() const;  // slot 67, 0x00529880
public:
    virtual void ForceAnim(long, bool);  // slot 68, 0x005298A0
public:
    virtual void ForceAnim(CCharString const &, bool);  // slot 69, 0x00529890
public:
    virtual long GetMaterialOnHitByMelee() const;  // slot 70, 0x005298B0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_VSLOTS2
// agentFAM04 (2026-11-11): retail CThingGameObject virtuals (TLC vtable slots aligned with the Xbox 360 Test PDB order;
// Ego_r.pdb / FableWin.pdb spellings). Declaration only (no layout change).
public:
    virtual void CopyGameState(const CThing&);                           // slot 7, 0x00838940
    virtual float GetTriggerRadius() const;                              // slot 62, 0x00529840
public:
#endif // FABLE_RETAIL_CTHINGGAMEOBJECT_VSLOTS2
};
FABLE_STATIC_ASSERT(sizeof(CThingGameObject)==0xd0);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingGameObject = 0xe8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingGameObject = 0xd0.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CThingGameObject {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char _pad_0x10[0x1];                            // +0x10
    unsigned char _pad_0x11[0x7];                            // +0x11
    unsigned char _pad_0x18[0x4];                            // +0x18
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char _pad_0x20[0x38];                           // +0x20
    unsigned char _pad_0x58[0x4];                            // +0x58
    unsigned char _pad_0x5c[0x4];                            // +0x5c
    unsigned char _pad_0x60[0x4];                            // +0x60
    unsigned char _pad_0x64[0x4];                            // +0x64
    unsigned char _pad_0x68[0x4];                            // +0x68
    unsigned char _pad_0x6c[0x4];                            // +0x6c
    unsigned char _pad_0x70[0x4];                            // +0x70
    unsigned char _pad_0x74[0x4];                            // +0x74
    unsigned char _pad_0x78[0x4];                            // +0x78
    unsigned char _pad_0x7c[0x4];                            // +0x7c
    unsigned char _pad_0x80[0x4];                            // +0x80
    unsigned char _pad_0x84[0x4];                            // +0x84
    unsigned char _pad_0x88[0x4];                            // +0x88
    unsigned char _pad_0x8c[0x4];                            // +0x8c
    unsigned char _pad_0x90[0x4];                            // +0x90
    unsigned char _pad_0x94[0x1];                            // +0x94
    unsigned char _pad_0x95[0x7];                            // +0x95
    unsigned char _pad_0x9c[0x4];                            // +0x9c
    unsigned char _pad_0xa0[0x4];                            // +0xa0
    unsigned char _pad_0xa4[0x4];                            // +0xa4
    unsigned char _pad_0xa8[0x4];                            // +0xa8
    unsigned char _pad_0xac[0x4];                            // +0xac
    unsigned char _pad_0xb0[0x1];                            // +0xb0
    unsigned char _pad_0xb1[0x3];                            // +0xb1
    unsigned char _pad_0xb4[0x4];                            // +0xb4
    unsigned char _pad_0xb8[0x4];                            // +0xb8
    unsigned char _pad_0xbc[0x1];                            // +0xbc
    unsigned char _pad_0xbd[0x3];                            // +0xbd
    unsigned char PThingStandingOn[0x8];                     // +0xc0 CIntelligentPointer<CThing>
    unsigned char GiveHeroStatChangesOnBeingHitOverrideSet[0x8]; // +0xc8 bitfield group: uchar:1
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingGameObject) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, PThingStandingOn) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CThingGameObject, GiveHeroStatChangesOnBeingHitOverrideSet) == 0xc8);

#endif // FABLE_RETAIL_CTHINGGAMEOBJECT
#endif // FABLE_ENGINE_CTHINGGAMEOBJECT_H
