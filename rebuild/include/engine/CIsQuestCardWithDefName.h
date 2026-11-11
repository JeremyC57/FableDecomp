#ifndef FABLE_ENGINE_CISQUESTCARDWITHDEFNAME_H
#define FABLE_ENGINE_CISQUESTCARDWITHDEFNAME_H
#include "stlport/fable_containers.h"
#include "engine/CQuestManager.h"
// Layout: original Ego52e12,4bytes; donor01c57100/01c6e720 stores reference and compares card.DefName.
class CIsQuestCardWithDefName:public std::unary_function<CQuestManager::CQuestCard,bool> {
public:
 CIsQuestCardWithDefName(const CCharString& name):DefName(name){}
 bool operator()(const CQuestManager::CQuestCard& card)const{return card.DefName==DefName;}
private:const CCharString& DefName;
};
#endif
