#ifndef FABLE_CDEFCLASSBASE_GETSUBDEF_H
#define FABLE_CDEFCLASSBASE_GETSUBDEF_H
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefPointer.h"
template<class T>bool CDefClassBase::GetSubDef(CDefPointer<const T>* def)const {
 const CSubDefInfo* psub_def_info=GetPSubDefInfo(T::StatGetTypeName());
 if(psub_def_info){
  CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
  def->SetDynamicCast(pbase);
  return true;
 }
 return false;
}
#endif
