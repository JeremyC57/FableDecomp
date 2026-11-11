#ifndef FABLE_ENGINE_CISHORNTASKWITHTHISHORN_H
#define FABLE_ENGINE_CISHORNTASKWITHTHISHORN_H
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CVillageTaskHorn.h"
#include "stlport/fable_containers.h"
// Layout: complete original Ego38ddd/FW3cff6,size8, genuine unary-function base.
class CIsHornTaskWithThisHorn:public std::unary_function<CCountedPointer<CVillageTaskHorn>,bool>{
public:
 CIsHornTaskWithThisHorn(CIntelligentPointer<CThing> p0):PHorn(p0){}
 bool operator()(CCountedPointer<CVillageTaskHorn> p0) const{return p0->GetHorn()==PHorn.Get();}
private:
 CIntelligentPointer<CThing> PHorn;
public:
 ~CIsHornTaskWithThisHorn(){}
 CIsHornTaskWithThisHorn & operator=(const CIsHornTaskWithThisHorn & p0);
};
FABLE_STATIC_ASSERT(sizeof(CIsHornTaskWithThisHorn)==8);
#endif
