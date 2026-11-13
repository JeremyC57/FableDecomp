// CTCTavernTable::OnInitialActivate() -- set the table up from its owner's named action points: for every free DUMMY_POINT_TANKARD_<n> point grow the per-seat vectors to n entries, create the def's tankard there (position and orientation handed to its tankard component and physics, created-by-owner flag, created / owned entity components naming the table's owner) when the table def asks for tankards, and clear the seat's tankard, food, used flag and occupant; then nobody is seated or being served
// Leaf: ?OnInitialActivate@CTCTavernTable@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B90F0, 1257 bytes, plain `ret`; speed TU with an 8-aligned frame (`and esp,-8 ; sub esp,78h`).
//   Identity: CTCTavernTable vtable 0x0126DBB4 slot 6; Ego_r.pdb / FableWin.pdb ?OnInitialActivate@CTCTavernTable@@UAEXXZ.
// Callees: CVectorMap LowerBound 0x0040F020 (GetTC expanded for TCI_NAMED_USER_ACTION_POINTS / TCI_TAVERN_TANKARD / TCI_CREATED_ENTITY);
//   vector<CIntelligentPointer<CThingObject>>::erase 0x007B9870 / resize 0x007B9EC0, vector<bool>::_M_fill_insert 0x004676EF,
//   vector<CThingAICreature*>::_M_fill_insert 0x007B98C0, memmove 0x00BFEAE6; CTCNamedUserActionPoints::GetFreeNamedActionPoint 0x007FF190 /
//   GetName 0x007FEFC0; CCharString 0x0099EBF0 / 0x0099E4B0 / ~ 0x0099EAE0, GetSubString 0x0099EC70, GFCharStringToInt 0x0099E7F0;
//   GetSubDef<CTavernTableDef> 0x007B9700 (same TU); CTCBase::GetWorld 0x00686A70 / PeekPlayerManager 0x00686D50; GetTankardName 0x007B8680;
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410; CPlayerManager::GetMainPlayer const 0x00449960; CThingObject::Create 0x00703210;
//   CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10; CTCPhysicsBase::SetRHSet
//   (slot 75); CThing::AddTC 0x004C9D60; CTCOwnedEntity::SetPOwner 0x007E8590.
// Shape: GetParentThing() as the first GetFreeNamedActionPoint argument (ParentThing in esi before the CCharString argument is built,
//   0x007B9150), the player number in a named local before Create's implicit "" argument, FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND (the
//   two in-loop LowerBounds expand, the entry one is called), a same-TU GetSubDef<CTavernTableDef> (release without a null test).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHING_CREATEDBYOWNER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCTAVERNTABLE_FULL_LAYOUT
#define FABLE_RETAIL_CTAVERNTABLEDEF
#define FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_FREEPOINT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTavernTableDef.h"
#include "engine/CTCTavernTable.h"
#include "engine/CTCNamedUserActionPoints.h"
#include "engine/CTCTavernTankard.h"
#include "engine/CTCCreatedEntity.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

long __fastcall GFCharStringToInt(const CCharString& text);              // 0x0099E7F0 ?GFCharStringToInt@@YIJABVCCharString@@@Z

// GetSubDef<CTavernTableDef> 0x007B9700 (landed, same body): defined in this TU as in retail's, so VC7.1 sees that it does not
// capture the def slot.
template <>
bool CDefClassBase::GetSubDef<CTavernTableDef>(CDefPointer<const CTavernTableDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernTableDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

void CTCTavernTable::OnInitialActivate()
{
    CTCNamedUserActionPoints* action_points = 0;
    ParentThing->GetTC(TCI_NAMED_USER_ACTION_POINTS, &action_points);
    Tankards.clear();
    C3DVector pos;
    CRightHandedSet rh_set;
    long index;
    while (action_points->GetFreeNamedActionPoint(GetParentThing(), pos, rh_set, "DUMMY_POINT_TANKARD_", index, 0, false)) {
        CCharString name;
        action_points->GetName(index, name);
        long seat = GFCharStringToInt(name.GetSubString(ActionPointTankardStrlen, name.GetLength())) - 1;
        if (SeatsCurrentlyUsed.size() < (unsigned long)(seat + 1)) {
            Tankards.resize(seat + 1);
            Food.resize(seat + 1);
            SeatsCurrentlyUsed.resize(seat + 1);
            Occupants.resize(seat + 1);
        }
        bool add_tankards;
        {
            CDefPointer<const CTavernTableDef> def;
            ParentThing->PeekBaseDef().GetSubDef(&def);
            add_tankards = def->AddTankards;
        }
        if (add_tankards) {
            long def_index = GetWorld().PeekDefinitionManager().GetDefGlobalIndexFromName(GetTankardName());
            long player = PeekPlayerManager().GetMainPlayer().GetNumber();
            CThingObject* tankard = CThingObject::Create(def_index, pos, player, 0, 0, "");
            CTCTavernTankard* tankard_tc = 0;
            ((CThing*)tankard)->GetTC(TCI_TAVERN_TANKARD, &tankard_tc);
            if (tankard_tc) {
                tankard_tc->RHSet = rh_set;
                tankard_tc->Pos = pos;
            }
            Tankards[seat] = tankard;
            ((CThing*)tankard)->GetPhysicsTC()->SetRHSet(rh_set);
            ((CThing*)tankard)->SetAsCreatedByOwnerThingCreation(true);
            ((CThing*)tankard)->AddTC("CTCCreatedEntity", false, 0);
            CTCCreatedEntity* created;
            ((CThing*)tankard)->GetTC(TCI_CREATED_ENTITY, &created);
            created->SetPCreator(*ParentThing);
            CTCOwnedEntity* owned = (CTCOwnedEntity*)((CThing*)tankard)->AddTC("CTCOwnedEntity", false, 0);
            owned->SetPOwner(ParentThing);
        } else {
            Tankards[seat] = 0;
        }
        Food[seat] = 0;
        SeatsCurrentlyUsed[seat] = false;
        Occupants[seat] = 0;
    }
    NumPeopleSeated = 0;
    BeingServed = false;
}