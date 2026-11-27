#ifndef FABLE_ENGINE_CTCPERCEIVETHING_H
#define FABLE_ENGINE_CTCPERCEIVETHING_H
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/C3DVector.h"
class CPerceivedThingDef;
class CTCPerceiveThing: public CTCBase {
public:
#if defined(FABLE_RETAIL_JACK_STAGE1_UPDATE) || defined(FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING)
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_PERCEIVE_THING;}
#else
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
#endif
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCPerceiveThing(const CTCPerceiveThing & p0);
 CTCPerceiveThing(CThing & p0);
 virtual void OnCreate();
 virtual void FrameUpdate();
 virtual void BulletTimeFrameUpdate();
 virtual bool HasFrameUpdate() const;
 virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0);
 virtual void OnKill();
 virtual void OnLevelUnload();
 virtual void OnSetInLimbo(bool p0);
 bool IsThingLineOfSight() const { return LineOfSight; }
 bool IsThingExtendedLineOfSight() const;
 bool IsThingIgnoreFOVLineOfSight() const;
#ifdef FABLE_RETAIL_SEEK_ENEMY_UPDATE_PASS29
 bool IsThingSmelt() const { return Smelt; }
#else
 bool IsThingSmelt() const;
#endif
 bool IsThingHeard() const;
#ifdef FABLE_RETAIL_SEEK_ENEMY_UPDATE_PASS29
 bool IsThingPerceived() const { return IsThingLineOfSight() || IsThingSmelt(); }
#else
 bool IsThingPerceived() const;
#endif
 bool IsPerceiving() const;
 bool IsPlayerLineOfSight() const;
 bool IsPlayerExtendedLineOfSight() const;
 bool IsPlayerIgnoreFOVLineOfSight() const;
#ifdef FABLE_RETAIL_HEARING
 bool IsAwareOfThing()const{return AwarenessTimer!=0.0f;}
 bool IsAwarenessOfThingDirect()const{return DirectAwareness;}
#elif defined(FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING)
 // Opt-in (chunk Q16, row 0x00957DB0 CAIStateGroup_MinionBehaviourCrowdBase::Update): IsAwareOfThing expanded in retail (`fld [+0x18];
 // fucompp` against 0.0f at 0x00957E30) while IsAwarenessOfThingDirect is called out of line (0x00744430 `mov al,[ecx+30h]; ret`,
 // Ego_r.pdb ?IsAwarenessOfThingDirect@CTCPerceiveThing@@QBE_NXZ).
 bool IsAwareOfThing()const{return AwarenessTimer!=0.0f;}
 bool IsAwarenessOfThingDirect() const;
#else
 bool IsAwareOfThing() const;
 bool IsAwarenessOfThingDirect() const;
#endif
 bool IsDrawingPerceptionIndicator() const;
 long GetFramesSinceDirectlyPerceivedThing() const;
 void SetThingLineOfSight(bool p0);
 void SetThingExtendedLineOfSight(bool p0);
 void SetThingIgnoreFOVLineOfSight(bool p0);
 void UpdateHeroPerception(bool p0);
#if defined(FABLE_RETAIL_SEEK_ENEMY_UPDATE_PASS29) || defined(FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29)
 const CThing* GetPerceivedThing() const;
#else
 const CThing * GetPerceivedThing() const { return PerceivedThing.Get(); }
#endif
 void SetPerceivedThing(const CThing * p0);
#ifdef FABLE_RETAIL_SEEK_ENEMY_UPDATE_PASS29
 const C3DVector& GetAwarenessPosition() const { return LastPerceivedPos; }
#else
 const C3DVector & GetAwarenessPosition() const;
#endif
 const C3DVector & GetLastPerceivedPos() const;
 void SetThingToPerceive(const CThing * p0);
 void SetAsDirectlyAwareOfPerceivedThing() { if(GetPerceivedThing()) { DirectAwareness=true; FramesSinceDirectlyPerceivedThing=0; SetLastAwareInfo(); } }
 void SetAsIndirectlyAwareOfPerceivedThing();
#ifdef FABLE_RETAIL_HEARING
 float GetSoundRadius()const{return (SoundRadius);}
#else
 float GetSoundRadius() const;
#endif
 float GetSmellRadius() const { return SmellRadius; }
 float GetSightRadius() const;
 float GetExtendedSightRadius() const;
 float GetGiveUpChaseRadius() const;
 void SetSoundRadius(float p0);
 void SetSmellRadius(float p0);
 void SetSightRadius(float p0);
 void SetExtendedSightRadius(float p0);
 void SetGiveUpChaseRadius(float p0);
 float GetFOV() const;
 void ResetPerceptionVariables();
 void OnHitByMelee(const CThing & p0);
 void OnHitByShot(const CThing & p0);
 void OnHit(const CThing & p0);
private:
 bool IsUpdatingThisFrame() const;
 bool IsSearchingForThingToPerceive() const;
 void SetLastAwareInfo();
 void UpdateDistanceFromPerceivedThing();
 void UpdateLineOfSight();
 void UpdateSmell();
 void UpdateHearing();
 void UpdatePerception();
 void ReactToAlerts();
 float GetTimeBasedVisibilityMultiplier() const;
 CIntelligentPointer<const CThing> PerceivedThing;
 float DistanceFromPerceivedThingSquared;
 float AwarenessTimer;
 long FramesSinceDirectlyPerceivedThing;
 C3DVector LastPerceivedPos;
 bool Perceiving;
 bool LineOfSight;
 bool ExtendedLineOfSight;
 bool IgnoreFOVLineOfSight;
 bool DirectAwareness;
 bool Smelt;
 bool Heard;
 bool SearchingForThingToPerceive;
 CDefPointer<const CPerceivedThingDef> PDef;
 long UpdateOffset;
 float SoundRadius;
 float SightRadius;
 float ExtendedSightRadius;
 float SmellRadius;
 float GiveUpChaseRadius;
 bool PerceptionCombat;
 long FrameHitByPerceivedThing;
public:
 virtual ~CTCPerceiveThing();
 CTCPerceiveThing & operator=(const CTCPerceiveThing & p0);
};
FABLE_STATIC_ASSERT_AT(123, sizeof(CTCPerceiveThing)==0x58);
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_TIMEVISIBILITY_INLINE
// Nested opt-in (NF39, row 0x007448F0 GetSightRadius): the body of the private Ego_r.pdb / FableWin.pdb
// ?GetTimeBasedVisibilityMultiplier@CTCPerceiveThing@@ABEMXZ (out-of-line copy 0x00744580, landed), inline where retail expands it
// (GetSightRadius 0x007448F0 / GetExtendedSightRadius 0x00744950: the multiplier first, then `fmul` by the radius; each arm returns on its
// own). The def's day-time / night-time multiplier, their average at dusk and dawn. Needs FABLE_RETAIL_HEARING (CPerceivedThingDef),
// FABLE_RETAIL_CWORLD, FABLE_RETAIL_CGAMETIMEMANAGER_FULL (IsDay / IsNightTime) and FABLE_RETAIL_DEF_CLASS_BASE.
#include "engine/CPerceivedThingDef.h"
#include "engine/CGameTimeManager.h"
#include "engine/CWorld.h"
inline float CTCPerceiveThing::GetTimeBasedVisibilityMultiplier() const
{
    if (PeekWorld().PeekGameTimeManager().IsDay())
        return PDef->DayTimeVisibilityMultiplier;
    if (PeekWorld().PeekGameTimeManager().IsNightTime())
        return PDef->NightTimeVisibilityMultiplier;
    return (PDef->NightTimeVisibilityMultiplier + PDef->DayTimeVisibilityMultiplier) * 0.5f;
}
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_GETFOV_INLINE
// NF59 opt-in (queue q10523, row 0x00745120 UpdateLineOfSight): the body of Ego_r.pdb 0x00757070 / FableWin.pdb 0x0258DA60
// ?GetFOV@CTCPerceiveThing@@QBEMXZ (out-of-line copy 0x007445D0, landed), inline where retail expands it (0x00745120 multiplies the def's
// FieldOfView by 1/360 in place). The landed UpdateLineOfSight row defined this body itself, a second strong definition of the symbol
// 0x007445D0 defines. Needs FABLE_RETAIL_HEARING (CPerceivedThingDef) and FABLE_RETAIL_DEF_CLASS_BASE.
#include "engine/CPerceivedThingDef.h"
inline float CTCPerceiveThing::GetFOV() const
{
    return PDef->FieldOfView / 360.0f;
}
#endif
#else
#ifdef FABLE_RETAIL_CTCPERCEIVETHING
// Retail opt-in (chunk 765, row 0x007446A0 CTCPerceiveThing::GetPerceivedThing): the polymorphic class, RTTI .?AVCTCPerceiveThing@@,
// single base CTCBase at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv) up to the one the row reads:
// PerceivedThing (CIntelligentPointer<const CThing>) +0x0C, DistanceFromPerceivedThingSquared +0x14. Name set:
// ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ. No virtual overrides and no later members are modelled (no sizeof assertion).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
class CPerceivedThingDef;
#endif
class CTCPerceiveThing : public CTCBase {
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:
static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_PERCEIVE_THING;}
void OnHit(const CThing&);
#endif
public:
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual void OnCreate();
    virtual void FrameUpdate();
    virtual void BulletTimeFrameUpdate();
    virtual bool HasFrameUpdate() const;
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void OnKill();
    virtual void OnLevelUnload();
    virtual void OnSetInLimbo(bool);
    virtual ~CTCPerceiveThing();
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
    // Nested opt-in (chunk 2004, row 0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable): ?IsAwareOfThing@CTCPerceiveThing@@QBE_NXZ
    // called out of line (0x00643B3B, the shared COMDAT `fld 0.0 ; fld [ecx+18h] ; fucompp`; 15 retail call sites, e.g. 0x00960DF1,
    // 0x00864783, 0x0091DC00), and the inline Ego_r / FableWin ?IsThingIgnoreFOVLineOfSight@CTCPerceiveThing@@QBE_NXZ (read in place,
    // `mov al,[edi+2Fh]`, 0x00960DFE), ?StatGetInterfaceType@CTCPerceiveThing@@SI?AW4ETCInterfaceType@@XZ (inline, 0x34) and the out-of-line
    // ?GetExtendedSightRadius@CTCPerceiveThing@@QBEMXZ (0x00744950: PDef +0x48 / +0x4C times ExtendedSightRadius +0x44; GetSightRadius
    // 0x007448F0 is the +0x40 twin before it, in Ego_r order 0x007573B0 / 0x00757430). Declarations and accessors only, no layout change.
    bool IsAwareOfThing() const;                                         // 0x00643B3B
    bool IsThingIgnoreFOVLineOfSight() const { return IgnoreFOVLineOfSight; }
#ifndef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_PERCEIVE_THING; }
    float GetExtendedSightRadius() const;                                // 0x00744950
#endif
#else
#ifdef FABLE_RETAIL_OPINIONREACTIONROOT_ISRUNNABLE_PASS27
    // Pass 27 (row 0x0095EF80): declared here, defined inline after the row's `#pragma optimize("s", on)`; every other includer keeps the
    // in-class body (landing 2026-11-22: the unconditional declaration broke 0x0091B3A0 / 0x009578A0).
    bool IsAwareOfThing() const;
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_PERCEIVE_THING; }
#else
    bool IsAwareOfThing() const { return AwarenessTimer!=0.0f; }
#endif
#endif
    bool IsThingLineOfSight() const { return LineOfSight; }
#ifdef FABLE_RETAIL_SENSED_PASS23
    // Both original PDB constructor types: CThing& parent, ordinary public thiscall.
    CTCPerceiveThing(CThing&);
    // Original PDB methods; donor parent 02A90CA0 calls the named accessors.
    // Retail 0089C9BB/C8/D5 read the same members at 2E/31/32.
    bool IsThingExtendedLineOfSight() const { return ExtendedLineOfSight; }
#ifndef FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS
    bool IsThingSmelt() const { return Smelt; }
    bool IsThingHeard() const { return Heard; }
    float GetSightRadius() const; // 007448F0, PDB order and native member +40.
#endif
    float GetSoundRadius() const; // 00744440, native member +3C.
    float GetSmellRadius() const; // 00744450, native member +48.
    float GetFOV() const; // 007445D0, confirmed by donor call order.
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_SEEKPERCEIVEDTHING_FRIEND
    // Nested opt-in (wf8, row 0x0091B3A0 CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition): that function reads the
    // private LastPerceivedPos (+0x20) directly (0x0091B452 `lea edi,[ebp+20h]`). No accessor existed in the source: FableWin.pdb
    // emits this function's other trivial inline accessors out of line (IsThingLineOfSight 0x01DA32C0, IsAwareOfThing 0x01F43D20)
    // and has its own copy of the function (0x02BD3B90), yet neither PDB has any GetLastPerceivedPos symbol; the only PDB accessor
    // of that return type, GetAwarenessPosition (FableWin 0x02B80DF0, 236 bytes), is not a trivial return. The friend grant is the
    // header's way of allowing that direct read of a member this branch keeps private (descriptive: the PDBs carry no friend
    // records). No layout or code change.
    friend class CAIStateGroup_SeekPerceivedThing;
#endif
    bool IsAwarenessOfThingDirect() const;
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_CANSEEORSMELL
    // Overlay opt-in (wfD4, row 0x006D1E30): CTCPerceiveThing::CanSeeOrSmellPerceivedThing() const, out of line in retail at 0x00661600
    // (landed row; reloc_bindings ?CanSeeOrSmellPerceivedThing@CTCPerceiveThing@@QBE_NXZ; called at 0x006D23B6). Declaration only.
    bool CanSeeOrSmellPerceivedThing() const;                            // 0x00661600
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS
    // Nested opt-in (wf20, row 0x006C8E90 CThingFilter_IsEligibleWitness::operator()): FableWin.pdb inline accessors
    // ?IsThingIgnoreFOVLineOfSight / ?IsThingSmelt / ?IsThingHeard@CTCPerceiveThing@@QBE_NXZ (no Ego_r / Fable.exe copy: inlined in
    // place, `mov al,[esi+2Fh]` / `[esi+31h]` / `[esi+32h]`), and ?GetSightRadius@CTCPerceiveThing@@QBEMXZ out of line at 0x007448F0.
    bool IsThingIgnoreFOVLineOfSight() const { return IgnoreFOVLineOfSight; }
    bool IsThingSmelt() const { return Smelt; }
    bool IsThingHeard() const { return Heard; }
    float GetSightRadius() const;                                        // 0x007448F0
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_RADII
    // Nested opt-in (agentNF10, row 0x00744ED0 OnCreate): Ego_r.pdb / FableWin.pdb ?SetSoundRadius / SetSmellRadius / SetSightRadius /
    // SetExtendedSightRadius / SetGiveUpChaseRadius@CTCPerceiveThing@@QAEXM@Z (landed 0x00744470 / 0x007444A0 / 0x00744540 / 0x00744500 /
    // 0x007444D0) and ?ResetPerceptionVariables@CTCPerceiveThing@@QAEXXZ (0x00744DB0). Declarations only: no layout change.
    void SetSoundRadius(float radius);                                   // 0x00744470
    void SetSmellRadius(float radius);                                   // 0x007444A0
    void SetSightRadius(float radius);                                   // 0x00744540
    void SetExtendedSightRadius(float radius);                           // 0x00744500
    void SetGiveUpChaseRadius(float radius);                             // 0x007444D0
    void ResetPerceptionVariables();                                     // 0x00744DB0
#endif
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_PERCEIVE_THING;}
    float GetExtendedSightRadius()const;
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_ALERTNEARBY_PASS24
    void SetAsIndirectlyAwareOfPerceivedThing();                        // 0x007449E0
#endif
    const CThing* GetPerceivedThing() const;                             // 0x007446A0
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_SETASDIRECTLYAWARE
    // NF68 opt-in (row 0x0097A630 CAIStateGroup_CreatureTakeCover::Update): Ego_r.pdb ?SetAsDirectlyAwareOfPerceivedThing@CTCPerceiveThing@@QAEXXZ,
    // out of line in retail (0x007449B0, called at the TakeCover site). Declaration only.
    void SetAsDirectlyAwareOfPerceivedThing();                           // 0x007449B0
#endif
#if defined(FABLE_RETAIL_CTCPERCEIVETHING_GETSIGHTRADIUS) && !(defined(FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS) && defined(FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS))
    // Nested opt-in (2026-11-16, rows 0x009207A0 / 0x00933B30 / 0x00936210, respelled from GetSurfaceRange / GetExtendedSightRadius):
    // Ego_r.pdb / FableWin.pdb ?GetSightRadius@CTCPerceiveThing@@QBEMXZ, out of line at 0x007448F0 (reads SightRadius +0x40; its twin
    // GetExtendedSightRadius is 0x00744950). Declaration only: no layout change.
    float GetSightRadius() const;                                        // 0x007448F0
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS_CALLS
    // Nested opt-in (agentNF24, row 0x00935160): both awareness queries called out of line, as retail calls them there:
    // 0x00643B3B Ego_r/FableWin ?IsAwareOfThing@CTCPerceiveThing@@QBE_NXZ (reloc_bindings first caller 0x00864700) and
    // 0x00744430 ?IsAwarenessOfThingDirect@CTCPerceiveThing@@QBE_NXZ (`mov al,[ecx+30h]; ret`). Declarations only: no layout change.
    bool IsAwareOfThing() const;                                         // 0x00643B3B
    bool IsAwarenessOfThingDirect() const;                               // 0x00744430
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
private:
#else
protected:
#endif
    CIntelligentPointer<const CThing> PerceivedThing;                    // +0x0c
    float DistanceFromPerceivedThingSquared;                             // +0x14
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
    float AwarenessTimer;
    long FramesSinceDirectlyPerceivedThing;
    C3DVector LastPerceivedPos;
    bool Perceiving,LineOfSight,ExtendedLineOfSight,IgnoreFOVLineOfSight;
    bool DirectAwareness,Smelt,Heard,SearchingForThingToPerceive;
    CDefPointer<const CPerceivedThingDef> PDef;
    long UpdateOffset;
    float SoundRadius,SightRadius,ExtendedSightRadius,SmellRadius,GiveUpChaseRadius;
    bool PerceptionCombat;
    long FrameHitByPerceivedThing;
#endif
};
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
FABLE_STATIC_ASSERT_AT(242, sizeof(CTCPerceiveThing)==0x58);
#endif
#else // flattened Ego_r layout (the landed default)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCPerceiveThing = 0x58.
// Retail evidence: 1 trusted landed function(s); reconciliation 3 conflict(s) in 1 file(s) vs 0 agreeing; PINNED by 2 retyped file(s) despite conflicts.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CDefPointeeBase;
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETPERCEIVEDTHING
class CThing;
#endif

#pragma pack(push, 1)
class CTCPerceiveThing {
public:
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETFOV
    // Flat-branch opt-in (agentNF55, row 0x00665260 CThingCreatureBase::IsPosInFOV): the retail branches' GetFOV on the flattened
    // layout. Ego_r.pdb 0x00757070 / FableWin.pdb 0x0258DA60 ?GetFOV@CTCPerceiveThing@@QBEMXZ (retail 0x007445D0, between
    // GetTimeBasedVisibilityMultiplier 0x00744580 and the constructor 0x007445E0 as in Ego_r). Declaration only (no layout change).
    float GetFOV() const;                                    // 0x007445D0
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETPERCEIVEDTHING
    // NF168 flat-branch opt-in (row 0x009577E0 CAIStateGroupBehaviourBase::IsRunnable): the retail branch's out-of-line getter on the
    // flattened layout, Ego_r.pdb / FableWin.pdb ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ (retail 0x007446A0, the landed
    // row's leaf). Declaration only (no layout change).
    const CThing* GetPerceivedThing() const;                 // 0x007446A0
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_FLAT_CANSEEORSMELL
    // NF168 flat-branch opt-in (row 0x0091DB50 CAIStateGroup_EnemyCreatureBase::GetEnemy): CanSeeOrSmellPerceivedThing() const, out of
    // line in retail at 0x00661600 (landed row; the retail-branch FABLE_RETAIL_CTCPERCEIVETHING_CANSEEORSMELL spelling). Declaration only.
    bool CanSeeOrSmellPerceivedThing() const;                // 0x00661600
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_FLAT_ISAWAREOFTHING
    // NF168 flat-branch opt-in (row 0x0091DB50): Ego_r.pdb / FableWin.pdb ?IsAwareOfThing@CTCPerceiveThing@@QBE_NXZ, out of line in
    // retail at 0x00643B3B (landed row). Declaration only.
    bool IsAwareOfThing() const;                             // 0x00643B3B
#endif
#ifdef FABLE_RETAIL_CTCPERCEIVETHING_FLAT_SETASDIRECTLYAWARE
    // NF168 flat-branch opt-in (row 0x00849320 CCreatureAction_AlertToPlayer::PerformAction): Ego_r.pdb / FableWin.pdb
    // ?SetAsDirectlyAwareOfPerceivedThing@CTCPerceiveThing@@QAEXXZ, out of line in retail at 0x007449B0 (landed row). Declaration only.
    void SetAsDirectlyAwareOfPerceivedThing();               // 0x007449B0
#endif
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    int              field4;                                 // +0x04 retail-only int (previous header)
    unsigned char    _pad_0x08[0x4];                         // +0x08
    unsigned char    PerceivedThing[0x8];                    // +0x0c CIntelligentPointer<CThing_const_>
    float            DistanceFromPerceivedThingSquared;      // +0x14
    float            AwarenessTimer;                         // +0x18
    long             FramesSinceDirectlyPerceivedThing;      // +0x1c
    float            LastPerceivedPos_X;                     // +0x20 C3DVector::X
    float            LastPerceivedPos_Y;                     // +0x24 C3DVector::Y
    float            LastPerceivedPos_Z;                     // +0x28 C3DVector::Z
    bool             Perceiving;                             // +0x2c
    bool             LineOfSight;                            // +0x2d
    bool             ExtendedLineOfSight;                    // +0x2e
    bool             IgnoreFOVLineOfSight;                   // +0x2f
    bool             DirectAwareness;                        // +0x30
    bool             Smelt;                                  // +0x31
    bool             Heard;                                  // +0x32
    bool             SearchingForThingToPerceive;            // +0x33
    CDefPointeeBase* PDef_Object;                            // +0x34 CDefPointer<CPerceivedThingDef_const_>::Object
    long             UpdateOffset;                           // +0x38
    float            SoundRadius;                            // +0x3c
    float            SightRadius;                            // +0x40
    float            ExtendedSightRadius;                    // +0x44
    float            SmellRadius;                            // +0x48
    float            GiveUpChaseRadius;                      // +0x4c
    bool             PerceptionCombat;                       // +0x50
    unsigned char    _pad_0x51[0x3];                         // +0x51
    long             FrameHitByPerceivedThing;               // +0x54
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(289, sizeof(CTCPerceiveThing) == 0x58);
FABLE_STATIC_ASSERT_AT(290, offsetof(CTCPerceiveThing, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(291, offsetof(CTCPerceiveThing, field4) == 0x4);
FABLE_STATIC_ASSERT_AT(292, offsetof(CTCPerceiveThing, PerceivedThing) == 0xc);
FABLE_STATIC_ASSERT_AT(293, offsetof(CTCPerceiveThing, DistanceFromPerceivedThingSquared) == 0x14);
FABLE_STATIC_ASSERT_AT(294, offsetof(CTCPerceiveThing, AwarenessTimer) == 0x18);
FABLE_STATIC_ASSERT_AT(295, offsetof(CTCPerceiveThing, FramesSinceDirectlyPerceivedThing) == 0x1c);
FABLE_STATIC_ASSERT_AT(296, offsetof(CTCPerceiveThing, LastPerceivedPos_X) == 0x20);
FABLE_STATIC_ASSERT_AT(297, offsetof(CTCPerceiveThing, LastPerceivedPos_Y) == 0x24);
FABLE_STATIC_ASSERT_AT(298, offsetof(CTCPerceiveThing, LastPerceivedPos_Z) == 0x28);
FABLE_STATIC_ASSERT_AT(299, offsetof(CTCPerceiveThing, Perceiving) == 0x2c);
FABLE_STATIC_ASSERT_AT(300, offsetof(CTCPerceiveThing, LineOfSight) == 0x2d);
FABLE_STATIC_ASSERT_AT(301, offsetof(CTCPerceiveThing, ExtendedLineOfSight) == 0x2e);
FABLE_STATIC_ASSERT_AT(302, offsetof(CTCPerceiveThing, IgnoreFOVLineOfSight) == 0x2f);
FABLE_STATIC_ASSERT_AT(303, offsetof(CTCPerceiveThing, DirectAwareness) == 0x30);
FABLE_STATIC_ASSERT_AT(304, offsetof(CTCPerceiveThing, Smelt) == 0x31);
FABLE_STATIC_ASSERT_AT(305, offsetof(CTCPerceiveThing, Heard) == 0x32);
FABLE_STATIC_ASSERT_AT(306, offsetof(CTCPerceiveThing, SearchingForThingToPerceive) == 0x33);
FABLE_STATIC_ASSERT_AT(307, offsetof(CTCPerceiveThing, PDef_Object) == 0x34);
FABLE_STATIC_ASSERT_AT(308, offsetof(CTCPerceiveThing, UpdateOffset) == 0x38);
FABLE_STATIC_ASSERT_AT(309, offsetof(CTCPerceiveThing, SoundRadius) == 0x3c);
FABLE_STATIC_ASSERT_AT(310, offsetof(CTCPerceiveThing, SightRadius) == 0x40);
FABLE_STATIC_ASSERT_AT(311, offsetof(CTCPerceiveThing, ExtendedSightRadius) == 0x44);
FABLE_STATIC_ASSERT_AT(312, offsetof(CTCPerceiveThing, SmellRadius) == 0x48);
FABLE_STATIC_ASSERT_AT(313, offsetof(CTCPerceiveThing, GiveUpChaseRadius) == 0x4c);
FABLE_STATIC_ASSERT_AT(314, offsetof(CTCPerceiveThing, PerceptionCombat) == 0x50);
FABLE_STATIC_ASSERT_AT(315, offsetof(CTCPerceiveThing, FrameHitByPerceivedThing) == 0x54);

#endif // FABLE_RETAIL_CTCPERCEIVETHING
#endif
#endif // FABLE_ENGINE_CTCPERCEIVETHING_H
