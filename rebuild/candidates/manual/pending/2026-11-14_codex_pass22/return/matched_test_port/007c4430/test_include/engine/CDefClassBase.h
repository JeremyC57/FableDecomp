#ifndef FABLE_ENGINE_CDEFCLASSBASE_H
#define FABLE_ENGINE_CDEFCLASSBASE_H
#include "engine/CDefPointeeBase.h"
#include "engine/CDefPointer.h"
#include "fable_string.h"
class CDefinitionManager;
class CDefClassBase:public CDefPointeeBase {public:class CSubDefInfo{public:long DefIndex,OriginalParentDefIndex;};CDefinitionManager*PDefManager;bool Available;CSubDefInfo Info;CDefClassBase():PDefManager(0),Available(true){Info.DefIndex=19;Info.OriginalParentDefIndex=0;}virtual const CSubDefInfo*GetPSubDefInfo(const CCharString&n)const{assert(strcmp(n.Text,"CSpecialEffectsDef")==0);return Available?&Info:0;}template<class T>bool GetSubDef(CDefPointer<const T>*)const;};
#endif
