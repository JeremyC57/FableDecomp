// Standalone fixture for the candidate body. Engine services are deterministic doubles.
#include <stdio.h>
#include <vector>
#include <string.h>
enum EOpinionDeedType { TestDeed };
enum ETCInterfaceType { TestComponent, TCI_BERSERK = 132, TCI_CREATURE_OPINION_OF_HERO = 79, TCI_GUARD = 180, TCI_DIGGING_SPOT = 239, TCI_TROPHY = 57 };
enum EContextSensitiveOpinionExpression { Offensive, Attractive, Scary, Agreeable, Ugly, Ridiculous };
enum EContextSensitiveType { CONTEXT_SENSITIVE_NULL, CONTEXT_SENSITIVE_GUILD_SEAL, CONTEXT_SENSITIVE_LAMP, CONTEXT_SENSITIVE_HEALTH,
    CONTEXT_SENSITIVE_MANA, CONTEXT_SENSITIVE_EXPRESSION, CONTEXT_SENSITIVE_OPINION_EXPRESSION, CONTEXT_SENSITIVE_GIFT,
    CONTEXT_SENSITIVE_MARKER, CONTEXT_SENSITIVE_TROPHY };
enum ECreatureType { NOT_HUMAN, HUMAN_CHILD, HUMAN_ADULT, HUMAN_ELDERLY };

struct C2DCoordI { long x,y; };
struct C2DBoxI { C2DBoxI(C2DCoordI,long) {} };
struct C3DVector { float X,Y,Z; C2DCoordI ToC2DCoordI() const {C2DCoordI v={(long)X,(long)Y};return v;} };
struct C2DVector { float X,Y; explicit C2DVector(const C3DVector& v):X(v.X),Y(v.Y){} };
static float GFGet2DDistanceSquared(const C2DVector& a, const C2DVector& b) {
    const float dx = a.X - b.X, dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}
static float GFSqr(float value) { return value * value; }
struct CThing {
    CThing():Dead(false),MaxHealth(100),Health(50),ThingTypeFlags(0),DefGlobalIndex(0){memset(tc,0,sizeof(tc));pos.X=pos.Y=pos.Z=0;}
    bool Dead; float MaxHealth,Health; int ThingTypeFlags,DefGlobalIndex;
    C3DVector pos; void* tc[256];
    const C3DVector& GetPos() const{return pos;}
    bool HasTC(int k)const{return tc[k]!=0;}
    float GetMaxHealth() const { return MaxHealth; }
    float GetHealth() const { return Health; }
    int GetTypeFlags() const { return ThingTypeFlags; }
    int GetDefGlobalIndex() const { return DefGlobalIndex; }
    template<class T> bool PeekTC(const T** out) const { return PeekTC(out, T::Type); }
    template<class T> bool PeekTC(ETCInterfaceType type, const T** out) const { return PeekTC(out, int(type)); }
    template<class T> bool GetTC(T** out) { return GetTC(out, T::Type); }
    template<class T> bool PeekTC(const T** out,int k)const{*out=(const T*)tc[k];return *out!=0;}
    template<class T> bool GetTC(T** out,int k)const{*out=(T*)tc[k];return *out!=0;}
};
static bool IS_THING_ALIVE(const CThing* thing){return thing&&!thing->Dead;}
struct CThingCreatureBase: CThing { CThingCreatureBase():pick(false),kind(1){} bool pick; int kind; bool IsPickPocketable()const{return pick;} int GetCreatureType()const{return kind;} };
struct CThingAICreature: CThingCreatureBase {
    CThingAICreature():RespondingToFollowAndWait(false),CanBeCourted(false){}
    bool RespondingToFollowAndWait,CanBeCourted;
    bool IsRespondingToFollowAndWait() const {return RespondingToFollowAndWait;}
    bool IsCourtable() const {return CanBeCourted;}
};
struct CThingPlayerCreature: CThingCreatureBase{};
struct CTCHeroStats{ enum {Type = 4}; long MaxStamina,Stamina; long GetMaxStamina()const{return MaxStamina;} long GetStamina()const{return Stamina;} };
struct CTCTargetingBase{ enum {Type = 8}; CThing* target;CThing* GetPTargetedThing()const{return target;}};
struct CTCDoor{ enum {Type = 38}; bool lock;bool IsLockPickable()const{return lock;}};
struct CTCStockItem{ enum {Type = 84}; bool Stealable; bool IsStealable() const { return Stealable; }};
struct CTCFollowed{ enum {Type = 188}; bool valid,following;bool IsCreatureValidFollower(const CThingAICreature&,int)const{return valid;}bool IsFollower(const CThing*)const{return following;}};
struct CTCCreatureOpinionOfHero{ enum {Type = 79}; bool InFindAGuardAndYellForHelp; bool IsInFindAGuardAndYellForHelp() const { return InFindAGuardAndYellForHelp; }};
struct CTCGuard{ enum {Type = 180}; bool criminal;bool PlayerIsCriminalAndCanApologise()const{return criminal;}};
struct CTCWife{ enum {Type = 215}; bool court,love,ring;bool IsCourtableAndNotBlockingIt()const{return court;}bool IsInLoveWithHero()const{return love;}bool HasReceivedWeddingRing()const{return ring;}};
struct CTCDiggingSpot{ enum {Type = 239}; bool enabled,hidden;bool IsEnabled()const{return enabled;}bool IsHidden()const{return hidden;}};
struct CTCCarrying{ enum {Type = 70}; CThing* item;CThing* GetThingInPrimarySlot()const{return item;}};
struct CTCTrophy{ enum {Type = 57}; bool MinigameInProgress; bool IsMiniGameInProgress() const {return MinigameInProgress;} };
struct CTCInventory{long NumberOfCSOpinionExpressions[8],LastCSOpinionExpression[8],LastButOneCSOpinionExpression[8];
    long GetCSOpinionExpressionCount(EContextSensitiveOpinionExpression e) const { return NumberOfCSOpinionExpressions[e]; }
    long GetLastOpinionExpression(EContextSensitiveOpinionExpression e) const { return LastCSOpinionExpression[e]; }
    long GetLastButOneOpinionExpression(EContextSensitiveOpinionExpression e) const { return LastButOneCSOpinionExpression[e]; }
};
template<class T> struct CDefPointer{T* p;T* operator->()const{return p;}};
struct COpinionDeedEffectsDef{bool ReactionRestrictToTarget;};
struct CTCHeroOpinionDeedLog{ enum {Type = 81}; COpinionDeedEffectsDef d;const COpinionDeedEffectsDef* PeekPDeedEffectsDef(EOpinionDeedType)const{return &d;}};
struct CGiftDef{bool IsWeddingRing;};struct CTrophyDef{};
struct CThingBaseDef{
    CGiftDef* gift;CTrophyDef* trophy;
    bool GetSubDef(CDefPointer<const CGiftDef>* out)const{out->p=gift;return gift!=0;}
    bool GetSubDef(CDefPointer<const CTrophyDef>* out)const{out->p=trophy;return trophy!=0;}
};
struct CGameDefinitionManager{
   static CGameDefinitionManager& Get(){static CGameDefinitionManager m;return m;}
   CThingBaseDef* def;bool GetDef(long,CDefPointer<const CThingBaseDef>* out)const{out->p=def;return def!=0;}
};
static bool dig,fish,react;
struct CTCCarriedActionUseSpade{static bool IsThingAbleToDig(const CThingCreatureBase&){return dig;}};
struct CTCCarriedActionUseFishingRod{static bool IsThingAbleToFish(const CThingCreatureBase&){return fish;}};
struct CThingFilter_HasTC { explicit CThingFilter_HasTC(ETCInterfaceType){} };
struct CThingMarker : CThing {};
struct CThingSearchTools {
    std::vector<CThingMarker*> found;
    template<class T> void GetAllThingsInRoughArea(const C2DBoxI&,const CThingFilter_HasTC&,std::vector<T*>& out) const { out=found; }
};
struct CWorld { CThingSearchTools* PThingSearchTools; const CThingSearchTools& PeekThingSearchTools() const {return *PThingSearchTools;} };
struct CMainGameComponent {static CMainGameComponent* PMainGameComponent; CWorld* PWorld; static CMainGameComponent* Get(){return PMainGameComponent;} const CWorld& PeekWorld() const {return *PWorld;} };
CMainGameComponent* CMainGameComponent::PMainGameComponent=0;
static long GFFloatToLongCeil(float f){long i=(long)f;return i+(i<f);}
struct CContextSensitiveItemDef{
   int Type,ExtraData1,ExtraData2;
   bool TargetCreatureLikelyToReactToDeed(const CThingPlayerCreature*,const CThingAICreature*,EOpinionDeedType)const{return react;}
   bool IsValidOverride(const CThing&,long,const CTCInventory*)const;
};
template<class S,class T> static T GFDebugPDynamicCast(S p,T){return static_cast<T>(p);}
bool CContextSensitiveItemDef::IsValidOverride(const CThing& thing, long inventory_item_def_index, const CTCInventory* ptc_inventory) const
{
    const CThingPlayerCreature* phero = GFDebugPDynamicCast(&thing, (const CThingPlayerCreature*)0);
    if (IS_THING_ALIVE(phero))
    switch (Type) {
    case CONTEXT_SENSITIVE_LAMP:
        return true;
    case CONTEXT_SENSITIVE_HEALTH:
        if (phero->GetMaxHealth() - phero->GetHealth() >= ExtraData1 || ExtraData2 == 0 || ExtraData2 >= phero->GetHealth())
            return true;
        break;
    case CONTEXT_SENSITIVE_MANA: {
        const CTCHeroStats* ptc_hero_stats;
        if (phero->PeekTC(&ptc_hero_stats) && ptc_hero_stats->GetMaxStamina() - ptc_hero_stats->GetStamina() >= ExtraData1)
            return true;
        break;
    }
    case CONTEXT_SENSITIVE_EXPRESSION: {
        const CTCTargetingBase* ptc_targeting = 0;
        phero->PeekTC(&ptc_targeting);
        if (!ptc_targeting) break;
        CThing* ptarget = ptc_targeting->GetPTargetedThing();
        if (!ptarget) break;
        if (phero->HasTC(TCI_BERSERK))
            return false;
        switch (ExtraData1) {
        case 1: {
            CThingCreatureBase* pcreature = GFDebugPDynamicCast(ptarget, (CThingCreatureBase*)0);
            if ((ptarget->GetTypeFlags() & 8) && pcreature->IsPickPocketable())
                return true;
            return false;
        }
        case 2: {
            const CTCDoor* ptc_door;
            if (ptarget->PeekTC(&ptc_door) && ptc_door->IsLockPickable())
                return true;
            return false;
        }
        case 3: {
            const CTCStockItem* ptc_stock_item;
            if (ptarget->PeekTC(&ptc_stock_item) && ptc_stock_item->IsStealable())
                return true;
            return false;
        }
        case 4: {
            const CThingAICreature* pcreature = GFDebugPDynamicCast(ptarget, (const CThingAICreature*)0);
            if (!(ptarget->GetTypeFlags() & 0x10) || !pcreature->IsRespondingToFollowAndWait())
                return false;
            const CTCFollowed* ptc_followed = 0;
            phero->PeekTC(&ptc_followed);
            if (ptc_followed != 0 && ptc_followed->IsCreatureValidFollower(*pcreature, 0) && !ptc_followed->IsFollower(ptarget))
                return true;
            return false;
        }
        case 5: {
            const CThingAICreature* pcreature = GFDebugPDynamicCast(ptarget, (const CThingAICreature*)0);
            if (!(ptarget->GetTypeFlags() & 0x10) || !pcreature->IsRespondingToFollowAndWait())
                return false;
            const CTCFollowed* ptc_followed = 0;
            phero->PeekTC(&ptc_followed);
            if (ptc_followed != 0 && ptc_followed->IsFollower(ptarget))
                return true;
            return false;
        }
        case 6: {
            if (!(ptarget->GetTypeFlags() & 0x10))
                return false;
            const CTCCreatureOpinionOfHero* ptc_opinion;
            if (ptarget->PeekTC(&ptc_opinion) && ptc_opinion->IsInFindAGuardAndYellForHelp())
                return true;
            CTCGuard* ptc_guard;
            if (ptarget->GetTC(&ptc_guard) && ptc_guard->PlayerIsCriminalAndCanApologise())
                return true;
            return false;
        }
        }
        return false;
    }
    case CONTEXT_SENSITIVE_OPINION_EXPRESSION: {
        const CTCTargetingBase* ptc_targeting = 0;
        phero->PeekTC(&ptc_targeting);
        if (!ptc_targeting) break;
        CThing* ptarget = ptc_targeting->GetPTargetedThing();
        if (ptarget != 0) {
            if (!(ptarget->GetTypeFlags() & 0x10) || !ptarget->HasTC(TCI_CREATURE_OPINION_OF_HERO) || ptarget->HasTC(TCI_GUARD))
                return false;
            CThingAICreature* ptarget_creature = GFDebugPDynamicCast(ptarget, (CThingAICreature*)0);
            if (!TargetCreatureLikelyToReactToDeed(phero, ptarget_creature, (EOpinionDeedType)ExtraData2))
                return false;
            if (ptc_inventory != 0 && inventory_item_def_index > 0) {
                if (ptc_inventory->GetCSOpinionExpressionCount((EContextSensitiveOpinionExpression)ExtraData1) > 1) {
                    if (ptc_inventory->GetLastOpinionExpression((EContextSensitiveOpinionExpression)ExtraData1) == inventory_item_def_index)
                        return false;
                    if (ptc_inventory->GetCSOpinionExpressionCount((EContextSensitiveOpinionExpression)ExtraData1) > 2 &&
                        ptc_inventory->GetLastButOneOpinionExpression((EContextSensitiveOpinionExpression)ExtraData1) == inventory_item_def_index)
                        return false;
                }
            }
            if (ExtraData1 == 4 || ExtraData1 == 5) {
                return true;
            } else if (ptarget_creature->GetCreatureType() != HUMAN_CHILD && (ExtraData1 == 2 || ExtraData1 == 3)) {
                return true;
            } else if (ptarget_creature->IsCourtable() && (ExtraData1 == 0 || ExtraData1 == 1)) {
                const CTCWife* ptc_wife;
                if (!(ExtraData1 == 1 && ptarget_creature->PeekTC(&ptc_wife) && !ptc_wife->IsCourtableAndNotBlockingIt()))
                    return true;
            }
        } else if (ExtraData2 > 0 && ExtraData2 != 0 && ExtraData2 < 0x5c) {
            const CTCHeroOpinionDeedLog* ptc_deedlog = 0;
            phero->PeekTC(&ptc_deedlog);
            if (ptc_deedlog && !ptc_deedlog->PeekPDeedEffectsDef((EOpinionDeedType)ExtraData2)->ReactionRestrictToTarget)
                return true;
        }
        break;
    }
    case CONTEXT_SENSITIVE_GIFT: {
        const CTCTargetingBase* ptc_targeting = 0;
        if (phero->PeekTC(&ptc_targeting)) {
            CThing* ptarget = ptc_targeting->GetPTargetedThing();
            if (ptarget && (ptarget->GetTypeFlags() & 0x10) && ptarget->HasTC(TCI_CREATURE_OPINION_OF_HERO) && !ptarget->HasTC(TCI_GUARD)) {
                CThingAICreature* ptarget_creature = GFDebugPDynamicCast(ptarget, (CThingAICreature*)0);
                if (!TargetCreatureLikelyToReactToDeed(phero,ptarget_creature,(EOpinionDeedType)ExtraData2))
                    return false;
                if (ptarget_creature->GetCreatureType()!=HUMAN_CHILD && (ExtraData1==2 || ExtraData1==0)) {
                    return true;
                } else if (ptarget_creature->IsCourtable() && ExtraData1==1) {
                    const CTCWife* ptc_wife;
                    if (ptarget_creature->PeekTC(&ptc_wife) && ptc_wife->IsCourtableAndNotBlockingIt()) {
        CDefPointer<const CThingBaseDef> pbase_def;
        if (CGameDefinitionManager::Get().GetDef(inventory_item_def_index, &pbase_def)) {
            CDefPointer<const CGiftDef> pgift_def;
            if (!(pbase_def->GetSubDef(&pgift_def) && pgift_def->IsWeddingRing) ||
                (ptc_wife->IsInLoveWithHero() && !ptc_wife->HasReceivedWeddingRing()))
                return true;
        }
                    }
                }
            }
        }
        break;
    }
    case CONTEXT_SENSITIVE_MARKER: {
        C2DBoxI search_area(phero->GetPos().ToC2DCoordI(), ExtraData2);
        switch (ExtraData1) {
        case 1:
            if (CTCCarriedActionUseFishingRod::IsThingAbleToFish(*phero)) return true;
            return false;
        case 2: {
            C2DBoxI area(phero->GetPos().ToC2DCoordI(), GFFloatToLongCeil(3.0f));
            std::vector<CThingMarker*> digging_spots;
            CMainGameComponent::Get()->PeekWorld().PeekThingSearchTools().GetAllThingsInRoughArea<CThingMarker>(
                area, CThingFilter_HasTC(TCI_DIGGING_SPOT), digging_spots);
            for (std::vector<CThingMarker*>::iterator it = digging_spots.begin(); it != digging_spots.end(); ++it) {
                CThingMarker* pmarker = *it;
                const CTCDiggingSpot* ptc_digging_spot;
                if (pmarker->PeekTC(&ptc_digging_spot) && ptc_digging_spot->IsEnabled() && !ptc_digging_spot->IsHidden()) {
                    if (GFGet2DDistanceSquared(C2DVector(phero->GetPos()), C2DVector(pmarker->GetPos())) < GFSqr(3.0f) &&
                        CTCCarriedActionUseSpade::IsThingAbleToDig(*phero))
                        return true;
                }
            }
            return false;
        }
        }
        return false;
    }
    case CONTEXT_SENSITIVE_TROPHY: {
        if (inventory_item_def_index <= 0)
            return false;
        CDefPointer<const CThingBaseDef> pinventory_item_base_def;
        CGameDefinitionManager::Get().GetDef(inventory_item_def_index, &pinventory_item_base_def);
        CDefPointer<const CTrophyDef> ptrophy_def;
        if (pinventory_item_base_def->GetSubDef(&ptrophy_def)) {
            const CTCCarrying* ptc_carrying;
            if (phero->PeekTC(&ptc_carrying)) {
                const CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
                const CTCTrophy* ptc_trophy;
                if (IS_THING_ALIVE(pcarried_thing) && pcarried_thing->GetDefGlobalIndex() == inventory_item_def_index &&
                    pcarried_thing->PeekTC(TCI_TROPHY, &ptc_trophy))
                    return ptc_trophy->IsMiniGameInProgress();
            }
        }
        break;
    }
    }
    return false;
}

int main(){
    CThingPlayerCreature hero;CThingAICreature npc; CTCTargetingBase targ={&npc};
    CContextSensitiveItemDef def={0,0,0}; CTCInventory inv; memset(&inv,0,sizeof(inv));
    CTCHeroStats stats={64,16}; CTCDoor door={true}; CTCStockItem stock={true};
    CTCFollowed followed={true,false};CTCGuard guard={true}; CTCCreatureOpinionOfHero opinion={false};
    CTCWife wife={true,true,false};CTCHeroOpinionDeedLog deeds={{false}};
    CThingBaseDef record;CGiftDef gift={true}; CTrophyDef trophy;record.gift=&gift;record.trophy=&trophy;
    CGameDefinitionManager::Get().def=&record;
    CTCDiggingSpot spot={true,false};CThingMarker ground;CThingSearchTools search;CWorld world={&search};
    CMainGameComponent game={&world};CMainGameComponent::PMainGameComponent=&game;
    CThing item;CTCCarrying carrying={&item};CTCTrophy held={true};
    int slot=7;
#define CHECK(want) do { if(def.IsValidOverride(hero,slot,&inv)!=(want)){printf("failed line %d\n",__LINE__);return 1;} }while(0)
    CHECK(false);def.Type=2;CHECK(true);hero.Dead=true;CHECK(false);hero.Dead=false;
    def.Type=3;def.ExtraData1=50;CHECK(true);def.ExtraData1=51;def.ExtraData2=1;CHECK(false);def.ExtraData2=50;CHECK(true);
    def.Type=4;CHECK(false);hero.tc[4]=&stats;def.ExtraData1=48;CHECK(true);def.ExtraData1=49;CHECK(false);
    def.Type=5;CHECK(false);hero.tc[8]=&targ;targ.target=0;CHECK(false);targ.target=&npc;hero.tc[0x84]=&npc;CHECK(false);hero.tc[0x84]=0;
    def.ExtraData1=1;npc.pick=true;CHECK(false);npc.ThingTypeFlags=8;CHECK(true);npc.pick=false;CHECK(false);
    def.ExtraData1=2;CHECK(false);npc.tc[0x26]=&door;CHECK(true);door.lock=false;CHECK(false);
    def.ExtraData1=3;CHECK(false);npc.tc[0x54]=&stock;CHECK(true);stock.Stealable=false;CHECK(false);
    def.ExtraData1=4;CHECK(false);npc.ThingTypeFlags=16;CHECK(false);npc.RespondingToFollowAndWait=true;CHECK(false);
    hero.tc[0xbc]=&followed;CHECK(true);followed.valid=false;CHECK(false);followed.valid=true;followed.following=true;CHECK(false);
    def.ExtraData1=5;CHECK(true);followed.following=false;CHECK(false);npc.RespondingToFollowAndWait=false;CHECK(false);
    def.ExtraData1=6;CHECK(false);npc.tc[0x4f]=&opinion;opinion.InFindAGuardAndYellForHelp=true;CHECK(true);
    opinion.InFindAGuardAndYellForHelp=false;npc.tc[0xb4]=&guard;CHECK(true);guard.criminal=false;CHECK(false);npc.tc[0xb4]=0;
    def.ExtraData1=7;CHECK(false);def.Type=6;def.ExtraData1=4;react=false;CHECK(false);react=true;CHECK(true);
    inv.NumberOfCSOpinionExpressions[4]=3;inv.LastCSOpinionExpression[4]=7;CHECK(false);
    inv.LastCSOpinionExpression[4]=6;inv.LastButOneCSOpinionExpression[4]=7;CHECK(false);inv.NumberOfCSOpinionExpressions[4]=2;CHECK(true);
    def.ExtraData1=2;npc.kind=2;CHECK(true);npc.kind=1;CHECK(false);def.ExtraData1=0;npc.CanBeCourted=true;CHECK(true);
    def.ExtraData1=1;npc.tc[0xd7]=&wife;wife.court=false;CHECK(false);wife.court=true;CHECK(true);
    targ.target=0;def.ExtraData2=0;CHECK(false);def.ExtraData2=0x5c;CHECK(false);def.ExtraData2=3;CHECK(false);hero.tc[0x51]=&deeds;CHECK(true);deeds.d.ReactionRestrictToTarget=true;CHECK(false);
    def.Type=7;CHECK(false);targ.target=&npc;react=false;CHECK(false);react=true;CHECK(true);wife.love=false;CHECK(false);
    wife.love=true;wife.ring=true;CHECK(false);gift.IsWeddingRing=false;CHECK(true);CGameDefinitionManager::Get().def=0;CHECK(false);
    CGameDefinitionManager::Get().def=&record;npc.kind=2;def.ExtraData1=2;CHECK(true);
    def.Type=8;def.ExtraData1=1;CHECK(false);fish=true;CHECK(true);def.ExtraData1=2;CHECK(false);search.found.push_back(&ground);
    ground.tc[0xef]=&spot;ground.pos.X=3;dig=true;CHECK(false);ground.pos.X=2;CHECK(true);spot.hidden=true;CHECK(false);spot.hidden=false;spot.enabled=false;CHECK(false);
    def.Type=9;slot=0;CHECK(false);slot=7;CHECK(false);hero.tc[0x46]=&carrying;item.DefGlobalIndex=6;CHECK(false);
    item.DefGlobalIndex=7;CHECK(false);item.tc[0x39]=&held;CHECK(true);item.Dead=true;CHECK(false);record.trophy=0;CHECK(false);
    puts("00850a80_TEST PASS");return 0;
}
