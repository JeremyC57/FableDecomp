#ifndef FABLE_ENGINE_CTHINGCOMPARE_BESTCREATURETOATTACK_H
#define FABLE_ENGINE_CTHINGCOMPARE_BESTCREATURETOATTACK_H
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
class CThing;class CThingCreatureBase;
class CThingCompare_BestCreatureToAttack {
public:
 CThingCompare_BestCreatureToAttack(CThingCreatureBase const & p0, CThing const * p1, float p2, float p3, float p4, float p5):Targeter(p0){PCurrentBestTargetForAttack=p1;PreferPlayerWeight=p2;PreferNonCombatantsWeight=p3;PreferLastAttackerWeight=p4;PreferCurrentBestEnemyWeight=p5;}
 template<class T> T* operator()(T*,T*)const;
private:
 CThingCreatureBase const & Targeter;
 CThing const * PCurrentBestTargetForAttack;
 float PreferPlayerWeight;
 float PreferNonCombatantsWeight;
 float PreferLastAttackerWeight;
 float PreferCurrentBestEnemyWeight;
};
#endif
