// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_CTHINGCOMPARE_BESTFORTARGETING2_H
#define FABLE_ENGINE_CTHINGCOMPARE_BESTFORTARGETING2_H
// Retail opt-in (Codex pass 17): original class, both PDB signatures and raw type records.
#include "engine/NPlayerTargeting.h"
#include "engine/CVectorCompare_BestForTargeting.h"
class CThingCompare_BestForTargeting2 {
public:
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
 // Original copy constructor is compiler-generated (__that).
 CThingCompare_BestForTargeting2(const CThing& hero,const C3DVector& pos,const C3DVector& line,float enemy,float angle,float dist,float short_factor,float short_falloff):Hero(hero),VectorCompare(pos,line,angle,dist,short_factor,short_falloff),EnemyPreferenceFactor(enemy){}
 void SetAsTargetCompareInXYPlane(){VectorCompare.SetAsTargetCompareInXYPlane();}
#endif
 bool operator()(const NPlayerTargeting::CTargetPoint&,const NPlayerTargeting::CTargetPoint&) const;
private:
 float GetThingCompareMultiplier(const CThing*) const;
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
 const CThing& Hero;
#else
 CThing* Hero;
#endif
 CVectorCompare_BestForTargeting VectorCompare;
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
 const float EnemyPreferenceFactor;
#else
 float EnemyPreferenceFactor;
#endif
};
#endif
