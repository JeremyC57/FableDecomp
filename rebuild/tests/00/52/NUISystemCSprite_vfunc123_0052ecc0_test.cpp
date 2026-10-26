// BEGIN exact source snapshot SHA256 4e72bad1d3ccace93fc9a11bec02167e388f311a9090ce87a828a39c70c49e19
// 0052ecc0 inherited CComponent::GetTargetRelativeRenderZoom, PDB debug005a88b0.
// Layout: shared CComponent is flattened PDB donor; retail TargetZoom64,Parentc8,PositionParent118
// are observed fields; PDB TargetZoom6c shifts8, later fields shift30.
// CComponent vtable0124608c slot123 and CSprite slot123 both contain0052ecc0.
#include "engine/CSprite.h" // donor includes inherited CComponent storage; no CComponent header exists
struct Vec {float X,Y;Vec(){} Vec(float x,float y):X(x),Y(y){} Vec(const Vec& o):X(o.X),Y(o.Y){} };
namespace NUISystem {
struct CManagerCalls {static CManagerCalls* __fastcall GetInstance(); Vec GetUIScale()const;}; //0041e5f2,0041cf47
// CUIScreenUnitConverter::ConvertVector recovered PDB debug0055ed1a, retail0052e580.
struct CUIScreenUnitConverter {static Vec __fastcall ConvertVector(Vec value);};
class CComponent { public:
 virtual void Slot000()=0;
 virtual void Slot001()=0;
 virtual void Slot002()=0;
 virtual void Slot003()=0;
 virtual void Slot004()=0;
 virtual void Slot005()=0;
 virtual void Slot006()=0;
 virtual void Slot007()=0;
 virtual void Slot008()=0;
 virtual void Slot009()=0;
 virtual void Slot010()=0;
 virtual void Slot011()=0;
 virtual void Slot012()=0;
 virtual void Slot013()=0;
 virtual void Slot014()=0;
 virtual void Slot015()=0;
 virtual void Slot016()=0;
 virtual void Slot017()=0;
 virtual void Slot018()=0;
 virtual void Slot019()=0;
 virtual void Slot020()=0;
 virtual void Slot021()=0;
 virtual void Slot022()=0;
 virtual void Slot023()=0;
 virtual void Slot024()=0;
 virtual void Slot025()=0;
 virtual void Slot026()=0;
 virtual void Slot027()=0;
 virtual void Slot028()=0;
 virtual void Slot029()=0;
 virtual void Slot030()=0;
 virtual void Slot031()=0;
 virtual void Slot032()=0;
 virtual void Slot033()=0;
 virtual void Slot034()=0;
 virtual void Slot035()=0;
 virtual void Slot036()=0;
 virtual void Slot037()=0;
 virtual void Slot038()=0;
 virtual void Slot039()=0;
 virtual void Slot040()=0;
 virtual void Slot041()=0;
 virtual void Slot042()=0;
 virtual void Slot043()=0;
 virtual void Slot044()=0;
 virtual void Slot045()=0;
 virtual void Slot046()=0;
 virtual void Slot047()=0;
 virtual void Slot048()=0;
 virtual void Slot049()=0;
 virtual void Slot050()=0;
 virtual void Slot051()=0;
 virtual void Slot052()=0;
 virtual void Slot053()=0;
 virtual void Slot054()=0;
 virtual void Slot055()=0;
 virtual void Slot056()=0;
 virtual void Slot057()=0;
 virtual void Slot058()=0;
 virtual void Slot059()=0;
 virtual void Slot060()=0;
 virtual void Slot061()=0;
 virtual void Slot062()=0;
 virtual void Slot063()=0;
 virtual void Slot064()=0;
 virtual void Slot065()=0;
 virtual void Slot066()=0;
 virtual void Slot067()=0;
 virtual void Slot068()=0;
 virtual void Slot069()=0;
 virtual void Slot070()=0;
 virtual void Slot071()=0;
 virtual void Slot072()=0;
 virtual void Slot073()=0;
 virtual void Slot074()=0;
 virtual void Slot075()=0;
 virtual void Slot076()=0;
 virtual void Slot077()=0;
 virtual void Slot078()=0;
 virtual void Slot079()=0;
 virtual void Slot080()=0;
 virtual void Slot081()=0;
 virtual void Slot082()=0;
 virtual void Slot083()=0;
 virtual void Slot084()=0;
 virtual void Slot085()=0;
 virtual void Slot086()=0;
 virtual void Slot087()=0;
 virtual void Slot088()=0;
 virtual void Slot089()=0;
 virtual void Slot090()=0;
 virtual void Slot091()=0;
 virtual void Slot092()=0;
 virtual void Slot093()=0;
 virtual void Slot094()=0;
 virtual void Slot095()=0;
 virtual void Slot096()=0;
 virtual void Slot097()=0;
 virtual void Slot098()=0;
 virtual void Slot099()=0;
 virtual void Slot100()=0;
 virtual void Slot101()=0;
 virtual void Slot102()=0;
 virtual bool IsZoomIndependent()const=0;
 virtual void Slot104()=0;
 virtual void Slot105()=0;
 virtual void Slot106()=0;
 virtual void Slot107()=0;
 virtual void Slot108()=0;
 virtual void Slot109()=0;
 virtual void Slot110()=0;
 virtual void Slot111()=0;
 virtual void Slot112()=0;
 virtual void Slot113()=0;
 virtual void Slot114()=0;
 virtual void Slot115()=0;
 virtual bool IsUsingRelativeZoom()const=0;
 virtual void Slot117()=0;
 virtual void Slot118()=0;
 virtual void Slot119()=0;
 virtual void Slot120()=0;
 virtual void Slot121()=0;
 virtual void Slot122()=0;
 virtual Vec GetTargetRelativeRenderZoom()const;
 unsigned char ToSize[0x64-4];Vec TargetZoom;unsigned char ToParent[0xc8-0x6c];CComponent* Parent;
 unsigned char ToOverride[0x118-0xcc];CComponent* PositionParent;
};
Vec CComponent::GetTargetRelativeRenderZoom()const {
 Vec size=TargetZoom;
 if(!Parent||IsZoomIndependent()) {
 size.X*=CManagerCalls::GetInstance()->GetUIScale().X;
 size.Y*=CManagerCalls::GetInstance()->GetUIScale().Y;
 }
 if(IsUsingRelativeZoom())size=CUIScreenUnitConverter::ConvertVector(size);
 CComponent* parent=PositionParent?PositionParent:Parent;
 if(parent&&!IsZoomIndependent()) {
  if(!IsZoomIndependent()) {size.X*=parent->GetTargetRelativeRenderZoom().X;size.Y*=parent->GetTargetRelativeRenderZoom().Y;}
  return Vec(size.X,size.Y);
 }
 return size;
}
}

// END exact source snapshot
#include <stdio.h>
namespace NUISystem {
CManagerCalls* __fastcall CManagerCalls::GetInstance(){static CManagerCalls m;return &m;}
Vec CManagerCalls::GetUIScale()const{return Vec(2,3);}
Vec __fastcall CUIScreenUnitConverter::ConvertVector(Vec v){return Vec(v.X*10,v.Y*10);}
struct TestSprite:CComponent {bool independent,screen;
void Slot000(){}
void Slot001(){}
void Slot002(){}
void Slot003(){}
void Slot004(){}
void Slot005(){}
void Slot006(){}
void Slot007(){}
void Slot008(){}
void Slot009(){}
void Slot010(){}
void Slot011(){}
void Slot012(){}
void Slot013(){}
void Slot014(){}
void Slot015(){}
void Slot016(){}
void Slot017(){}
void Slot018(){}
void Slot019(){}
void Slot020(){}
void Slot021(){}
void Slot022(){}
void Slot023(){}
void Slot024(){}
void Slot025(){}
void Slot026(){}
void Slot027(){}
void Slot028(){}
void Slot029(){}
void Slot030(){}
void Slot031(){}
void Slot032(){}
void Slot033(){}
void Slot034(){}
void Slot035(){}
void Slot036(){}
void Slot037(){}
void Slot038(){}
void Slot039(){}
void Slot040(){}
void Slot041(){}
void Slot042(){}
void Slot043(){}
void Slot044(){}
void Slot045(){}
void Slot046(){}
void Slot047(){}
void Slot048(){}
void Slot049(){}
void Slot050(){}
void Slot051(){}
void Slot052(){}
void Slot053(){}
void Slot054(){}
void Slot055(){}
void Slot056(){}
void Slot057(){}
void Slot058(){}
void Slot059(){}
void Slot060(){}
void Slot061(){}
void Slot062(){}
void Slot063(){}
void Slot064(){}
void Slot065(){}
void Slot066(){}
void Slot067(){}
void Slot068(){}
void Slot069(){}
void Slot070(){}
void Slot071(){}
void Slot072(){}
void Slot073(){}
void Slot074(){}
void Slot075(){}
void Slot076(){}
void Slot077(){}
void Slot078(){}
void Slot079(){}
void Slot080(){}
void Slot081(){}
void Slot082(){}
void Slot083(){}
void Slot084(){}
void Slot085(){}
void Slot086(){}
void Slot087(){}
void Slot088(){}
void Slot089(){}
void Slot090(){}
void Slot091(){}
void Slot092(){}
void Slot093(){}
void Slot094(){}
void Slot095(){}
void Slot096(){}
void Slot097(){}
void Slot098(){}
void Slot099(){}
void Slot100(){}
void Slot101(){}
void Slot102(){}
void Slot104(){}
void Slot105(){}
void Slot106(){}
void Slot107(){}
void Slot108(){}
void Slot109(){}
void Slot110(){}
void Slot111(){}
void Slot112(){}
void Slot113(){}
void Slot114(){}
void Slot115(){}
void Slot117(){}
void Slot118(){}
void Slot119(){}
void Slot120(){}
void Slot121(){}
void Slot122(){}
 bool IsZoomIndependent()const{return independent;}bool IsUsingRelativeZoom()const{return screen;}
 TestSprite(){Parent=PositionParent=0;independent=screen=false;TargetZoom=Vec(4,5);}
};}
int main(){using namespace NUISystem;TestSprite s,p;Vec v=s.GetTargetRelativeRenderZoom();if(v.X!=8||v.Y!=15)return 1;
 s.Parent=&p;v=s.GetTargetRelativeRenderZoom();if(v.X!=32||v.Y!=75)return 2;
 s.independent=true;s.screen=true;v=s.GetTargetRelativeRenderZoom();if(v.X!=80||v.Y!=150)return 3;
 puts("0052ecc0_TEST PASS");return 0;}