#ifndef FABLE_ENGINE_TCD_EXPLOSION_FILTERS_H
#define FABLE_ENGINE_TCD_EXPLOSION_FILTERS_H
#ifdef FABLE_RETAIL_EXPLOSION_HITS_PASS29
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCDEXPLOSION_DYNAMIC_HITS_PASS30
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#endif
namespace {
#ifdef FABLE_RETAIL_CTCDEXPLOSION_DYNAMIC_HITS_PASS30
// Both original debug databases place this filter in tcd_explosion.obj.
class CThingFilter_IsAffectedByExplosions {
public:
    // The two native users call the same 82-byte filter at 0080E480.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    bool operator()(const CThing& thing) const
    {
        const CTCCreatureModeManager* mode_manager;
        if (thing.PeekTC(&mode_manager) &&
            mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_HIT_BY_EPIC_SPELL)
            return false;
        return true;
    }
};
#endif
class CGetThingFromIntelligentPointer {
public:
    CThing* operator()(const CIntelligentPointer<CThing>& p){return p.Get();}
};
class CThingFilter_IsEvadeExplosion:public std::unary_function<CThing,bool>{
    const C3DVector& StartPos;
    const C3DVector& EndPos;
public:
    CThingFilter_IsEvadeExplosion(const C3DVector&a,const C3DVector&b):StartPos(a),EndPos(b){}
    bool operator()(const CThing&)const;
};}
#endif
#endif
