// BEGIN exact source snapshot SHA256 f393f09bd5cfe4b161b434b9fdeff1ff22692e0edd4aec7e2bd1148ee4b28fbe
//00542140 NUISystem::CIconText::SetValue(resource ID); PDB SetValue and CDataBank text-entry signatures recovered.
#include "fable_string.h"
struct RefCount {mutable long References;void DecRef()const{--References;}long GetRefCount()const{return References;}};struct IVPointeeVirtual {virtual void Slot0()=0;virtual void Delete()const=0;};namespace NGameText { class CTextEntry:public IVPointeeVirtual,public RefCount { public:CWideString GetText()const;void Release()const{DecRef();if(GetRefCount()==0)Delete();}}; }
struct CIVTextPointer {const NGameText::CTextEntry* Object;__forceinline ~CIVTextPointer(){if(Object){Object->Release();Object=0;}}operator bool()const{return Object!=0;}const NGameText::CTextEntry* operator->()const{return Object;}};
struct CDataBankTextEntry;
struct CDataBank {const CDataBankTextEntry* GetTextInfo(unsigned long)const;CIVTextPointer GetPTextEntry(unsigned long,bool)const;};
// GetTextDataBank is an inferred inline accessor name; offsets14/60 are retail evidence.
struct Game {unsigned char Pad[0x14];CDataBank* Manager;__forceinline CDataBank* GetTextDataBank()const{return Manager;}};struct System {unsigned char Pad[0x60];CDataBank* Manager;__forceinline CDataBank* GetTextDataBank()const{return Manager;}};extern Game* Game_013b86a0;extern System* System_013b871c;extern CWideString Empty_013bca24;
namespace NUISystem {class CIconText { public:
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
virtual void Slot122()=0;
virtual void Slot123()=0;
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
virtual void Slot144()=0;
virtual void Slot145()=0;
virtual void Slot146()=0;
virtual void Slot147()=0;
virtual void Slot148()=0;
virtual void Slot149()=0;
virtual void UpdateText()=0;
unsigned char BeforeText[0x15c-4];CWideString Text;void SetValue(unsigned long);
};
void CIconText::SetValue(unsigned long id){
 if(id>0 && Game_013b86a0){if(Game_013b86a0->GetTextDataBank()->GetTextInfo(id)&&Game_013b86a0->GetTextDataBank()->GetPTextEntry(id,true))Text=Game_013b86a0->GetTextDataBank()->GetPTextEntry(id,true)->GetText();else Text=Empty_013bca24;}
 else if(id>0 && System_013b871c){if(System_013b871c->GetTextDataBank()->GetTextInfo(id)&&System_013b871c->GetTextDataBank()->GetPTextEntry(id,true))Text=System_013b871c->GetTextDataBank()->GetPTextEntry(id,true)->GetText();else Text=Empty_013bca24;}
 else Text=Empty_013bca24;
 UpdateText();
}
}

// END exact source snapshot
#include <stdio.h>
static int assignments,updates,getCalls,deletes;static bool exists=true,available=true;static CWideStringData data;
CWideString::CWideString():storage_(0){}CWideString::CWideString(const CWideString& s):storage_(s.storage_){}CWideString::~CWideString(){}const CWideString& CWideString::operator=(const CWideString& s){storage_=s.storage_;++assignments;return *this;}
Game* Game_013b86a0;System* System_013b871c;CWideString Empty_013bca24;
struct Resource:NGameText::CTextEntry {void Slot0(){}void Delete()const{++deletes;}}resource;
CWideString NGameText::CTextEntry::GetText()const{CWideString s;return s;}const CDataBankTextEntry* CDataBank::GetTextInfo(unsigned long)const{return exists?(const CDataBankTextEntry*)1:0;}CIVTextPointer CDataBank::GetPTextEntry(unsigned long,bool)const{++getCalls;CIVTextPointer p;p.Object=available?&resource:0;if(p.Object)++p.Object->References;return p;}
struct Icon:NUISystem::CIconText {
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
void Slot122(){}
void Slot123(){}
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
void Slot144(){}
void Slot145(){}
void Slot146(){}
void Slot147(){}
void Slot148(){}
void Slot149(){}
void UpdateText(){++updates;}};
int main(){CDataBank mgr;Game game;game.Manager=&mgr;System sys;sys.Manager=&mgr;Icon icon;resource.References=10;Game_013b86a0=&game;icon.SetValue(5);if(getCalls!=2||updates!=1||assignments!=1)return 1;Game_013b86a0=0;System_013b871c=&sys;icon.SetValue(5);if(getCalls!=4||updates!=2)return 2;exists=false;icon.SetValue(5);if(getCalls!=4||updates!=3)return 3;exists=true;available=false;icon.SetValue(5);if(getCalls!=5||updates!=4)return 4;icon.SetValue(0);if(getCalls!=5||updates!=5||assignments!=5)return 5;puts("00542140_TEST PASS");return 0;}