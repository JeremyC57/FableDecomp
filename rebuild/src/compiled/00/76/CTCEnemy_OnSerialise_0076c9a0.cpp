// CTCEnemy::OnSerialise(CPersistContext& context, bool) -- persists the two enemy flags and the faction name. Saving: the name is
//   written only when the thing's CEnemyDef sub-def names a different faction than the current PFaction (else the empty default);
//   loading: a non-empty name is looked up in the world's faction manager and becomes PFaction.
// Leaf: ?OnSerialise@CTCEnemy@@MAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076C9A0, 607 bytes, `ret 8`; CTCEnemy vtable 0x0126A47C slot 1.
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<CCharString> 0x004109A0 (two-argument named-default form inline:
//   GetDefaultVal<CCharString> builds a local, copies it out, destroys it), CTCBase::GetDefinitionManager 0x006869D0,
//   CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C, CDefClassBase::GetSubDef<CEnemyDef> 0x006C7720,
//   CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0, CDefString::operator!=(const CCharString&) 0x0076D640,
//   CTCBase::PeekWorld 0x00686A80, CFactionManager::GetPFaction 0x006B93D0, CCharString ctor 0x0099E4B0 / (const char*,long)
//   0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0 / operator= 0x0099EFB0 / operator!=(const char*) 0x0099E960.
// Shape: the GetSubDef<T> and GetDef<T> template bodies are visible to the compiler (out of line under auto_inline(off), as in
//   the landed same-TU CTCEnemy::CTCEnemy 0x0076C8A0): with them VC7.1 keeps the enemy-def handle in ebp after the call and reuses
//   its stack slot (the dead context-argument slot) for the `differs` bool. CFaction::GetName() returns the name BY VALUE
//   (FableWin.pdb ?GetName@CFaction@@QBE?AVCCharString@@XZ): the operator!= argument is that returned temporary, evaluated and
//   pushed before the GetDefNameFromGlobalIndex object expression. CFaction has no shared header: local model of its +0 name only.
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
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CDEFSTRING_NE_CHARSTRING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCEnemy.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CEnemyDef.h"
#include "engine/CDefString.h"
#include "engine/CFactionManager.h"
#include "engine/CWorld.h"
#include "engine/CPersistContext.h"
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
class CFaction {
public:
    CCharString GetName() const { return Name; }
private:
    CCharString Name;
};
template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
void CTCEnemy::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("FriendsWithEverythingFlag", FriendsWithEverythingFlag, false);
    context.Transfer("EnableFollowersEnemyProxy", EnableFollowersEnemyProxy, true);
    if (context.IsSaving()) {
        CDefPointer<const CThingBaseDef> pthing_def;
        GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &pthing_def);
        CDefPointer<const CEnemyDef> penemy_def;
        CCharString faction_name;
        bool differs = PFaction && pthing_def->GetSubDef(&penemy_def) &&
            GetDefinitionManager().GetDefNameFromGlobalIndex(penemy_def->Faction) != PFaction->GetName();
        if (differs)
            faction_name = PFaction ? PFaction->GetName() : CCharString("");
        context.Transfer("FactionName", faction_name);
    } else {
        CCharString faction_name;
        context.Transfer("FactionName", faction_name);
        if (faction_name != "")
            PFaction = PeekWorld().GetFactionManager().GetPFaction(faction_name);
    }
}