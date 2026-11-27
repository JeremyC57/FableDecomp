#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
// Leaf: ?Main@CCeremonialEffect@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Flags: (none)
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
// Inline-budget: native CCeremonialEffect expands the position getter at the three effect creation sites.
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
void NScript::CQ_OpeningGraveyardSecretPassageScript::CCeremonialEffect::Main()

{
    TRetVoid _Fail_Val;
    bool Activated = false;
    CScriptThing particle_effect;
    long Index = GFCharStringToInt(Me.GetDataString()) - 1;
    while (!Game->IsObjectInThingsPossession(ParentClass.hero_object[Index], Game->GetHero())) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            { _Fail_Val.Return(); return; }
        if (!Activated) {
            if (IsActiveThreadTerminating())
                { _Fail_Val.Return(); return; }
            if (ParentClass.skeleton_visited) {
                if (IsActiveThreadTerminating())
                    { _Fail_Val.Return(); return; }
                Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
                particle_effect = Game->CreateEffect("GRAVEYARD_SPARKLES_GRAVE", Me.GetPos(), "", 0.0f, false, true);
                Activated = true;
            }
        } else {
            if (IsActiveThreadTerminating())
                { _Fail_Val.Return(); return; }
            if (!particle_effect.IsAlive()) {
                if (IsActiveThreadTerminating())
                    { _Fail_Val.Return(); return; }
                particle_effect = Game->CreateEffect("GRAVEYARD_SPARKLES_GRAVE", Me.GetPos(), "", 0.0f, false, true);
            }
        }
    }
    if (IsActiveThreadTerminating())
        { _Fail_Val.Return(); return; }
    Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
    Game->MiniMapRemoveMarker(Me);
    if (particle_effect.IsAlive()) {
        if (IsActiveThreadTerminating())
            { _Fail_Val.Return(); return; }
        Game->RemoveThing(particle_effect, false, true);
    }
    if (ParentClass.doors_open) {
        if (IsActiveThreadTerminating())
            { _Fail_Val.Return(); return; }
        CScriptThing spawn_point = Game->GetNearestWithScriptName(Me, "SecretPassageUndeadSpawn");
        CScriptThing particle_effect;
        CScriptThing zombie=Game->CreateCreature("CREATURE_OK_UNDEAD_LIEUTENANT_AXE_LOW_DETAIL", spawn_point.GetPos(), "", true);
        Game->GiveThingBestEnemyTarget(zombie,Game->GetHero());
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            { _Fail_Val.Return(); return; }
    }
}