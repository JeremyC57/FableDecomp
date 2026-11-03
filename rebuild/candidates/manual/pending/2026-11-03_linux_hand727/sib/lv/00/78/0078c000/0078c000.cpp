// 0078c000 CTCBerserk::GetNumFramesBeforeCrazyIdle
// Leaf: ?GetNumFramesBeforeCrazyIdle@CTCBerserk@@SIJXZ
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/_quarantine/CSpecialAbilitiesBerserkDef.h"
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CTCBerserk CTCBerserkLayout
#include "engine/CTCBerserk.h"
#undef CTCBerserk
class CCharStringData;
class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CMainGameComponent{protected:static long ConstantFPS;public:static long __fastcall GetConstantFPS(){return ConstantFPS;}};
// Layout: original shared manager projection, with original template methods restored.
class CDefinitionManager:public CDefinitionManagerLayout{public:
 long GetGlobalIndexFromClassIndex(const CCharString&,long)const;
 CDefPointer<const CDefClassBase>GetPDefFromGlobalIndex(long)const;
 template<class T>bool GetDef(long,CDefPointer<const T>*)const;
};
class CGameDefinitionManager:public CDefinitionManager{public:static CGameDefinitionManager&__fastcall Get();};
class CTCBerserk:public CTCBerserkLayout{public:static long __fastcall GetNumFramesBeforeCrazyIdle();};
template<class T>__declspec(noinline)bool CDefinitionManager::GetDef(long index,CDefPointer<const T>*out)const{
 if(index<=0)return false;
 CDefPointer<const CDefClassBase>value=GetPDefFromGlobalIndex(index);
 if(value.IsNull())return false;
 out->SetDynamicCast(value);return true;
}
long __fastcall CTCBerserk::GetNumFramesBeforeCrazyIdle(){
 long index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BERSERK_DEF",1);
 {
  CDefPointer<const CSpecialAbilitiesBerserkDef>def;
  CGameDefinitionManager::Get().GetDef(index,&def);
  // Layout: shared def is flattened; the original pointee base begins at zero.
  return (long)(CMainGameComponent::GetConstantFPS()*reinterpret_cast<const CSpecialAbilitiesBerserkDef*>(def.InternalGetObject())->CrazyIdleIntervalSecs);
 }
}