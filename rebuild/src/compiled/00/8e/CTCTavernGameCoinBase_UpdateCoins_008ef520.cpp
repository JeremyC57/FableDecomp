// CTCTavernGameCoinBase::UpdateCoins
// Leaf: ?UpdateCoins@CTCTavernGameCoinBase@@QAEXM@Z
// Flags: /Oi
// Bracket: grouping the angular friction scale preserves the retail x87 cleanup at 0x008EEC56.
#define FABLE_RETAIL_COIN_UPDATE_PASS27
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_DIVASSIGN_PASS26
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_MASS_BACKPLANEFRICTION
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTavernGameCoinBaseDef.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefinitionManager.h"
#include <math.h>

#ifdef _MSC_VER
__declspec(noinline)
#endif
void Mass::BackplaneFriction(float strength, float radius)
{
    float speed = Vel.GetMagnitude();
    float factor = speed <= strength ? 0.0f : 1.0f - strength / speed;
    Vel *= factor;
    strength /= radius;
    speed = fabsf(AVel);
    factor = speed <= strength ? 0.0f : (1.0f - (strength / speed));
    AVel *= factor;
}

template<> inline bool CDefClassBase::GetSubDef<CTavernGameCoinBaseDef>(CDefPointer<const CTavernGameCoinBaseDef>* out) const
{
    const CSubDefInfo* info;
    {
        CCharString name("CTavernGameCoinBaseDef", -1);
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CTavernGameCoinBaseDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = result;
        return true;
    }
    return false;
}

void CTCTavernGameCoinBase::UpdateCoins(float time_delta)
{
    CDefPointer<const CTavernGameCoinBaseDef> pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&pdef);
    for (std::vector<Coin>::iterator i = Coins.begin(); i != Coins.end(); ++i) {
        i->Update(time_delta);
        i->BackplaneFriction(pdef->CoinSurfaceFriction * time_delta, i->Radius);
    }
    for (std::vector<Coin>::iterator i = Coins.begin(); i != Coins.end(); ++i) {
        if (!IS_THING_ALIVE((*i).Thing)) {
            SetState(ETGS_LEAVE);
            return;
        }
        if (IS_THING_ALIVE((*i).Thing) && !(*i).Thing->DrawIsDrawable() && !((*i).Thing->GetTypeFlags() & 0x8000))
            continue;
        for (std::vector<Coin>::iterator j = Coins.begin(); j != i; ++j) {
            if (!IS_THING_ALIVE((*j).Thing)) {
                SetState(ETGS_LEAVE);
                return;
            }
            if (IS_THING_ALIVE((*j).Thing) && !(*j).Thing->DrawIsDrawable() && !((*j).Thing->GetTypeFlags() & 0x8000))
                continue;
            if (i->IMass == 0.0f && j->IMass == 0.0f)
                continue;
            C2DVector dir = i->Pos - j->Pos;
            float dist = dir.GetSquaredMagnitude();
            if (dist == 0.0f)
                continue;
            if (dist < (i->Radius + j->Radius) * (i->Radius + j->Radius)) {
                dist = sqrtf(dist);
                dir /= dist;
                if (Mass::Collide(&*i, &*j, i->Pos - dir * i->Radius, j->Pos + dir * j->Radius, dir,
                                  pdef->CoinEdgeRestitution, pdef->CoinEdgeFriction, 0.0f, time_delta)) {
                    CIntelligentPointer<CThing> thing(i->Thing);
                    if (thing == 0)
                        thing = j->Thing;
                    if (thing != 0) {
                        CTCSoundPlayer* sound_player;
                        thing->GetTC(&sound_player);
                        const CCharString hit("TAVERN_GAME_COIN_HIT_COIN");
                        sound_player->PlayAttachedCriteria(hit, true, false, false, -1);
                    }
                }
            }
        }
    }
}