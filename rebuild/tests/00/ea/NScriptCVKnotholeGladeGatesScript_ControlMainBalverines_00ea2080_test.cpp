// Behaviour test for NScript::CV_KnotholeGladeGatesScript::ControlMainBalverines (0x00EA2080): one round of the main attack --
// pick a random villager outside (the hero if none is alive), wait until the KGG_Balv1 marker is off screen, spawn one balverine
// there (horn + persistent + best enemy target; the opinion source only for a villager target), then wait until it is dead.
#include <cstdio>
#include "engine/TRetVoid.h"
#include <cstring>
#include <vector>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct C3DVector { float x, y, z; };
struct CScriptThing { int id; bool alive; bool IsAlive() const { return alive; } C3DVector GetPos() const { C3DVector p = { (float)id, 0, 0 }; return p; } };
struct CGame {
    bool villagerAlive; int frames; int offScreenAfter; int deadAfter;
    CScriptThing GetThingWithScriptName(const char*) { CScriptThing t = { 1, true }; return t; }
    CScriptThing GetRandomThingWithScriptName(const char*) { CScriptThing t = { 2, villagerAlive }; return t; }
    CScriptThing GetHero() { CScriptThing t = { 3, true }; return t; }
    void NewScriptFrame() { ++frames; }
    bool IsCameraPosOnScreen(C3DVector p) { return p.x == 1.0f && frames < offScreenAfter; }
    CScriptThing CreateCreatureNearby(const char* def, C3DVector p, float r, const char* name, bool) {
        Log("create"); Log(def); Log(name); if (p.x != 1.0f || r != 6.0f) Log("badpos"); CScriptThing t = { 9, true }; return t; }
    void PlayCriteriaSoundOnThing(CScriptThing g, const char* s) { Log(g.id == 7 ? "horn" : "badgate"); Log(s); }
    void SetThingPersistent(CScriptThing, bool b) { Log(b ? "persist" : "nopersist"); }
    void GiveThingBestEnemyTarget(CScriptThing, CScriptThing t) { Log("target"); Log(t.id == 2 ? "villager" : t.id == 3 ? "hero" : "?"); }
    void EntitySetAsOpinionSource(CScriptThing, const char* s) { Log("opinion"); Log(s); }
};
struct CScript {
    CGame& Game; CScript(CGame& game):Game(game){} CScriptThing TheGate; int termAfter, termCalls;
    bool IsActiveThreadTerminating() { return ++termCalls > termAfter; }
    bool AreAllThingsInVectorDead(const std::vector<CScriptThing>& v) { return v.empty() || Game.frames >= Game.deadAfter; }
void ControlMainBalverines()
{
    TRetVoid _Fail_Val;
    std::vector<CScriptThing> KGG_Balverines;
    CScriptThing b2m = Game.GetThingWithScriptName("KGG_Balv1");
    CScriptThing Target;
    if (false) {} else for (long i = 0; i < 1; ++i) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Target = Game.GetRandomThingWithScriptName("KGG_VillagerOutside");
        if (Target.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (Game.IsCameraPosOnScreen(b2m.GetPos())) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing BalverineToAdd = Game.CreateCreatureNearby("CREATURE_BALVERINE_01", b2m.GetPos(), 6.0f, "KGG_Balverine", false);
            Game.PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
            Game.SetThingPersistent(BalverineToAdd, true);
            Game.GiveThingBestEnemyTarget(BalverineToAdd, Target);
            Game.EntitySetAsOpinionSource(BalverineToAdd, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
            KGG_Balverines.push_back(BalverineToAdd);
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Target = Game.GetHero();
            while (Game.IsCameraPosOnScreen(b2m.GetPos())) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing BalverineToAdd = Game.CreateCreatureNearby("CREATURE_BALVERINE_01", b2m.GetPos(), 6.0f, "KGG_Balverine", false);
            Game.PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
            Game.SetThingPersistent(BalverineToAdd, true);
            Game.GiveThingBestEnemyTarget(BalverineToAdd, Target);
            KGG_Balverines.push_back(BalverineToAdd);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!AreAllThingsInVectorDead(KGG_Balverines)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
};
int main(){
 for(int alive=0;alive<2;++alive)for(int camera=0;camera<5;++camera)
 for(int death=0;death<8;++death)for(int stop=0;stop<18;++stop){
  CGame g;g.villagerAlive=alive!=0;g.frames=0;g.offScreenAfter=camera;g.deadAfter=death;
  CScript s(g);s.TheGate.id=7;s.TheGate.alive=true;s.termAfter=stop;s.termCalls=0;
  g_log[0]=0;s.ControlMainBalverines();
  const bool spawned=stop>=camera+3;
  const char* expected=spawned?(alive?"create;CREATURE_BALVERINE_01;KGG_Balverine;horn;KNG_HORN;persist;target;villager;opinion;OPINION_SOURCE_INANIMATE_EVIL_HIGH;":"create;CREATURE_BALVERINE_01;KGG_Balverine;horn;KNG_HORN;persist;target;hero;"):"";
  int frames=stop>1?stop-1:0;if(frames>camera)frames=camera;
  if(spawned && stop>=camera+4){int extra=stop-camera-3;int cap=death-camera;if(cap<0)cap=0;if(extra>cap)extra=cap;frames+=extra;}
  if(std::strcmp(expected,g_log)||frames!=g.frames){printf("mismatch %d %d %d %d: %d vs %d\n",alive,camera,death,stop,frames,g.frames);return 1;}
 }
 std::puts("00ea2080_TEST PASS");return 0;
}