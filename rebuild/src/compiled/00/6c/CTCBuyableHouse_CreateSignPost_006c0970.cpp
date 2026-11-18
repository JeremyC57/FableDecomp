// 006c0970 CTCBuyableHouse::CreateSignPost -- Creates an owned sign and moves an overlapping player clear of it.
// Leaf: ?CreateSignPost@CTCBuyableHouse@@AAEPAVCThing@@ABVCCharString@@_N@Z
// Retail: 0x006C0970, 419 bytes, thiscall.
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/ETCInterfaceType.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_CREATE_SIGN_POST_PASS24
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_GETSIGNPOSTPOS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObject.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCPhysicsBase.h"
#include <math.h>

CThing* CTCBuyableHouse::CreateSignPost(const CCharString& sign_post_def, bool main)
{
    C3DVector post_pos;
    CRightHandedSet rhs;
    CThingObject* new_post = 0;
    if (GetSignPostPos(main, post_pos, &rhs)) {
        long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(sign_post_def);
        new_post = CThingObject::Create(def_index, post_pos, GetParentThing().GetPlayerNumber(), &rhs, 0, "");
        CTCOwnedEntity* ptc_owned_entity;
        if (!new_post->GetTC(TCI_OWNED_ENTITY, &ptc_owned_entity))
            ptc_owned_entity = (CTCOwnedEntity*)new_post->AddTC("CTCOwnedEntity", false, 0);
        ptc_owned_entity->SetPOwner(&GetParentThing());
        CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (player) {
            float total_rad = new_post->GetPhysicsTC()->Radius + player->GetPhysicsTC()->Radius;
            const C3DVector& heroPos = player->GetPos();
            const C3DVector& postPos = new_post->GetPos();
            float dx = postPos.X - heroPos.X;
            float dy = postPos.Y - heroPos.Y;
            if ((float)sqrt(dy * dy + dx * dx) < total_rad)
                player->GetPhysicsTC()->SetPosition(post_pos + rhs.GetForward() * total_rad);
        }
    }
    return new_post;
}