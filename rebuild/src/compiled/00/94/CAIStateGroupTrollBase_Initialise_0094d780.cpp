// CAIStateGroup_TrollBase::Initialise(brain, creature, priority, scriptPriority, name) -- base initialise, then the troll's throwing distances: the maximum from the creature's perception give-up-chase radius, the others from the explosion radius of its area-of-effect attack (defaults 28 / 8 / 11 / 8 when the attack names no explosion)
// Leaf: ?Initialise@CAIStateGroup_TrollBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094D780, 427 bytes, thiscall `ret 14h`, /O2, no frame; virtual slot 1 of vtable 0x0128C008
//   (inherited by CAIStateGroup_EarthTroll / _RockTroll), overriding CAIStateGroupBase::Initialise 0x008FCEC0.
// Identity: Ego_r.pdb and FableWin.pdb both carry the mangled name above; rtti-vtables.tsv slot 1.
// Callees: CAIStateGroupBase::Initialise 0x008FCEC0 (direct, non-virtual); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   0x0040F020 (through the expanded CThing::GetTC<CTCPerceiveThing>, TCI_PERCEIVE_THING = 0x34);
//   CTCPerceiveThing::GetGiveUpChaseRadius 0x00744460 (Ego_r.pdb ?GetGiveUpChaseRadius@CTCPerceiveThing@@QBEMXZ; `fld [ecx+4Ch]`);
//   CDefClassBase::GetSubDef<CAreaOfEffectAttackDef> 0x008C79B0, GetSubDef<CTrapDef> 0x00788E53, GetSubDef<CExplosionDef>
//   0x00751190; CGameDefinitionManager::Get 0x0044C6B0 (x2); CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const
//   CThingBaseDef>*) const 0x0042AF3C (x2); the defs' virtual slot 1 (CDefPointer release).
// Shape: the GetTC result is ignored (a missing component reads the unset local, as the landed GetTC rows). The area-of-effect def
//   handle lives to the end of the function; the explosion index is ExplosionIndex, else the trap thing def's CTrapDef
//   ExplosionDefIndex. No index -> the four defaults (the block sits after `ret` and jumps back to the handle's release; a missing
//   TrapIndex threads straight to it). The three radius stores are three reads of ExplosionRadius (`mov`, `mov`, `fld ; fadd 1.0f`).
//   A creature whose base def has no CAreaOfEffectAttackDef keeps only the maximum.
// What made it match (was DIFFER 487v427 on local stand-in models): the shared headers, with the REAL template bodies of
//   CDefinitionManager::GetDef<T> and CDefClassBase::GetSubDef<T> visible and out of line (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY /
//   FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, both __declspec(noinline) in engine/CDefinitionManager.h). VC7.1 then knows the def
//   handles do not escape: no `sub esp`, every handle homed in a dead argument slot, the thing def copied to esi while its slot is
//   reused for the sub-def handle.
// Headers: overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_TROLLBASE (engine/CAIStateGroup_TrollBase.h: the class on its real base) and
//   FABLE_RETAIL_CEXPLOSIONDEF_MEMBERS (engine/CExplosionDef.h: ExplosionRadius +0x2C, StatGetTypeName). CAreaOfEffectAttackDef and
//   CTrapDef have no shared header: modelled here as the landed GetSubDef rows do, plus the Ego_r.pdb members read at retail offset =
//   PDB offset - 4. CDefClassBase is 0x25 in both; the shift is the STLport container: CAreaOfEffectAttackDef's base
//   CParentDefClassBase is 0x39 retail (SubDefInfoMap CVectorMap 0x10, engine/CParentDefClassBase.h) v 0x3D in Ego_r (Dinkumware
//   0x14 vector), so its members start at +0x3C, not +0x40; CTrapDef's Sound map is 12 bytes retail v 16 in Ego_r (as the landed
//   CTCTrapBase::Trigger 0x00788229: ExplosionDefIndex +0x50).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_TROLLBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING
#define FABLE_RETAIL_CEXPLOSIONDEF_MEMBERS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CExplosionDef.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_TrollBase.h"

class CAreaOfEffectAttackDef : public CParentDefClassBase {      // RTTI .?AVCAreaOfEffectAttackDef@@ : CParentDefClassBase; no shared header
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAreaOfEffectAttackDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long TrapIndex;                                               // +0x3C (Ego_r +0x40)
    long ExplosionIndex;                                          // +0x40 (Ego_r +0x44)
    float ObstructionRadius;                                      // +0x44
    float ObstructionLifeTime;                                    // +0x48
};
class CTrapDef : public CSubDefClassBase {                        // RTTI .?AVCTrapDef@@ : CSubDefClassBase; no shared header
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTrapDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    unsigned char _pad_0x25[0x2b];                                // Sound .. UntriggeredCollisionMesh
    long ExplosionDefIndex;                                       // +0x50 (Ego_r +0x54)
};

void CAIStateGroup_TrollBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority,
                                         EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    CTCPerceiveThing* perceive;
    PBrain->Creature->GetTC(&perceive);
    TROLL_MAX_THROWING_DISTANCE = perceive->GetGiveUpChaseRadius();
    CDefPointer<const CAreaOfEffectAttackDef> aoe;
    if (PBrain->Creature->PeekBaseDef().GetSubDef(&aoe)) {
        long explosion_index = 0;
        if (aoe->ExplosionIndex != 0)
            explosion_index = aoe->ExplosionIndex;
        else if (aoe->TrapIndex != 0) {
            CDefPointer<const CThingBaseDef> trap_thing;
            if (CGameDefinitionManager::Get().GetDef(aoe->TrapIndex, &trap_thing)) {
                CDefPointer<const CTrapDef> trap;
                if (trap_thing->GetSubDef(&trap))
                    explosion_index = trap->ExplosionDefIndex;
            }
        }
        if (explosion_index != 0) {
            CDefPointer<const CThingBaseDef> explosion_thing;
            if (CGameDefinitionManager::Get().GetDef(explosion_index, &explosion_thing)) {
                CDefPointer<const CExplosionDef> explosion;
                if (explosion_thing->GetSubDef(&explosion)) {
                    TROLL_MIN_THROWING_DISTANCE = explosion->ExplosionRadius;
                    TROLL_STOP_POUNDING_DISTANCE = explosion->ExplosionRadius;
                    TROLL_STOP_THROWING_DISTANCE = explosion->ExplosionRadius + 1.0f;
                }
            }
        } else {
            TROLL_MAX_THROWING_DISTANCE = 28.0f;
            TROLL_MIN_THROWING_DISTANCE = 8.0f;
            TROLL_STOP_POUNDING_DISTANCE = 11.0f;
            TROLL_STOP_THROWING_DISTANCE = 8.0f;
        }
    }
}