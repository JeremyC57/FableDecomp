// CTCTavernGameCoinBase::FindObstacles
// Leaf: ?FindObstacles@CTCTavernGameCoinBase@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CCOINGAMEOBSTACLEDEF_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_MASS_INLINE_DTOR_PASS26
#define FABLE_RETAIL_COIN_OWNER_TYPE_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_LIST
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTCThingOwner.h"
#include "engine/CCoinGameObstacleDef.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase_GetSubDef.h"

void CTCTavernGameCoinBase::FindObstacles()
{
    CTCThingOwner* tc_owner;
    if (GetParentThing().GetTC(&tc_owner)) {
        const std::list<CIntelligentPointer<CThing> >& children = tc_owner->GetObjectList();
        for (std::list<CIntelligentPointer<CThing> >::const_iterator i = children.begin(); i != children.end(); ++i) {
            CDefPointer<const CCoinGameObstacleDef> obstacle(NULL_DEF);
            if (IS_THING_ALIVE(*i) && (*i)->PeekBaseDef().GetSubDef(&obstacle)) {
                C3DVector pos((*i)->GetPos());
                pos -= BoardPos;
                float y = pos.Dot(BoardRHSet.GetForward());
                float x = pos.Dot(BoardRHSet.GetRight());
                C2DVector local_pos(x, y);
                Coin c(obstacle->Radius, 0.0f, 0, false);
                c.Pos = local_pos;
                c.Thing = CIntelligentPointer<CThing>((*i).Get());
                c.Owner = 0;
                Coins.push_back(c);
            }
        }
    }
}