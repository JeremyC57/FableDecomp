// NUISystem::CManager::GetMiscThingsDef() -- retrieve UI definition through supplied or global manager.
// Retail0041d8e7 length163. String ctor99EBF0/dtor99EAE0; class-array index9AD390; global44C6B0.
// Refcount base native destructor slot0/Delete const slot1; template erases pointee to this base.
#pragma optimize("s", on)
#pragma optimize("y", off)
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
namespace NUISystem { struct CUIMiscThingsDef; struct CUIIconsDef; }
struct CDefinitionManager {
 CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long); //009ad9e0
 long GetGlobalIndexFromClassIndex(const CCharString&,long)const; //009ad390
 bool GetDef(long,CDefPointer<NUISystem::CUIMiscThingsDef>*); //0042b312
 bool GetDef(long,CDefPointer<NUISystem::CUIIconsDef>*); //0042b374
};
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long index,CDefPointer<NUISystem::CUIMiscThingsDef>* out){
 if(index>0){ CDefPointer<CDefClassBase> value=GetPDefFromGlobalIndex(index); if(value.PObject){
  if(out->PObject!=value.PObject){if(out->PObject){out->PObject->RemoveReference();out->PObject=0;}out->PObject=value.PObject;if(out->PObject)out->PObject->AddReference();}return true;
 }}return false;
}
bool CDefinitionManager::GetDef(long index,CDefPointer<NUISystem::CUIIconsDef>* out){
 if(index>0){ CDefPointer<CDefClassBase> value=GetPDefFromGlobalIndex(index); if(value.PObject){
  if(out->PObject!=value.PObject){if(out->PObject){out->PObject->RemoveReference();out->PObject=0;}out->PObject=value.PObject;if(out->PObject)out->PObject->AddReference();}return true;
 }}return false;
}
#pragma auto_inline(on)
CDefinitionManager* __fastcall GetDefinitionManager(); //0044c6b0
namespace NUISystem {
struct CManager {
 // Explicit native member projection: PDB donor+88 is retail+64 (0041d8f3).
 // Header preserved above; preceding UI state is opaque, no fabricated vtable slots.
 unsigned char NativeUIState[0x64];
 CDefinitionManager* m_pDefinitionManager;
 CDefPointer<CUIMiscThingsDef> GetMiscThingsDef();
 CDefPointer<CUIIconsDef> GetIconsDef();
};
}
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(NUISystem::CManager,m_pDefinitionManager)==0x64);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase,IVRefCount)==4);
FABLE_STATIC_ASSERT(sizeof(CDefPointer<NUISystem::CUIIconsDef>)==4);
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase)==sizeof(CIVCountedPointeeBaseDonor));
#endif
CDefPointer<NUISystem::CUIMiscThingsDef> NUISystem::CManager::GetMiscThingsDef(){
 CDefPointer<CUIMiscThingsDef> result;
 if(m_pDefinitionManager){
   long index; { CCharString name("UI_MISC_THINGS_DEF",-1); index=m_pDefinitionManager->GetGlobalIndexFromClassIndex(name,1); }
   m_pDefinitionManager->GetDef(index,&result);
 } else {
   long index; { CCharString name("UI_MISC_THINGS_DEF",-1); index=GetDefinitionManager()->GetGlobalIndexFromClassIndex(name,1); }
   GetDefinitionManager()->GetDef(index,&result);
 }
 return result;
}
