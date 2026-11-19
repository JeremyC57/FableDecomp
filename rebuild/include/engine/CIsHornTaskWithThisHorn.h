#ifndef FABLE_ENGINE_CISHORNTASKWITHTHISHORN_H
#define FABLE_ENGINE_CISHORNTASKWITHTHISHORN_H
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CVillageTaskHorn.h"
#include "stlport/fable_containers.h"
// Layout: complete original Ego38ddd/FW3cff6,size8, genuine unary-function base.
class CIsHornTaskWithThisHorn:public std::unary_function<CCountedPointer<CVillageTaskHorn>,bool>{
public:
#ifdef FABLE_RETAIL_CISHORNTASKWITHTHISHORN_CTOR_OUTOFLINE
 // Opt-in (NF95 q11276, row 0x0065F0FA): FableWin.pdb 0x02160C17 ??0CIsHornTaskWithThisHorn@@QAE@V?$CIntelligentPointer@VCThing@@@@@Z is
 // out of line in retail at 0x0065F0FA (called from CTCVillage::UpdateHornTasks 0x0065BF03). Declaration only; the row defines it.
 CIsHornTaskWithThisHorn(CIntelligentPointer<CThing> p0);
#else
 CIsHornTaskWithThisHorn(CIntelligentPointer<CThing> p0):PHorn(p0){}
#endif
 bool operator()(CCountedPointer<CVillageTaskHorn> p0) const{return p0->GetHorn()==PHorn.Get();}
private:
 CIntelligentPointer<CThing> PHorn;
public:
 ~CIsHornTaskWithThisHorn(){}
 CIsHornTaskWithThisHorn & operator=(const CIsHornTaskWithThisHorn & p0);
};
FABLE_STATIC_ASSERT_AT(18, sizeof(CIsHornTaskWithThisHorn)==8);
#endif
