#ifndef FABLE_ENGINE_TCD_EXPLOSION_FILTERS_H
#define FABLE_ENGINE_TCD_EXPLOSION_FILTERS_H
#ifdef FABLE_RETAIL_EXPLOSION_HITS_PASS29
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
namespace {
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
