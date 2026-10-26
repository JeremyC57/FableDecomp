// BEGIN exact source snapshot SHA256 1c2e45bb987722127344051ea884e46c389da795fc32d4fe794bdd09ce71b4ad
// 005401b0 NUISystem::CEditBox::SetActive, PDB005b9850.
// No generated namespace CEditBox/CIME headers. Active178/UsesIME17a vs PDB1b8/1ba
// reflect base/container shrink40. Slot148 is CText::GetTextWindow, returning C2DBoxF&.
struct Vec {float X,Y;Vec(){}Vec(float x,float y):X(x),Y(y){}Vec(const Vec&v):X(v.X),Y(v.Y){}};
class C2DBoxF { public:float Left,Top,Right,Bottom;};
extern bool EditActive_013b8ab0;
namespace NUISystem {
class CEditBox;
class CIME { public:unsigned char BeforeEdit[0x4c];CEditBox* EditBox;static CIME& __fastcall Get();void Enable(bool);void SetEditBoxRegion(long,long,long,long);}; //00404a80,00404f40,00404bb0
struct Component {
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
virtual void Slot103()=0;
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
virtual void Slot116()=0;
virtual void Slot117()=0;
virtual void Slot118()=0;
virtual void Slot119()=0;
virtual void Slot120()=0;
virtual void Slot121()=0;
virtual Vec GetTargetRelativeRenderPosition()const=0;
virtual Vec GetTargetRelativeRenderZoom()const=0;
virtual void Slot124()=0;
virtual void Slot125()=0;
virtual void Slot126()=0;
virtual void Slot127()=0;
virtual void Slot128()=0;
virtual void Slot129()=0;
virtual void Slot130()=0;
virtual void Slot131()=0;
virtual void Slot132()=0;
virtual void Slot133()=0;
virtual void Slot134()=0;
virtual void Slot135()=0;
virtual void Slot136()=0;
virtual void Slot137()=0;
virtual void Slot138()=0;
virtual void Slot139()=0;
virtual void Slot140()=0;
virtual void Slot141()=0;
virtual void Slot142()=0;
virtual void Slot143()=0;
virtual Component* GetTextComponent()=0;
virtual void Slot145()=0;
virtual void Slot146()=0;
virtual void UpdateDisplayText()=0;
virtual C2DBoxF& GetTextWindow()=0;
};
class CEditBox:public Component { public:unsigned char ToActive[0x178-4];bool Active,IsPasswordBox,UsesIME;void SetActive(bool);};
void CEditBox::SetActive(bool active){
 Active=active;EditActive_013b8ab0=active;
 if(active&&UsesIME){
 Component* text=GetTextComponent();Vec position=text->GetTargetRelativeRenderPosition();
 Vec size(text->GetTextWindow().Right*text->GetTargetRelativeRenderZoom().X,text->GetTextWindow().Bottom*text->GetTargetRelativeRenderZoom().Y);
 CIME::Get().Enable(true);CIME::Get().SetEditBoxRegion((long)position.X,(long)position.Y,(long)size.X,(long)size.Y);
 CIME::Get().EditBox=this;
 }else{CIME::Get().Enable(false);}
 UpdateDisplayText();
}
}

// END exact source snapshot
#include <stdio.h>
bool EditActive_013b8ab0;using namespace NUISystem;static CIME ime;static bool enabled;static int region[4],updates;static C2DBoxF def;
CIME& __fastcall CIME::Get(){return ime;}void CIME::Enable(bool b){enabled=b;}void CIME::SetEditBoxRegion(long x,long y,long w,long h){region[0]=x;region[1]=y;region[2]=w;region[3]=h;}
struct Test:CEditBox {
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
void Slot103(){}
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
void Slot116(){}
void Slot117(){}
void Slot118(){}
void Slot119(){}
void Slot120(){}
void Slot121(){}
void Slot124(){}
void Slot125(){}
void Slot126(){}
void Slot127(){}
void Slot128(){}
void Slot129(){}
void Slot130(){}
void Slot131(){}
void Slot132(){}
void Slot133(){}
void Slot134(){}
void Slot135(){}
void Slot136(){}
void Slot137(){}
void Slot138(){}
void Slot139(){}
void Slot140(){}
void Slot141(){}
void Slot142(){}
void Slot143(){}
void Slot145(){}
void Slot146(){}
Vec GetTargetRelativeRenderPosition()const{return Vec(3.9f,7.1f);}Vec GetTargetRelativeRenderZoom()const{return Vec(2,3);}Component* GetTextComponent(){return this;}void UpdateDisplayText(){++updates;}C2DBoxF& GetTextWindow(){return def;}};
int main(){def.Left=1;def.Top=2;Test e;e.UsesIME=true;def.Right=20;def.Bottom=10;e.SetActive(true);if(!enabled||!e.Active||!EditActive_013b8ab0||ime.EditBox!=&e||region[0]!=3||region[1]!=7||region[2]!=40||region[3]!=30||updates!=1)return 1;
 e.SetActive(false);if(enabled||e.Active||EditActive_013b8ab0||updates!=2)return 2;e.UsesIME=false;e.SetActive(true);if(enabled||!e.Active||updates!=3)return 3;
 puts("005401b0_TEST PASS");return 0;}