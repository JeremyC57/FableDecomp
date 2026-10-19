// CUserProfileManager::GetConfigOptionsDefaultsDef()const -- obtain configuration-option defaults from the active runtime definition manager.
// Retail004083E0 length194. Runtime singleton probes13B86A0/13B871C; getters44C6B0/43368D.
#define CManager CManagerDonor
#include "engine/CManager.h"
#undef CManager
#define CDefinitionManager CDefinitionManagerDonor
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CIVCountedPointeeBase CIVCountedPointeeBaseDonor
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CCharString CCharStringDonor
#include "fable_string.h"
#undef CCharString
struct CCharString { CCharStringData* Data; CCharString(const char*,long); CCharString(const CCharString&); ~CCharString(); };
struct CIVCountedPointeeBase {
 virtual ~CIVCountedPointeeBase();
 virtual void Delete() const;
 unsigned long IVRefCount;
 void AddReference(){++IVRefCount;}
 void DecRef(){--IVRefCount;}
 void RemoveReference(){DecRef();if(GetRefCount()==0)Delete();}
 long GetRefCount()const{return IVRefCount;}
};
template<class T> struct CDefPointer {
 CIVCountedPointeeBase* PObject;
 CDefPointer():PObject(0){}
 CDefPointer(const CDefPointer& p):PObject(p.PObject){if(PObject)PObject->AddReference();}
 ~CDefPointer(){if(PObject)PObject->RemoveReference();}
 CDefPointer& operator=(const CDefPointer& p){if(PObject!=p.PObject){if(PObject){PObject->RemoveReference();PObject=0;}PObject=p.PObject;if(PObject)PObject->AddReference();}return *this;}
};
struct CDefClassBase;
#include "engine/_quarantine/CConfigOptionsDefaultsDef.h"
struct CDefinitionManager {
 CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long); //009AD9E0
 long GetGlobalIndexFromClassIndex(const CCharString&,long)const; //009AD390
 bool GetDef(long,CDefPointer<const CConfigOptionsDefaultsDef>*); //00410890 native mutable helper projection
};
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long index,CDefPointer<const CConfigOptionsDefaultsDef>* out){
 if(index>0){CDefPointer<CDefClassBase> value=GetPDefFromGlobalIndex(index);if(value.PObject){
  if(out->PObject!=value.PObject){if(out->PObject){out->PObject->RemoveReference();out->PObject=0;}out->PObject=value.PObject;if(out->PObject)out->PObject->AddReference();}return true;
 }}return false;
}
#pragma auto_inline(on)
extern void* g_pMainGameComponent; //013B86A0 (only presence is inspected)
extern void* g_pFrontendRuntime; //013B871C (only presence is inspected)
CDefinitionManager* __fastcall GetGameDefinitionManager(); //0044C6B0 loads013B879C
CDefinitionManager* __fastcall GetFrontendDefinitionManager(); //0043368D loads013B8760
struct CUserProfileManager {CDefPointer<const CConfigOptionsDefaultsDef> GetConfigOptionsDefaultsDef()const;};
CDefPointer<const CConfigOptionsDefaultsDef> CUserProfileManager::GetConfigOptionsDefaultsDef()const{
 CDefPointer<const CConfigOptionsDefaultsDef> result;
 if(g_pMainGameComponent){
  long index;{CCharString name("CONFIG_OPTIONS_DEFAULTS_DEF",-1);index=GetGameDefinitionManager()->GetGlobalIndexFromClassIndex(name,1);}
  GetGameDefinitionManager()->GetDef(index,&result);
 }else if(g_pFrontendRuntime){
  long index;{CCharString name("CONFIG_OPTIONS_DEFAULTS_DEF",-1);index=GetFrontendDefinitionManager()->GetGlobalIndexFromClassIndex(name,1);}
  GetFrontendDefinitionManager()->GetDef(index,&result);
 }
 return result;
}
