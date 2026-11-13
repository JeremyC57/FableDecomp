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
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
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
 bool IsThingSmelt() const;
 bool IsThingHeard() const;
 bool IsThingPerceived() const;
 bool IsPerceiving() const;
 bool IsPlayerLineOfSight() const;
 bool IsPlayerExtendedLineOfSight() const;
 bool IsPlayerIgnoreFOVLineOfSight() const;
#ifdef FABLE_RETAIL_HEARING
 bool IsAwareOfThing()const{return AwarenessTimer!=0.0f;}
 bool IsAwarenessOfThingDirect()const{return DirectAwareness;}
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
 const CThing * GetPerceivedThing() const { return PerceivedThing.Get(); }
 void SetPerceivedThing(const CThing * p0);
 const C3DVector & GetAwarenessPosition() const;
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
FABLE_STATIC_ASSERT(sizeof(CTCPerceiveThing)==0x58);
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
    bool IsAwareOfThing() const { return AwarenessTimer!=0.0f; }
#endif
    bool IsThingLineOfSight() const { return LineOfSight; }
    bool IsAwarenessOfThingDirect() const;
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
    const CThing* GetPerceivedThing() const;                             // 0x007446A0
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
FABLE_STATIC_ASSERT(sizeof(CTCPerceiveThing)==0x58);
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

#pragma pack(push, 1)
class CTCPerceiveThing {
public:
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

FABLE_STATIC_ASSERT(sizeof(CTCPerceiveThing) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, field4) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, PerceivedThing) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, DistanceFromPerceivedThingSquared) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, AwarenessTimer) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, FramesSinceDirectlyPerceivedThing) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, LastPerceivedPos_X) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, LastPerceivedPos_Y) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, LastPerceivedPos_Z) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, Perceiving) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, LineOfSight) == 0x2d);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, ExtendedLineOfSight) == 0x2e);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, IgnoreFOVLineOfSight) == 0x2f);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, DirectAwareness) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, Smelt) == 0x31);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, Heard) == 0x32);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, SearchingForThingToPerceive) == 0x33);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, PDef_Object) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, UpdateOffset) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, SoundRadius) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, SightRadius) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, ExtendedSightRadius) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, SmellRadius) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, GiveUpChaseRadius) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, PerceptionCombat) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCPerceiveThing, FrameHitByPerceivedThing) == 0x54);

#endif // FABLE_RETAIL_CTCPERCEIVETHING
#endif
#endif // FABLE_ENGINE_CTCPERCEIVETHING_H
