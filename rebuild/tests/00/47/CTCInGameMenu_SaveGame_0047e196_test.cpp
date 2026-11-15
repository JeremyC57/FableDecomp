#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <new>
#include "rebuild_abi.h"
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
#define FABLE_RETAIL_STLP_FILL_N_FASTCALL
#include "stlport/fable_containers.h"
namespace std {void* FABLE_FASTCALL __copy_trivial(const void*f,const void*l,void*r){return l==f?r:static_cast<char*>(memmove(r,f,static_cast<const char*>(l)-static_cast<const char*>(f)))+(static_cast<const char*>(l)-static_cast<const char*>(f));}}
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
struct CWideString {
 wchar_t Text[80]; CWideString(){Text[0]=0;} CWideString(const wchar_t*s){wcscpy(Text,s);} CWideString(const CWideString&s){wcscpy(Text,s.Text);}
 CWideString&operator=(const CWideString&s){wcscpy(Text,s.Text);return *this;}bool operator!=(const wchar_t*s)const{return wcscmp(Text,s)!=0;}
};
CWideString FABLE_FASTCALL operator+(const wchar_t*a,const CWideString&b){CWideString s(a);wcscat(s.Text,b.Text);return s;}
class CCharString {public:const char*Text;CCharString(const char*s,long=-1):Text(s){}};
class CDateAndTime {public:bool Valid;bool IsValid()const{return Valid;}};
class C2DVector {public:float X,Y;C2DVector(float x,float y):X(x),Y(y){}};
class C3DVector {public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
enum EMessageEventType {MESSAGE_EVENT_GAME_SAVED_MANUALLY=62};
static bool Debug,Disk,PathOK,Quest,HaveText,HaveButton,FastTime;
static long SaveCalls,SetSlotCalls,FileNameCalls,PathCalls,DiskCalls,TimeCalls,MessageCalls,LiveComponents,LiveActions,LiveParams,Removed;
static long LastSlot,LastFileIndex;static CWideString LastName,LastPathInput;static const char*TextValue;static long Events[32],EventCount;
static void Event(long code){assert(EventCount<32);Events[EventCount++]=code;}
void FABLE_FASTCALL SavedConfirmed(){}
namespace NUISystem {
enum EEvent{UI_EVENT_MOUSE_MOVEMENT=25};enum EUIActionType{UI_ACTION_TYPE_SEND_EVENT=231};
class CActionParam {public:virtual~CActionParam(){--LiveParams;}CActionParam(){++LiveParams;}};
class CActionParamSLONG:public CActionParam {public:long Value;CActionParamSLONG(long n):Value(n){}};
class CAction {public:long ActionType;std::list<CActionParam*>UIActionParams;CAction():ActionType(0){++LiveActions;}~CAction(){for(std::list<CActionParam*>::iterator it=UIActionParams.begin();it!=UIActionParams.end();++it)delete *it;--LiveActions;}};
class CComponent {
public:long Id;CCountedPointer<CAction> Action;CComponent(long id):Id(id){++LiveComponents;}virtual~CComponent(){if(Id==10)++Removed;--LiveComponents;}
 virtual CComponent*FindChild(const CCharString&){assert(false);return 0;}
 void Initialise(){assert(Id==1);Event(101);}void ChangeState(unsigned long n){assert(Id==1&&n==1);Event(104);}
 void AddActionOnLeftUnclicked(const CCountedPointer<CAction>&a){assert(Id==2&&a->ActionType==231&&a->UIActionParams.size()==1);CActionParamSLONG*p=static_cast<CActionParamSLONG*>(a->UIActionParams.front());assert(p->Value==4);Action=a;Event(105);}
 void ObserveEvent(EEvent n){assert(n==UI_EVENT_MOUSE_MOVEMENT);Event(200+Id);}void ObserveAllEvents(){assert(Id==1);Event(301);}
 void ChangePosition(const C2DVector&p,float z,bool b){assert(Id==1&&p.X==110.0f&&p.Y==70.0f&&z==0&&!b);Event(401);}
};
class CText:public CComponent {public:CText():CComponent(4){}void SetValue(const CCharString&s,bool a,bool b){assert(a&&b);TextValue=s.Text;Event(strcmp(s.Text,"TEXT_GUI_SAVE_FAILED")==0?502:501);}};
class CYesNo:public CComponent {public:CText Text;CComponent Button;CYesNo():CComponent(1),Button(2){}CComponent*FindChild(const CCharString&s){if(strcmp(s.Text,"PC_GAME_SAVED_GAME_TEXT")==0)return HaveText?&Text:0;assert(strcmp(s.Text,"PC_OK_GAME_OKBUTTON")==0);return HaveButton?&Button:0;}
 void SetYesCallBack(void(FABLE_FASTCALL*p)()){assert(p==SavedConfirmed);Event(102);}void SetNoCallBack(void(FABLE_FASTCALL*p)()){assert(p==SavedConfirmed);Event(103);}
};
class CManager {public:static CManager*GetInstance(){static CManager m;return &m;}CComponent*CreateComponent(const CCharString&s,bool b){assert(strcmp(s.Text,"PC_DIALOG_GAME_SAVED_GAME")==0&&!b);Event(100);return new CYesNo;}
 void AddConcurrentExclusiveObserver(CComponent*p){Event(600+p->Id);}void RemoveObserver(CComponent*p){Event(700+p->Id);}C2DVector GetUIScale(){return C2DVector(2.5f,4.25f);}
};
class CUIScreenUnitConverter {public:static float FABLE_FASTCALL GetBackBufferWidth(){return 1240.0f;}static float FABLE_FASTCALL GetBackBufferHeight(){return 582.0f;}};
}
class CUserProfileManager {public:static CUserProfileManager&FABLE_FASTCALL Get(){static CUserProfileManager p;return p;}bool IsDebugProfile()const{return Debug;}CWideString GetManualSaveFileName(long n)const{++FileNameCalls;LastFileIndex=n;wchar_t s[3]={L'S',static_cast<wchar_t>(L'0'+n),0};return CWideString(s);}CWideString GetPathNameFromFileName(const CWideString&s,bool b)const{assert(b);++PathCalls;LastPathInput=s;return CWideString(PathOK?L"path":L"");}void SetSaveSlotFileName(long n,const CWideString&s){++SetSlotCalls;LastSlot=n;LastName=s;}};
class CMessageEventManager {public:void*AddMessage(EMessageEventType t,unsigned long fps,const C3DVector&p,void*a,void*b,float x,float y,long n){assert(t==62&&fps==30&&p.X==0&&p.Y==0&&p.Z==0&&!a&&!b&&x==0&&y==0&&n==0);++MessageCalls;return 0;}};
class CWorld {public:CMessageEventManager Messages;bool IsDiskSpaceForSaveGame()const{++DiskCalls;return Disk;}void SaveGameStateManual(const CWideString&s){++SaveCalls;LastName=s;}CMessageEventManager&GetMessageEventManager(){return Messages;}};
class CMainGameComponent {public:static CMainGameComponent*Get(){static CMainGameComponent g;return &g;}CWorld&GetWorld(){static CWorld w;return w;}static long FABLE_FASTCALL GetConstantFPS(){return 30;}};
class CQuestManager {public:static CQuestManager&Get(){static CQuestManager q;return q;}bool IsBetweenQuestStartAndFinishScreens()const{return Quest;}};
double FABLE_FASTCALL GFGetTime(){return static_cast<double>(TimeCalls++)+(FastTime?4.0:0.0);}
class CTCInGameMenu {public:std::vector<CWideString>FileNames;std::vector<CDateAndTime>FileTimes;std::vector<NUISystem::CComponent*>Components;NUISystem::CComponent*PSaveInProgress;double SaveInProgressTime;NUISystem::CYesNo*m_pOKDialog;NUISystem::CComponent*PMousePointer;CWorld&GetWorld()const{return CMainGameComponent::Get()->GetWorld();}void SaveGame(long);};
using namespace NUISystem;

namespace std {
// Original STLport pointer allocator operations are expanded in the retail overflow body.
template<> FABLE_FORCEINLINE CComponent** allocator<CComponent*>::allocate(size_t n, const void*)
{ return n ? static_cast<CComponent**>(malloc(n * sizeof(CComponent*))) : 0; }
template<> inline void allocator<CComponent*>::deallocate(CComponent** p, size_t)
{ if (p) free(p); }

#ifdef _MSC_VER
#pragma optimize("y", on)
#pragma optimize("s", on)
#pragma auto_inline(off)
#endif
template<>
CComponent** FABLE_FASTCALL fill_n(CComponent** first, unsigned int n, CComponent* const& x)
{
    for (; n > 0; --n, ++first)
        *first = x;
    return first;
}

template<>
_List_node<CActionParam*>* list<CActionParam*>::_M_create_node(CActionParam* const& x)
{
    _List_node<CActionParam*>* p = this->_M_node.allocate(1);
    new(static_cast<void*>(&p->_M_data)) CActionParam*(x);
    return p;
}

template<>
list<CActionParam*>::iterator list<CActionParam*>::insert(iterator position, CActionParam* const& x)
{
    _List_node<CActionParam*>* tmp = _M_create_node(x);
    _List_node_base* n = position._M_node;
    _List_node_base* p = n->_M_prev;
    tmp->_M_next=n; tmp->_M_prev=p; p->_M_next=tmp; n->_M_prev=tmp;
    return iterator(tmp);
}

template<>
vector<CComponent*>::iterator vector<CComponent*>::erase(iterator position)
{
    if (position + 1 != end())
        __copy_trivial(position + 1, this->_M_finish, position);
    --this->_M_finish;
    _Destroy(this->_M_finish);
    return position;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#pragma optimize("y", off)
#endif

} // namespace std

void CTCInGameMenu::SaveGame(long index)
{
    CWideString file_name;
    if (FileTimes[index].IsValid())
        file_name = FileNames[index];
    else if (CUserProfileManager::Get().IsDebugProfile())
        file_name = CUserProfileManager::Get().GetManualSaveFileName(FileNames.size());
    else {
        file_name = CUserProfileManager::Get().GetManualSaveFileName(index);
        file_name = L"Manual - " + file_name;
    }
    bool save_failed = false;
    if ((unsigned long)index < FileNames.size() && !CMainGameComponent::Get()->GetWorld().IsDiskSpaceForSaveGame())
        save_failed = true;
    else {
        CWideString path_name = CUserProfileManager::Get().GetPathNameFromFileName(file_name, true);
        if (path_name != L"") {
            if (!CUserProfileManager::Get().IsDebugProfile()) {
                if (index <= 0)
                    return;
                CUserProfileManager::Get().SetSaveSlotFileName(index - 1, file_name);
            }
            CMainGameComponent::Get()->GetWorld().SaveGameStateManual(file_name);
        }
    }
    while (GFGetTime() - SaveInProgressTime < 3.0) {
    }
    for (CComponent** it = Components.begin(); it != Components.end(); ++it) {
        if (*it == PSaveInProgress) {
            delete PSaveInProgress;
            Components.erase(it);
            break;
        }
    }
    if (!save_failed)
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_GAME_SAVED_MANUALLY, CMainGameComponent::GetConstantFPS(), C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
    CComponent* pComponent = CManager::GetInstance()->CreateComponent("PC_DIALOG_GAME_SAVED_GAME", false);
    if (CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
        CComponent* ptext_confirm = pComponent->FindChild("PC_GAME_SAVED_GAME_TEXT");
        if (ptext_confirm) {
            CText* ptext_confirm_real = static_cast<CText*>(ptext_confirm);
            ptext_confirm_real->SetValue("TEXT_GUI_CONFIRM_HERO_SAVED", true, true);
        }
    }
    if (save_failed) {
        CComponent* ptext_confirm = pComponent->FindChild("PC_GAME_SAVED_GAME_TEXT");
        if (ptext_confirm) {
            CText* ptext_confirm_real = static_cast<CText*>(ptext_confirm);
            ptext_confirm_real->SetValue("TEXT_GUI_SAVE_FAILED", true, true);
        }
    }
    pComponent->Initialise();
    Components.push_back(pComponent);
    m_pOKDialog = static_cast<CYesNo*>(pComponent);
    m_pOKDialog->SetYesCallBack(SavedConfirmed);
    m_pOKDialog->SetNoCallBack(SavedConfirmed);
    m_pOKDialog->ChangeState(1);
    CComponent* pclickable = m_pOKDialog->FindChild("PC_OK_GAME_OKBUTTON");
    if (pclickable) {
        CCountedPointer<CAction> paction(new CAction);
        paction->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        paction->UIActionParams.push_back(new CActionParamSLONG(4));
        pclickable->AddActionOnLeftUnclicked(paction);
        CManager::GetInstance()->AddConcurrentExclusiveObserver(pclickable);
        pclickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    CManager::GetInstance()->RemoveObserver(m_pOKDialog);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(m_pOKDialog);
    CManager::GetInstance()->RemoveObserver(PMousePointer);
    CManager::GetInstance()->AddConcurrentExclusiveObserver(PMousePointer);
    PMousePointer->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    m_pOKDialog->ObserveAllEvents();
    C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 204.0f,
                                       CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    m_pOKDialog->ChangePosition(pos, 0.0f, false);
}

int main(){
 for(long c=0;c<4096;++c){
  Debug=(c&1)!=0;Disk=(c&2)!=0;PathOK=(c&4)!=0;Quest=(c&8)!=0;HaveText=(c&16)!=0;HaveButton=(c&32)!=0;FastTime=(c&64)!=0;bool valid=(c&128)!=0,findProgress=(c&256)!=0,newSlot=(c&512)!=0;long n=2+(c%7),index=newSlot?n:(c>>10)%n;if(newSlot)valid=false;
  SaveCalls=SetSlotCalls=FileNameCalls=PathCalls=DiskCalls=TimeCalls=MessageCalls=EventCount=Removed=0;LastSlot=LastFileIndex=-1;LastName=CWideString();LastPathInput=CWideString();TextValue=0;
  CTCInGameMenu m;m.PMousePointer=new CComponent(3);m.PSaveInProgress=new CComponent(10);m.SaveInProgressTime=0;m.m_pOKDialog=0;
  for(long i=0;i<n;++i){wchar_t s[3]={L'O',static_cast<wchar_t>(L'0'+i),0};m.FileNames.push_back(CWideString(s));CDateAndTime t;t.Valid=valid;m.FileTimes.push_back(t);}
  CDateAndTime extra;extra.Valid=false;m.FileTimes.push_back(extra);
  CComponent*a=new CComponent(11),*b=new CComponent(12);m.Components.push_back(a);if(findProgress)m.Components.push_back(m.PSaveInProgress);m.Components.push_back(b);
  CWideString expect;if(valid)expect=m.FileNames[index];else{wchar_t s[3]={L'S',static_cast<wchar_t>(L'0'+(Debug?n:index)),0};expect=CWideString(s);if(!Debug)expect=L"Manual - "+expect;}
  bool failed=index<n&&!Disk,early=!failed&&PathOK&&!Debug&&index==0;
  m.SaveGame(index);
  assert(FileNameCalls==(!valid?1:0));if(!valid)assert(LastFileIndex==(Debug?n:index));assert(DiskCalls==(index<n?1:0)&&PathCalls==(!failed?1:0));if(!failed)assert(wcscmp(LastPathInput.Text,expect.Text)==0);
  assert(SaveCalls==(!failed&&PathOK&&!early?1:0));assert(SetSlotCalls==(!failed&&PathOK&&!Debug&&!early?1:0));if(SetSlotCalls)assert(LastSlot==index-1);if(SaveCalls)assert(wcscmp(LastName.Text,expect.Text)==0);
  if(early){assert(TimeCalls==0&&MessageCalls==0&&EventCount==0&&Removed==0&&m.Components.size()==(findProgress?3:2)&&m.m_pOKDialog==0);}
  else{
   assert(TimeCalls==(FastTime?1:4)&&MessageCalls==(!failed?1:0)&&Removed==(findProgress?1:0));assert(m.Components.size()==3);assert(m.Components[0]==a&&m.Components[1]==b&&m.Components[2]==m.m_pOKDialog);
   const char*expectedText=HaveText?(failed?"TEXT_GUI_SAVE_FAILED":Quest?"TEXT_GUI_CONFIRM_HERO_SAVED":0):0;assert((!TextValue&&!expectedText)||(TextValue&&expectedText&&strcmp(TextValue,expectedText)==0));
   long expected[32],k=0;expected[k++]=100;if(Quest&&HaveText)expected[k++]=501;if(failed&&HaveText)expected[k++]=502;expected[k++]=101;expected[k++]=102;expected[k++]=103;expected[k++]=104;if(HaveButton){expected[k++]=105;expected[k++]=602;expected[k++]=202;}expected[k++]=701;expected[k++]=601;expected[k++]=703;expected[k++]=603;expected[k++]=203;expected[k++]=301;expected[k++]=401;assert(EventCount==k);for(long e=0;e<k;++e)assert(Events[e]==expected[e]);
   assert(LiveActions==(HaveButton?1:0)&&LiveParams==(HaveButton?1:0));if(HaveButton)assert(m.m_pOKDialog->Button.Action.GetReferenceCount()==1);delete m.m_pOKDialog;
  }
  delete a;delete b;delete m.PMousePointer;if(!findProgress||early)delete m.PSaveInProgress;
  assert(LiveComponents==0&&LiveActions==0&&LiveParams==0);
 }
 // Independently exercise the genuine helper bodies with non-null backing, zero counts,
 // last/middle erasure and overflow insertion through the same shared STLport model.
 {
  CComponent a(20),b(21);CComponent*pa=&a;CComponent*pb=&b;
  for(unsigned int n=0;n<65;++n){CComponent*data[66];for(unsigned int j=0;j<66;++j)data[j]=pb;assert(std::fill_n(data,n,pa)==data+n);for(unsigned int j=0;j<66;++j)assert(data[j]==(j<n?pa:pb));}
  for(unsigned int n=1;n<=32;n*=2)for(unsigned int i=0;i<=n;++i){std::vector<CComponent*>v;for(unsigned int j=0;j<n;++j)v.push_back(pa);v.insert(v.begin()+i,pb);assert(v.size()==n+1);for(unsigned int j=0;j<=n;++j)assert(v[j]==(j==i?pb:pa));assert(v.erase(v.begin()+i)==v.begin()+i);assert(v.size()==n);for(unsigned int j=0;j<n;++j)assert(v[j]==pa);}
  std::list<CActionParam*>q;CActionParamSLONG*a0=new CActionParamSLONG(0),*a1=new CActionParamSLONG(1),*a2=new CActionParamSLONG(2);q.push_back(a0);q.push_back(a2);std::list<CActionParam*>::iterator it=q.begin();++it;q.insert(it,a1);long expected=0;for(it=q.begin();it!=q.end();++it){assert(static_cast<CActionParamSLONG*>(*it)->Value==expected++);delete *it;}assert(expected==3);
 }
 assert(LiveComponents==0&&LiveActions==0&&LiveParams==0);
 puts("0047e196_TEST PASS");return 0;
}