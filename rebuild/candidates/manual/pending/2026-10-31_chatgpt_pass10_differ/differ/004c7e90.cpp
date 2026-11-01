// 004c7e90 CThing::IsUnderScriptSupervision
// Leaf: ?IsUnderScriptSupervision@CThing@@QBE_NXZ
// Flags:
#define CThing CThingLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CQuestManager.h"
// Layout: real const method on imported CThing fields; no added data or virtual slot.
class CThing:public CThingLayout{public:bool IsUnderScriptSupervision()const;};
class CGameScriptThingManager{public:bool HasScriptThing(const CThing&);CScriptThing CreateScriptThing(const CThing&);};
template<class T>class CTBaseSingletonNoDefaultAlloc{public:static T&__fastcall Get(){return *PClass;}private:static CCountedPointer<T>PClass;};
__forceinline CScriptThing::~CScriptThing(){}
bool CThing::IsUnderScriptSupervision()const{
 bool supervised=CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PGameScriptThingManager_Data->HasScriptThing(*this)
 && CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PGameScriptThingManager_Data->CreateScriptThing(*this).GetScriptCounter()>0;
 return supervised;
}
