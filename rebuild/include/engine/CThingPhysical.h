#ifndef FABLE_ENGINE_CTHINGPHYSICAL_H
#define FABLE_ENGINE_CTHINGPHYSICAL_H
#ifdef FABLE_RETAIL_CTHINGPHYSICAL
// Retail opt-in (Codex pass 11 consolidation 2): the retail CThingPhysical model (needs the retail CThing chain: FABLE_RETAIL_CCLASS, FABLE_RETAIL_DEF_CLASS_BASE); see below.
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
class C2DLineF;
#ifdef FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
class CMaterialDef;
#endif
class CThingPhysical : public CThing {
#ifdef FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
public:
 virtual bool IsSolidToThing(const CThingPhysical * p0) const;
 virtual bool IsAffectedByDamage() const;
 virtual unsigned long GetProperties() const;
 virtual bool IsCrop() const;
 virtual bool IsHidden() const;
 virtual bool IsTargetedByPlayer(long p0) const;
 virtual void SetTargetedByPlayer(long p0);
 virtual void ClearTargetedByPlayer(long p0);
 virtual bool IsFragile() const;
 virtual bool CanBlockNavigatingCreature() const;
 virtual bool CanBlockUpperLevelNavigatingCreature() const;
 virtual bool UseHighDetailQuadTree() const;
 virtual CDefPointer<CMaterialDef const > PeekPMaterial() const;
 virtual void ModifyHealth(float p0, bool p1);
#endif

#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

private:
    unsigned char HealthOverTimeEffects[4];
protected:
    float Scale;
    CIntelligentPointer<CThingPhysical> PLastCreator;
    CIntelligentPointer<CThingPhysical> POriginalCreator;
    float MaxHealth;
    float Health;
    long CanComeBetweenCameraAndObservedThing;
    unsigned char Damageable:1, Killable:1, RespondToHits:1, EvadeAllHits:1,
        NegateAllHits:1, AbleToBeEngagedInCombat:1;
public:
#if defined(FABLE_RETAIL_CTHINGPHYSICAL_CTOR) || defined(FABLE_RETAIL_CTHINGGAMEOBJECT_CTOR)
    // Retail opt-in (chunk 1021, row 0x0083D460 CThingMarker::Create): Ego_r.pdb ??0CThingPhysical@@QAE@K@Z (0x005325E0 = retail
    // 0x005296B0), out of line (`push 0Bh ; mov ecx,esi ; call 0x005296B0`, 0x0083D47D). Declaration only: no layout change.
    CThingPhysical(unsigned long type);                                  // 0x005296B0
#endif
    const C3DVector& PeekPos() const;
    float GetHealth() const { return Health; }
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
    float GetMaxHealth()const{return MaxHealth;}
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
    // Original PDB method and native006a4e40, called from008c7280.
    float GetHealthPercentage() const;
#endif
    bool IsToRespondToHits() const { return RespondToHits; }
    bool IsKillable() const { return Killable; }
    bool IsDamageable() const { return Damageable; }
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
// Evidence: both original PDB type records and donor OnHit/helper disassembly.
public:
bool IsToNegateAllHits()const{return NegateAllHits;}
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_EVADEALLHITS
    // Nested opt-in (chunk 1016, row 0x0085FA00 NCreatureMode::IsToEvadeAllHits): FableWin.pdb ?IsToEvadeAllHits@CThingPhysical@@QBE_NXZ,
    // inline in retail (`test byte ptr [thing+0BCh],8`, bit 3 of the +0xBC flag run). Body only: no layout change.
    bool IsToEvadeAllHits() const { return EvadeAllHits; }
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_HITSETTERS
    // Nested opt-in (chunk 802, row 0x00974AA0): FableWin.pdb ?SetAsDamageable@CThingPhysical@@QAEX_N@Z and
    // ?SetAsRespondToHits@CThingPhysical@@QAEX_N@Z, inline in retail (`and byte ptr [thing+0BCh],0FEh` / `0FBh`). Bodies only.
    void SetAsDamageable(bool value) { Damageable = value; }
    void SetAsRespondToHits(bool value) { RespondToHits = value; }
#endif
    // Chunk 724 round 3 (row 0x008771A0): Ego_r.pdb ?IsThingAbleToComeBetweenCameraAndObservedThing@CThingPhysical@@QBE_NXZ,
    // out of line at retail 0x006A5AB0 (catalog name; `mov ecx,edi ; call` in CTools::GetThingPointIntersectionFromTo).
    bool IsThingAbleToComeBetweenCameraAndObservedThing() const;   // 0x006A5AB0
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_FIRSTCOLLISION
    // Retail opt-in (chunk Q8, row 0x006A4FB0): Ego_r.pdb ?GetFirstCollision@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z (public
    // const), out of line at retail 0x006A4FB0 (called at 0x006A5469). Declaration only: no layout change.
    bool GetFirstCollision(float radius, const C3DVector& from, const C3DVector& delta, C3DVector& hit_position,
                           C3DVector& hit_normal) const;                // 0x006A4FB0
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_SERIALISE
    // Retail opt-in (agentNF9, row 0x007030C0 CThingObject::SerialiseGameState, which calls it by name): Ego_r.pdb / FableWin.pdb
    // ?SerialiseGameState@CThingPhysical@@UAE_NAAVCPersistContext@@@Z (public virtual, CThingPhysical vtable 0x01244E84 slot 3 =
    // 0x006A5AF0). Declaration only (CPersistContext forward-declared).
    virtual bool SerialiseGameState(class CPersistContext& context);   // 0x006A5AF0
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
    // Nested opt-in (chunk 771, row 0x006A5D90): Ego_r.pdb ?ConstructFromLevelScript@CThingPhysical@@UAE_NPBD@Z (public virtual
    // override of CThing's, FABLE_RETAIL_CTHING_LEVELSCRIPT; vtable entry 0x00E44E94 file offset). A declaration only.
    virtual bool ConstructFromLevelScript(const char* script);   // 0x006A5D90
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool IsAffectedByDamage() const;  // slot 47, 0x00529730
public:
    virtual unsigned long GetProperties() const;  // slot 48, 0x00529740
public:
    virtual bool IsCrop() const;  // slot 49, 0x00529750
public:
    virtual bool IsHidden() const;  // slot 50, 0x00529760
public:
    virtual bool IsTargetedByPlayer(long) const;  // slot 51, 0x00529770
public:
    virtual void SetTargetedByPlayer(long);  // slot 52, 0x00529780
public:
    virtual void ClearTargetedByPlayer(long);  // slot 53, 0x00529790
public:
    virtual bool CanBlockUpperLevelNavigatingCreature() const;  // slot 56, 0x005297B0
public:
    virtual bool UseHighDetailQuadTree() const;  // slot 57, 0x005297C0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGPHYSICAL_VSLOTS2
// agentFAM04 (2026-11-11): retail CThingPhysical overrides of CThing virtuals (TLC vtable slots aligned with the Xbox 360
// Test PDB order; Ego_r.pdb / FableWin.pdb spellings). Declaration only (no layout change). Needs CThingPhysicalDebugInfo declared.
public:
    virtual void CopyGameState(const CThing&);                           // slot 7, 0x006A4B70
    virtual bool DrawGetPhysicsDebugInfo(class CThingPhysicalDebugInfo&) const;   // slot 35, 0x006A4E70
    virtual bool CanBlockNavigatingCreature() const;                     // slot 55, 0x006A4D90
public:
#endif // FABLE_RETAIL_CTHINGPHYSICAL_VSLOTS2
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysical)==0xc0);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingPhysical = 0xd8.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingPhysical = 0xc0.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CThingPhysical {
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
    long          f64;                                       // +0x64 retail-only long (previous header)
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
    unsigned char _pad_0x95[0x3];                            // +0x95
    unsigned char HealthOverTimeEffects[0x4];                // +0x98 list<CThingPhysical::CHealthOverTimeHelper,std::allocator<CThingPhysical::CHealthOverTimeHelper>_>
    float         Scale;                                     // +0x9c
    unsigned char PLastCreator[0x8];                         // +0xa0 CIntelligentPointer<CThingPhysical>
    unsigned char POriginalCreator[0x8];                     // +0xa8 CIntelligentPointer<CThingPhysical>
    float         MaxHealth;                                 // +0xb0
    float         Health;                                    // +0xb4
    long          CanComeBetweenCameraAndObservedThing;      // +0xb8
    unsigned char AbleToBeEngagedInCombat[0x4];              // +0xbc bitfield group: uchar:1
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingPhysical) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, f64) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, HealthOverTimeEffects) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, Scale) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, PLastCreator) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, POriginalCreator) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, MaxHealth) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, Health) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, CanComeBetweenCameraAndObservedThing) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CThingPhysical, AbleToBeEngagedInCombat) == 0xbc);

#endif // FABLE_RETAIL_CTHINGPHYSICAL
#endif // FABLE_ENGINE_CTHINGPHYSICAL_H
