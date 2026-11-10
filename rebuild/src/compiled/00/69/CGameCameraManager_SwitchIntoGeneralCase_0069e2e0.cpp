// CGameCameraManager::SwitchIntoGeneralCase() -- build a string-and-cage mode from the first available camera point.
// Leaf: ?SwitchIntoGeneralCase@CGameCameraManager@@AAEXXZ
// Flags: none
// Retail:0x0069E2E0,237 bytes,__fastcall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMPOINTS
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GENERALCASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_CONSTRUCT
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CStringAndCage.h"
#include "engine/CTCCameraPointGeneralCase.h"
namespace {extern CDefPointer<const CCameraModeDef> DEFAULT_CAMERA_MODE_DEF;}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
NCameraModes::CStringAndCage::CStringAndCage(CGameCameraManager& manager,const CDefPointer<const CCameraModeDef>& definition,
 float string_length,float cage_radius,float height_offset,bool auto_go_behind,float auto_go_behind_time,float fov)
 :CStringAndCageBase(manager,definition,string_length,cage_radius,height_offset),AutoGoBehind(auto_go_behind),AutoGoBehindTime(auto_go_behind_time)
{DefaultFOV=fov;}
template<class T>void CCountedPointer<T>::Reset(T* data)
{
 ReleaseData();
 Data=data;
 if(Data)Info=new CCPPointerInfo(1,DeleteData,(void*)Data);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
float CTCCameraPointDefinitionBase::GetFOV()const{return FOV/360.0f;}
// Unary+ preserves each original float return while folding its scalar result into the constructor argument.
float CTCCameraPointGeneralCase::GetStringLength()const{return +(CameraStringLength);}
float CTCCameraPointGeneralCase::GetCageRadius()const{return +(CameraCageRadius);}
float CTCCameraPointGeneralCase::GetHeightOffset()const{return +(CameraHeightOffset);}
bool CTCCameraPointGeneralCase::IsAutoGoBehind()const{return AutoGoBehind;}
float CTCCameraPointGeneralCase::GetAutoGoBehindTime()const{return +(AutoGoBehindTime);}
void CGameCameraManager::SwitchIntoGeneralCase()
{
 const CThing* cam_pt=AvailableCamPoints.front().Get();
 const CTCCameraPointGeneralCase* ptc_gen_case;
 cam_pt->PeekTC(&ptc_gen_case);
 CCountedPointer<NCameraModes::CBase> general_case;
 general_case.Reset(new NCameraModes::CStringAndCage(*this,DEFAULT_CAMERA_MODE_DEF,
  ptc_gen_case->GetStringLength(),ptc_gen_case->GetCageRadius(),ptc_gen_case->GetHeightOffset(),
  ptc_gen_case->IsAutoGoBehind(),ptc_gen_case->GetAutoGoBehindTime(),ptc_gen_case->GetFOV()));
 SetCameraMode(general_case);
}