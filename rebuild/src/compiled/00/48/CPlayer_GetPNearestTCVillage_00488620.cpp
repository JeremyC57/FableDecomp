// CPlayer::GetPNearestTCVillage -- nearest village component for the living player character.
// Retail: 0x00488620, 196 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?GetPNearestTCVillage@CPlayer@@QAEPAVCTCVillage@@XZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGVILLAGE_STATGETTYPE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_NEARESTVILLAGE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TCI
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThing.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingVillage.h"
#include "engine/CTCVillage.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CThingSearchTools.h"
#include "engine/ThingAlive.h"
#include "stlport/member_binders.h"
CTCVillage* CPlayer::GetPNearestTCVillage()
{
    if(IS_THING_ALIVE(PeekPPlayerCharacter())){
        CThingVillage* pvillage=0;
        if(GetThingSearchTools().GetPBestLeafThing(std::bind2nd(std::mem_fun_ref(&CThing::HasTC),TCI_VILLAGE),
            CThingCompare_Nearest(GetPPlayerCharacter()->PeekPos()),&pvillage)){
            CTCVillage* ptcvillage;
            pvillage->GetTC(&ptcvillage);
            return ptcvillage;
        }
    }
    return 0;
}