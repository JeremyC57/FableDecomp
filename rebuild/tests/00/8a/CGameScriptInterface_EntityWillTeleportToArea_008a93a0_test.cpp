// Exercise the complete production method with deterministic search, navigation and action dependencies.
#include <stdio.h>
#include <vector>
#include <functional>
#include <algorithm>
struct C3DVector {int X,Y,Z;C3DVector(int x=0,int y=0,int z=0):X(x),Y(y),Z(z){}};
class CVectorFilter_FurtherFromPointThan: public std::unary_function<C3DVector,bool> {
    C3DVector centre;float radius;
public: CVectorFilter_FurtherFromPointThan(const C3DVector& c,float r):centre(c),radius(r){}
    bool operator()(const C3DVector& p)const{int d=p.X-centre.X;return d*d>radius*radius;}
};
struct CWorld {unsigned long seed;int queries;int GetWorldMap()const{return 3;}int PeekThingSearchTools()const{return 5;}
    unsigned long* GetWorldSeed(){++queries;return &seed;}};
namespace NCreatureHiding {
    static std::vector<C3DVector> options;static int queries;
    void GetValidPositions(std::vector<C3DVector>& out,int map,int search,const C3DVector& centre,float limit,bool flag,int shape){
        if(map==3&&search==5&&centre.X==1&&limit==12.0f&&!flag&&!shape)++queries;out=options;
    }
}
static unsigned long GFRandom(unsigned long n,unsigned long* seed){return (*seed)++%n;}
struct CTCCreatureNavigation {
    int accessible,tries; bool IsAbleToNavigateFromPosToPos(const C3DVector& from,const C3DVector& to,float radius) {
        ++tries;return from.X==17&&radius==0.0f&&to.X==accessible;
    }
};
struct CThing {unsigned long bits;unsigned long GetTypeFlags()const{return bits;}};
struct CThingAICreature;
struct CCreatureAction_TeleportOut {
    CThingAICreature* owner; C3DVector position;bool flag;
    CCreatureAction_TeleportOut(CThingAICreature& a,C3DVector& p,bool b):owner(&a),position(p),flag(b){}
};
struct CThingAICreature: public CThing {
    CTCCreatureNavigation nav;int actions,copies;bool accepted;C3DVector target;
    C3DVector GetPos()const{return C3DVector(17);}
    void GetTC(CTCCreatureNavigation** ptr){++copies;*ptr=&nav;}
    bool SetCurrentAction(const CCreatureAction_TeleportOut& action){if(action.owner==this&&!action.flag)++actions;target=action.position;return accepted;}
};
struct CScriptThing {CThing* thing;bool alive;bool IsAlive()const{return alive;}CThing* GetPThing()const{return thing;}};
struct CGameScriptInterface {CWorld* World;bool EntityWillTeleportToArea(const CScriptThing&,C3DVector,float,float)const;};

bool CGameScriptInterface::EntityWillTeleportToArea(const CScriptThing& entity, C3DVector centre_pos,
                                                   float min_distance, float max_distance) const
{
    if (entity.IsAlive()) {
        CThing* pthing = entity.GetPThing();
        if (pthing->GetTypeFlags() & 0x10) {
            CThingAICreature* pcreature = static_cast<CThingAICreature*>(pthing);
            C3DVector teleport_pos = centre_pos;
            std::vector<C3DVector> validPositions;
            NCreatureHiding::GetValidPositions(validPositions, World->GetWorldMap(), World->PeekThingSearchTools(),
                                               centre_pos, max_distance, false, 0);
            validPositions.erase(std::remove_if(validPositions.begin(), validPositions.end(),
                std::not1(CVectorFilter_FurtherFromPointThan(centre_pos, min_distance))), validPositions.end());
            long num_positions = validPositions.size();
            if (num_positions == 0)
                return false;
            else {
                long n; // The debug scope records this local; the debug procedure never reads or writes it.
                bool found = false;
                CTCCreatureNavigation* ptc_nav;
                pcreature->GetTC(&ptc_nav);
                do {
                    long random_index = GFRandom(validPositions.size(), World->GetWorldSeed());
                    if (ptc_nav->IsAbleToNavigateFromPosToPos(pcreature->GetPos(), validPositions[random_index], 0.0f)) {
                        teleport_pos = validPositions[random_index];
                        found = true;
                        break;
                    }
                    validPositions.erase(validPositions.begin() + random_index);
                } while (validPositions.size() > 0);
                if (!found) return false;
                return pcreature->SetCurrentAction(CCreatureAction_TeleportOut(*pcreature, teleport_pos, false));
            }
        }
    }
    return false;
}

int main(){
    for(int mode=0;mode<8;++mode)for(int accept=0;accept<2;++accept)for(int seed=0;seed<4;++seed){
        CWorld world;world.seed=seed;world.queries=0;CGameScriptInterface api;api.World=&world;
        CThingAICreature creature;creature.bits=mode==1?0:16;creature.actions=creature.copies=creature.nav.tries=0;
        creature.nav.accessible=mode==6?4:mode==7?7:-1;creature.accepted=accept!=0;
        CScriptThing entity;entity.alive=mode!=0;entity.thing=&creature;
        NCreatureHiding::options.clear();NCreatureHiding::queries=0;
        if(mode>=3)NCreatureHiding::options.push_back(C3DVector(3));
        if(mode>=4)NCreatureHiding::options.push_back(C3DVector(4,5,6));
        if(mode>=5)NCreatureHiding::options.push_back(C3DVector(7,8,9));
        bool value=api.EntityWillTeleportToArea(entity,C3DVector(1),2.0f,12.0f);
        if(value!=(mode>=6&&accept!=0)||NCreatureHiding::queries!=(mode>=2))return 1;
        if(creature.copies!=(mode>=4)||creature.actions!=(mode>=6))return 2;
        if(creature.nav.tries!=world.queries)return 3;
        if(mode<4&&creature.nav.tries)return 4;
        if(mode==4&&creature.nav.tries!=1)return 5;
        if(mode==5&&creature.nav.tries!=2)return 6;
        if(mode>=6&&(creature.nav.tries<1||creature.nav.tries>2||creature.target.X!=creature.nav.accessible||
            creature.target.Y!=creature.target.X+1||creature.target.Z!=creature.target.X+2))return 7;
    }
    puts("008a93a0_TEST PASS");return 0;
}