#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct TRetVoid { void Return() {} };
typedef const char* CCharString;
static long GFCharStringToInt(CCharString s) { return atol(s); }
struct C3DVector { float X,Y,Z; C3DVector(float x=0):X(x),Y(0),Z(0){} };
static int frame;
static int stop_check;
static int checks;
static int event_count;
static char events[512];
static void record(char event) {
    if(event_count >= 510) exit(2);
    events[event_count++] = event;
    events[event_count] = 0;
}
struct CScriptThing {
    int id,expires;
    CScriptThing(int n=0,int end=99):id(n),expires(end){}
    CCharString GetDataString() const { return "2"; }
    C3DVector GetPos() const { return C3DVector((float)id); }
    bool IsAlive() const { return id!=0 && frame<expires; }
};
struct ScriptDef { long EXPERIENCE_GRANT_SMALL; } def={37};
static ScriptDef* pGlobalScriptDef=&def;
struct GameStub {
    int possess_at,stop_at,markers,removed_markers,effects,removed_effects,experience,spawns,targets,errors;
    GameStub():possess_at(3),stop_at(5),markers(0),removed_markers(0),effects(0),removed_effects(0),experience(0),spawns(0),targets(0),errors(0){}
    CScriptThing GetHero() { record('H'); return CScriptThing(42); }
    bool IsObjectInThingsPossession(CCharString key,const CScriptThing& h) {
        record('P'); if(strcmp(key,"shield") || h.id!=42) ++errors;
        return frame>=possess_at;
    }
    void NewScriptFrame() { record('F'); ++frame; }
    void MiniMapAddMarker(const CScriptThing& me,CCharString marker) { record('M'); if(me.id!=7 || strcmp(marker,"HUD_ORB_GREEN_SMALL")) ++errors; ++markers; }
    CScriptThing CreateEffect(CCharString key,C3DVector pos,CCharString script,float seconds,bool b,bool c) {
        record('E'); if(strcmp(key,"GRAVEYARD_SPARKLES_GRAVE") || pos.X!=7 || *script || seconds!=0 || b || !c) ++errors;
        ++effects; return CScriptThing(70+effects,effects==1?2:90);
    }
    void GiveHeroExperience(long n) { record('X'); experience+=n; }
    void MiniMapRemoveMarker(const CScriptThing& me) { record('R'); if(me.id!=7) ++errors; ++removed_markers; }
    void RemoveThing(const CScriptThing& thing,bool b,bool c) { record('D'); if(thing.id!=70+effects || b || !c) ++errors; ++removed_effects; }
    CScriptThing GetNearestWithScriptName(const CScriptThing& me,CCharString key) {
        record('N'); if(me.id!=7 || strcmp(key,"SecretPassageUndeadSpawn")) ++errors;
        return CScriptThing(101);
    }
    CScriptThing CreateCreature(CCharString key,C3DVector pos,CCharString script,bool enable) {
        record('G'); if(strcmp(key,"CREATURE_OK_UNDEAD_LIEUTENANT_AXE_LOW_DETAIL") || pos.X!=101 || *script || !enable) ++errors;
        ++spawns; return CScriptThing(102);
    }
    void GiveThingBestEnemyTarget(const CScriptThing& a,const CScriptThing& b) { record('T'); if(a.id!=102 || b.id!=42) ++errors; ++targets; }
};
namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript {
public:
    bool skeleton_visited,doors_open;
    CCharString hero_object[4];
    CQ_OpeningGraveyardSecretPassageScript(bool visit,bool doors):skeleton_visited(visit),doors_open(doors) {
        hero_object[0]="wrong zero";hero_object[1]="shield";hero_object[2]="wrong two";hero_object[3]="wrong three";
    }
    class CCeremonialEffect {
    public:
        CQ_OpeningGraveyardSecretPassageScript& ParentClass;
        CScriptThing Me;
        GameStub* Game;
        CCeremonialEffect(CQ_OpeningGraveyardSecretPassageScript& p,GameStub* g):ParentClass(p),Me(7),Game(g){}
        bool IsActiveThreadTerminating() const { record('?'); ++checks; return checks==stop_check || frame>=Game->stop_at; }
        void Main();
    };
};
}
void NScript::CQ_OpeningGraveyardSecretPassageScript::CCeremonialEffect::Main()

{
    TRetVoid _Fail_Val;
    bool Activated = false;
    CScriptThing particle_effect;
    long Index = GFCharStringToInt(Me.GetDataString()) - 1;
    while (!Game->IsObjectInThingsPossession(ParentClass.hero_object[Index], Game->GetHero())) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            { _Fail_Val.Return(); return; }
        if (!Activated) {
            if (IsActiveThreadTerminating())
                { _Fail_Val.Return(); return; }
            if (ParentClass.skeleton_visited) {
                if (IsActiveThreadTerminating())
                    { _Fail_Val.Return(); return; }
                Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
                particle_effect = Game->CreateEffect("GRAVEYARD_SPARKLES_GRAVE", Me.GetPos(), "", 0.0f, false, true);
                Activated = true;
            }
        } else {
            if (IsActiveThreadTerminating())
                { _Fail_Val.Return(); return; }
            if (!particle_effect.IsAlive()) {
                if (IsActiveThreadTerminating())
                    { _Fail_Val.Return(); return; }
                particle_effect = Game->CreateEffect("GRAVEYARD_SPARKLES_GRAVE", Me.GetPos(), "", 0.0f, false, true);
            }
        }
    }
    if (IsActiveThreadTerminating())
        { _Fail_Val.Return(); return; }
    Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
    Game->MiniMapRemoveMarker(Me);
    if (particle_effect.IsAlive()) {
        if (IsActiveThreadTerminating())
            { _Fail_Val.Return(); return; }
        Game->RemoveThing(particle_effect, false, true);
    }
    if (ParentClass.doors_open) {
        if (IsActiveThreadTerminating())
            { _Fail_Val.Return(); return; }
        CScriptThing spawn_point = Game->GetNearestWithScriptName(Me, "SecretPassageUndeadSpawn");
        CScriptThing particle_effect;
        CScriptThing zombie=Game->CreateCreature("CREATURE_OK_UNDEAD_LIEUTENANT_AXE_LOW_DETAIL", spawn_point.GetPos(), "", true);
        Game->GiveThingBestEnemyTarget(zombie,Game->GetHero());
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            { _Fail_Val.Return(); return; }
    }
}
static bool run(int which) {
    frame=0; checks=0; stop_check=0; event_count=0; events[0]=0;
    GameStub game;
    NScript::CQ_OpeningGraveyardSecretPassageScript parent(which!=2,which==0);
    NScript::CQ_OpeningGraveyardSecretPassageScript::CCeremonialEffect script(parent,&game);
    if(which==1) game.stop_at=1;
    if(which==3) game.possess_at=0;
    script.Main();
    if(game.errors || frame!=game.stop_at) return false;
    if(which==1) return !game.markers && !game.effects && !game.experience && !game.removed_markers && !game.spawns;
    bool active=which==0;
    return game.markers==(active?1:0) && game.effects==(active?2:0) && game.removed_effects==(active?1:0)
        && game.experience==37 && game.removed_markers==1 && game.spawns==(active?1:0) && game.targets==game.spawns;
}
// H hero, P possession, F frame, M add marker, E create effect, X grant
// experience, R remove marker, D delete effect, N nearest, G spawn, T target.
static bool verify_prefixes(bool visited,bool doors,int possession,const char* expected) {
    const int length=(int)strlen(expected);
    int ordinal=0;
    for(int i=0;i<length;++i) {
        if(expected[i]!='?') continue;
        frame=0; checks=0; stop_check=++ordinal; event_count=0; events[0]=0;
        GameStub game;
        game.stop_at=100;
        game.possess_at=possession;
        NScript::CQ_OpeningGraveyardSecretPassageScript parent(visited,doors);
        NScript::CQ_OpeningGraveyardSecretPassageScript::CCeremonialEffect script(parent,&game);
        script.Main();
        if(game.errors || event_count!=i+1 || strncmp(events,expected,i+1)!=0 || checks!=ordinal) {
            printf("ordinal %d trace %s expected %.*s\n",ordinal,events,i+1,expected);
            return false;
        }
    }
    return true;
}
int main() {
    // Activation on frame one, effect expiry on frame two, active on frame
    // three, award, cleanup, open-door spawn and two idle-loop iterations.
    if(!verify_prefixes(true,true,3,"HPF???MEHPF???EHPF??HP?XR?D?NGHTF?F?")) return 2;
    // Never activated, no live effect to remove, closed door.
    if(!verify_prefixes(false,false,2,"HPF??HPF??HP?XRF?F?")) return 3;
    // Already possessed at entry, open door and no effect.
    if(!verify_prefixes(false,true,0,"HP?XR?NGHTF?F?")) return 4;
    for(int i=0;i<4;++i) if(!run(i)) return 1;
    puts("00dc4600_TEST PASS");
    return 0;
}