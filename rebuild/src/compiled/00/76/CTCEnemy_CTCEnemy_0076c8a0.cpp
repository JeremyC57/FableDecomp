// CTCEnemy::CTCEnemy(CThing&) -- initialize enemy and ally lists and resolve the owner's faction.
// Leaf: ??0CTCEnemy@@QAE@AAVCThing@@@Z
// Retail: 0x0076C8A0,254 bytes,__fastcall.
// Flags: none
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_FACTIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENEMYDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCEnemy.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CEnemyDef.h"
#include "engine/CDefString.h"
#include "engine/CFactionManager.h"
#include "engine/CWorld.h"
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
#include "engine/CDefClassBase_GetSubDef.h"
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out) const
{
    if(index<=0) return false;
    CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
    if(def.Get()) { out->SetDynamicCast(def); return true; }
    return false;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void CTCEnemy::SetFaction(const CCharString& faction)
{
    PFaction=PeekWorld().GetFactionManager().GetPFaction(faction);
}
CTCEnemy::CTCEnemy(CThing& thing)
    : CTCBase(thing),FriendsWithEverythingFlag(false),EnableFollowersEnemyProxy(true),PFaction(0)
{
    CDefPointer<const CThingBaseDef> pthing_def;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(),&pthing_def);
    CDefPointer<const CEnemyDef> penemy_def;
    if(pthing_def->GetSubDef(&penemy_def))
        SetFaction(GetDefinitionManager().GetDefNameFromGlobalIndex(penemy_def->Faction));
}