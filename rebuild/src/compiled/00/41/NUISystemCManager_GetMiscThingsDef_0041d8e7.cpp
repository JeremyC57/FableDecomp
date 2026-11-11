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
class CCharString { public: CCharStringData* Data; CCharString(const char*,long); CCharString(const CCharString&); ~CCharString(); };
class CIVCountedPointeeBase { public:
 virtual ~CIVCountedPointeeBase();
 virtual void Delete() const;
 unsigned long IVRefCount;
 void AddReference(){++IVRefCount;}
 void DecRef(){--IVRefCount;}
 void RemoveReference(){DecRef();if(GetRefCount()==0)Delete();}
 long GetRefCount()const{return IVRefCount;}
};
template<class T> class CDefPointer { public:
 CIVCountedPointeeBase* PObject;
 CDefPointer():PObject(0){}
 CDefPointer(const CDefPointer& p):PObject(p.PObject){if(PObject)PObject->AddReference();}
 template<class U> CDefPointer(const CDefPointer<U>& p):PObject(p.PObject){if(PObject)PObject->AddReference();} // Ego_r.pdb ??$?0$$CBV...@?$CDefPointer@V...@@QAE@ABV?$CDefPointer@$$CBV...@@@Z
 ~CDefPointer(){if(PObject)PObject->RemoveReference();}
 CDefPointer& operator=(const CDefPointer& p){if(PObject!=p.PObject){if(PObject){PObject->RemoveReference();PObject=0;}PObject=p.PObject;if(PObject)PObject->AddReference();}return *this;}
};
class CDefClassBase;
namespace NUISystem { class CUIMiscThingsDef; class CUIIconsDef; }
class CDefinitionManager { public:
 CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long) const; //009ad9e0
 long GetGlobalIndexFromClassIndex(const CCharString&,long)const; //009ad390
 template<class T> bool GetDef(long,CDefPointer<const T>*) const; //0042b312 GetDef<NUISystem::CUIMiscThingsDef>, 0042b374 GetDef<NUISystem::CUIIconsDef> (Ego_r.pdb member template)
};
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ
#pragma auto_inline(off)
template<> bool CDefinitionManager::GetDef<NUISystem::CUIMiscThingsDef>(long index,CDefPointer<const NUISystem::CUIMiscThingsDef>* out) const{
 if(index>0){ CDefPointer<const CDefClassBase> value=GetPDefFromGlobalIndex(index); if(value.PObject){
  if(out->PObject!=value.PObject){if(out->PObject){out->PObject->RemoveReference();out->PObject=0;}out->PObject=value.PObject;if(out->PObject)out->PObject->AddReference();}return true;
 }}return false;
}
template<> bool CDefinitionManager::GetDef<NUISystem::CUIIconsDef>(long index,CDefPointer<const NUISystem::CUIIconsDef>* out) const{
 if(index>0){ CDefPointer<const CDefClassBase> value=GetPDefFromGlobalIndex(index); if(value.PObject){
  if(out->PObject!=value.PObject){if(out->PObject){out->PObject->RemoveReference();out->PObject=0;}out->PObject=value.PObject;if(out->PObject)out->PObject->AddReference();}return true;
 }}return false;
}
#pragma auto_inline(on)
namespace NUISystem {
class CManager { public:
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
 CDefPointer<const CUIMiscThingsDef> result;   // the GetDef<T> out slot is CDefPointer<const T>; returned through the converting copy
 if(m_pDefinitionManager){
   long index; { CCharString name("UI_MISC_THINGS_DEF",-1); index=m_pDefinitionManager->GetGlobalIndexFromClassIndex(name,1); }
   m_pDefinitionManager->GetDef(index,&result);
 } else {
   long index; { CCharString name("UI_MISC_THINGS_DEF",-1); index=::CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name,1); }
   ::CGameDefinitionManager::Get().GetDef(index,&result);
 }
 return result;
}