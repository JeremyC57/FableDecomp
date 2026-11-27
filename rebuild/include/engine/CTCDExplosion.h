#ifndef FABLE_ENGINE_CTCDEXPLOSION_H
#define FABLE_ENGINE_CTCDEXPLOSION_H
#include "engine/CTCDriverBase.h"
#include "engine/CDefPointer.h"
#include "engine/CExplosionDef.h"
// Original TPI0x315c4; full native78 with PExplosionDef38; four vector reductions.
class CTCDExplosion : public CTCDriverBase {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
public: CTCDExplosion(CThing&);
#endif
private:
    float DamageRadius,PhysicsRadius,MaxDamage;
    long FireDamage,ExplosionLifeTimer;
#ifdef FABLE_RETAIL_EXPLOSION_HITS_PASS29
    C3DVector StartPos,EndPos;
#else
    unsigned char StartPos[12],EndPos[12];
#endif
    CDefPointer<const CExplosionDef> PExplosionDef;
#ifdef FABLE_RETAIL_DYNAMIC_EXPLOSION_PASS29
    bool CreatedEmitter, NeedToTriggerExplosion;
    std::vector<std::pair<CIntelligentPointer<CThing>, long> > Emitters;
    long ExplosionEvolutionTimer, CurrentExplosionRing;
    std::vector<CIntelligentPointer<CThing> > HitThings, ImmuneThings, SpecifiedThingsToHit;
    void UpdateDynamicExplosion();
    void UpdateDynamicHitThings(float inner_radius, float outer_radius);
#else
#ifdef FABLE_RETAIL_EXPLOSION_HITS_PASS29
    bool CreatedEmitter,NeedToTriggerExplosion;
    std::vector<std::pair<CIntelligentPointer<CThing>,long> > Emitters;
    long ExplosionEvolutionTimer,CurrentExplosionRing;
    std::vector<CIntelligentPointer<CThing> > HitThings,ImmuneThings,SpecifiedThingsToHit;
    void GiveDamageToThing(CThingGameObject&);
    void UpdateHitThings(const C3DVector&,const C3DVector&);
    float GetDistanceFromMeAsRatioOfDamageRadius(const CThing&) const;
#else
    unsigned char _pad_0x3c[0x3c];
#endif
#endif
#if defined(FABLE_RETAIL_DYNAMIC_EXPLOSION_PASS29) && defined(FABLE_RETAIL_EXPLOSION_HITS_PASS29)
    void GiveDamageToThing(CThingGameObject&);
    void UpdateHitThings(const C3DVector&,const C3DVector&);
    float GetDistanceFromMeAsRatioOfDamageRadius(const CThing&) const;
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_EXPLOSION; }
    CDefPointer<const CExplosionDef> GetPExplosionDef() const { return PExplosionDef; }
#ifdef FABLE_RETAIL_CTCDEXPLOSION_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x004E3165
public:
#endif
#if defined(FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION) || defined(FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT)
// Retail opt-in (Codex pass21): both original PDB types and donor OnHit/026FD4F0.
public:
static CThing* FABLE_FASTCALL Create(long,const C3DVector&);
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
void SetThingAsImmune(CThing&);
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
void SetSpecificThingToHit(CThing&);
#endif
#endif
#ifdef FABLE_RETAIL_CTCDEXPLOSION_DRIVERSLOTS
// agentFAM13 (2026-11-12): CTCDriverBase virtuals this driver overrides but this declaration lacks (TLC vtable slot 28 IsDriverType,
// slot 29 CreateAdditionalTCs, as engine/CTCDriverBase.h); spellings from the Ego_r / FableWin name sets. Declaration only.
public:
    virtual bool IsDriverType(long type) const;
#endif // FABLE_RETAIL_CTCDEXPLOSION_DRIVERSLOTS
};
FABLE_STATIC_ASSERT_AT(46, sizeof(CTCDExplosion)==0x78);
#endif
