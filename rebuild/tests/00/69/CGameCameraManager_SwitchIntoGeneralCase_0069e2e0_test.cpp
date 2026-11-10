#include <cstdio>
#include <list>
#include <cmath>
#include <cstring>
class CCameraModeDef { public:int guard;};
template<class T>class CDefPointer { public:T* Object;};
namespace {CDefPointer<const CCameraModeDef> DEFAULT_CAMERA_MODE_DEF;}
static int allocated,destroyed,reset_calls,set_calls,copy_count,wrong_args;
class CGameCameraManager;
namespace NCameraModes {
class CBase {public:virtual ~CBase(){++destroyed;}};
class CStringAndCage:public CBase {public:CGameCameraManager* Manager;const CDefPointer<const CCameraModeDef>* Definition;float StringLength,CageRadius,HeightOffset,Time,FOV;bool AutoGoBehind;CStringAndCage(CGameCameraManager& m,const CDefPointer<const CCameraModeDef>& d,float a,float b,float c,bool flag,float time,float fov):Manager(&m),Definition(&d),StringLength(a),CageRadius(b),HeightOffset(c),Time(time),FOV(fov),AutoGoBehind(flag){++allocated;}};
}
class CCPPointerInfo { public:int RefCount;void(*DeleteFunc)(void*);void* Data;CCPPointerInfo(int r,void(*f)(void*),void* d):RefCount(r),DeleteFunc(f),Data(d){}};
template<class T>class CCountedPointer {public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}CCountedPointer(const CCountedPointer& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;++copy_count;}~CCountedPointer(){ReleaseData();}void ReleaseData(){if(Info&&--Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}Info=0;}static void DeleteData(void* data){delete static_cast<T*>(data);}void Reset(T* data);};
class CTCCameraPointDefinitionBase {public:float FOV;float GetFOV()const;};
class CTCCameraPointGeneralCase:public CTCCameraPointDefinitionBase {public:float CameraStringLength,CameraCageRadius,CameraHeightOffset,AutoGoBehindTime;bool AutoGoBehind;float GetStringLength()const;float GetCageRadius()const;float GetHeightOffset()const;float GetAutoGoBehindTime()const;bool IsAutoGoBehind()const;};
class CThing {public:const CTCCameraPointGeneralCase* Component;bool PeekTC(const CTCCameraPointGeneralCase** out)const{*out=Component;return true;}};
template<class T>class CIntelligentPointer { public:T* Value;T* Get()const{return Value;}};
class CGameCameraManager {public:std::list<CIntelligentPointer<CThing> > AvailableCamPoints;void SwitchIntoGeneralCase();float ExpectedLength,ExpectedRadius,ExpectedHeight,ExpectedTime,ExpectedFOV;bool ExpectedAuto;void SetCameraMode(CCountedPointer<NCameraModes::CBase> mode){++set_calls;NCameraModes::CStringAndCage* value=static_cast<NCameraModes::CStringAndCage*>(mode.Data);wrong_args|=!value||value->Manager!=this||value->Definition!=&DEFAULT_CAMERA_MODE_DEF||mode.Info->RefCount!=2;wrong_args|=value->StringLength!=ExpectedLength||value->CageRadius!=ExpectedRadius||value->HeightOffset!=ExpectedHeight||value->Time!=ExpectedTime||value->FOV!=ExpectedFOV||value->AutoGoBehind!=ExpectedAuto;}};
template<class T>void CCountedPointer<T>::Reset(T* data)
{
 ReleaseData();
 Data=data;
 if(Data)Info=new CCPPointerInfo(1,DeleteData,(void*)Data);
}
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

static unsigned int Bits(float value){unsigned int bits;std::memcpy(&bits,&value,4);return bits;}
static float Value(unsigned int bits){float value;std::memcpy(&value,&bits,4);return value;}
int main(){int ok=1,cases=0;CCameraModeDef definition;definition.guard=95;DEFAULT_CAMERA_MODE_DEF.Object=&definition;for(int length=0;length<4;++length)for(int radius=0;radius<3;++radius)for(int flag=0;flag<2;++flag)for(int angle=0;angle<4;++angle){CTCCameraPointGeneralCase settings;settings.CameraStringLength=(float)(length*2-3);settings.CameraCageRadius=(float)radius*0.5f;settings.CameraHeightOffset=(float)(radius-1)*0.25f;settings.AutoGoBehindTime=(float)length*0.125f;settings.AutoGoBehind=flag!=0;settings.FOV=(float)(angle*90);CThing thing;thing.Component=&settings;CIntelligentPointer<CThing> pointer;pointer.Value=&thing;CGameCameraManager manager;manager.AvailableCamPoints.push_back(pointer);manager.ExpectedLength=settings.CameraStringLength;manager.ExpectedRadius=settings.CameraCageRadius;manager.ExpectedHeight=settings.CameraHeightOffset;manager.ExpectedTime=settings.AutoGoBehindTime;manager.ExpectedAuto=settings.AutoGoBehind;manager.ExpectedFOV=settings.FOV/360.0f;int a=allocated,d=destroyed,c=copy_count,s=set_calls;manager.SwitchIntoGeneralCase();ok &= allocated==a+1&&destroyed==d+1&&set_calls==s+1&&copy_count==c+1;ok &= manager.AvailableCamPoints.front().Get()==&thing&&definition.guard==95;++cases;}unsigned int fixtures[]={0,0x80000000u,0x3f800000u,0xbf800000u,0x00800000u,0x7f7fffffu,0x7f800000u,0xff800000u};for(int i=0;i<8;++i){CTCCameraPointGeneralCase point;point.CameraStringLength=point.CameraCageRadius=point.CameraHeightOffset=point.AutoGoBehindTime=Value(fixtures[i]);ok &= Bits(point.GetStringLength())==fixtures[i]&&Bits(point.GetCageRadius())==fixtures[i]&&Bits(point.GetHeightOffset())==fixtures[i]&&Bits(point.GetAutoGoBehindTime())==fixtures[i];}ok &= cases==96&&!wrong_args;std::printf(ok?"0069e2e0_TEST PASS\n":"0069e2e0_TEST FAIL\n");return ok?0:1;}