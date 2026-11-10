// CCreatureAction_CastPhysicalShieldSpell::Initialise -- create and configure a shield.
// Leaf: ?Initialise@CCreatureAction_CastPhysicalShieldSpell@@UAEXXZ
// Retail: Fable.exe 0x008B9DD0,125 bytes.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTION_CASTPHYSICALSHIELD
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_CastPhysicalShieldSpell.h"
#include "engine/CTCPhysicalShieldSpell.h"

void CCreatureAction_CastPhysicalShieldSpell::Initialise()
{
    CCombatActionBase::Initialise();
    if(!PCreature->HasTC(TCI_PHYSICAL_SHIELD_SPELL)) {
        CTCPhysicalShieldSpell* ptc_shield=GFDebugPDynamicCast(PCreature->AddTC("CTCPhysicalShieldSpell",true,0),(CTCPhysicalShieldSpell*)0);
        if(PCreature->GetType()==1) {
            ptc_shield->SetAICasting(LevelToCast);
            ptc_shield->SetTimeLimit(TimeLimit);
        }
        ptc_shield->SetShieldHealth(ShieldHealth);
    }
}