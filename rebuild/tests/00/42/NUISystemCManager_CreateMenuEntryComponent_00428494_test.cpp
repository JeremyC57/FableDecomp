
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "engine/NUISystem_enums.h"
static void ck(bool ok) { if(!ok) exit(1); }
class CCharString: public std::string { public: CCharString(const char* x=""):std::string(x){} };
class CWideString { public: int value; CWideString(int n):value(n){} };
template<class T> class CCountedPointer {
public:
    T* p;
    CCountedPointer(T* t=0):p(t){}
    void Reset(T* t){p=t;}
    T* operator->() const{return p;}
    bool IsNull() const{return !p;}
    template<class U> void SetStaticCast(const CCountedPointer<U>& q){p=static_cast<T*>(q.p);}
};
namespace NUISystem {
struct Vec2 {float X,Y;};struct Colour4 {unsigned char A;};
struct State {Vec2 Position;Colour4 Colour;State(){Position.X=7;Position.Y=-9;Colour.A=91;}};
class CComponent {
public:
    std::map<unsigned long,State> states; std::vector<CComponent*> children; CCharString name,definition; bool pc;
    CComponent():pc(false){} virtual ~CComponent(){}
    std::map<unsigned long,State>& GetStates(){return states;}
    bool IsPCStyle() const{return pc;}
    void AddChild(CCountedPointer<CComponent>& q){children.push_back(q.p);}
    void SetName(const CCharString& n){name=n;}
};
class CText:public CComponent{public:int text;CText():text(-4){} void SetValue(const CWideString& t,bool a,bool b){ck(a&&b);text=t.value;}};
class CManager{
public:
    std::map<CCharString,CCountedPointer<CComponent> > BastardChildren;
    std::vector<CComponent*> created; bool pc;
    CComponent* CreateComponent(const CCharString& d,bool flag){ck(!flag);CComponent* p=new CText;p->pc=pc;p->definition=d;created.push_back(p);return p;}
    ~CManager(){for(unsigned int i=0;i<created.size();++i) delete created[i];}
    CComponent* CreateMenuEntryComponent(const CWideString&,float,float,const CCharString&,float,const CCharString,float,CCharString);
};
CComponent* CManager::CreateMenuEntryComponent(const CWideString& text, float position,
    float alpha, const CCharString& name, float position_x, CCharString text_def,
    float text_position, CCharString button_def)
{
    if (BastardChildren.find(button_def) == BastardChildren.end()) {
        CCountedPointer<CComponent> cpbastard_child(CreateComponent(button_def,false));
        BastardChildren[button_def] = cpbastard_child;
    } else if (BastardChildren[button_def].IsNull()) {
        BastardChildren[button_def].Reset(CreateComponent(button_def,false));
    }
    CCountedPointer<CComponent> pcomponent;
    if (text_def == "")
        pcomponent.Reset(CreateComponent("UI_WEAPON_TEXT_TEMPLATE",false));
    else
        pcomponent.Reset(CreateComponent(text_def,false));
    CCountedPointer<CText> ptext;
    pcomponent->GetStates()[UI_STATE_INACTIVE].Position.X += text_position;
    pcomponent->GetStates()[UI_STATE_SELECTED].Position.X += text_position;
    pcomponent->GetStates()[UI_STATE_UNSELECTED].Position.X += text_position;
    ptext.SetStaticCast(CCountedPointer<CComponent>(pcomponent));
    ptext->SetValue(text,true,true);
    ptext->SetName("MENU_ENTRY_TEXT");
    CCountedPointer<CComponent> pbutton(CreateComponent("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE",false));
    pbutton->AddChild(BastardChildren[button_def]);
    pbutton->AddChild(pcomponent);
    CComponent* pmenu_entry = CreateComponent(name,false);
    pmenu_entry->AddChild(pbutton);
    pmenu_entry->GetStates()[UI_STATE_ACTIVE].Position.Y = position;
    pmenu_entry->GetStates()[UI_STATE_UNSELECTED].Position.Y = position;
    pmenu_entry->GetStates()[UI_STATE_LIST_ACTIVE].Position.Y = position;
    if (pmenu_entry->IsPCStyle()) {
        pmenu_entry->GetStates()[UI_STATE_INACTIVE].Position.Y = position;
        pmenu_entry->GetStates()[UI_STATE_SELECTED].Position.Y = position;
    }
    pmenu_entry->GetStates()[UI_STATE_ACTIVE].Position.X = position_x;
    pmenu_entry->GetStates()[UI_STATE_UNSELECTED].Position.X = position_x;
    pmenu_entry->GetStates()[UI_STATE_LIST_ACTIVE].Position.X = position_x;

    if (alpha < 0.0f) alpha=0.0f;
    if (alpha > 1.0f) alpha=1.0f;
    pmenu_entry->GetStates()[UI_STATE_ACTIVE].Colour.A = static_cast<unsigned char>(alpha * 255.0f);
    pmenu_entry->GetStates()[UI_STATE_UNSELECTED].Colour.A = static_cast<unsigned char>(alpha * 255.0f);
    pmenu_entry->GetStates()[UI_STATE_LIST_ACTIVE].Colour.A = static_cast<unsigned char>(alpha * 255.0f);
    return pmenu_entry;
}
}


using namespace NUISystem;
int main(){
int cases=0;const float alphas[]={-2.5f,0,0.25f,0.5f,0.999f,1,1.5f};
for(int cache=0;cache<3;++cache)for(int empty=0;empty<2;++empty)for(int pc=0;pc<2;++pc)
for(int ia=0;ia<7;++ia)for(int n=0;n<6;++n){
    CManager m;m.pc=pc!=0;CComponent cache_entry; const char* txt=empty?"":"custom_text";
    if(cache==1)m.BastardChildren["button"].Reset(0);
    if(cache==2)m.BastardChildren["button"].Reset(&cache_entry);
    float x=n-2.5f,y=3.5f-n,off=0.3f*n;CComponent* p=m.CreateMenuEntryComponent(CWideString(12+n),y,alphas[ia],"menu",x,txt,off,"button");
    ck(p->definition=="menu"&&p->children.size()==1);
    CComponent* button=p->children[0];ck(button->definition=="PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE"&&button->children.size()==2);
    ck(button->children[0]==m.BastardChildren["button"].p);
    if (cache==2)ck(button->children[0]==&cache_entry);else ck(button->children[0]->definition=="button");
    CText* text=static_cast<CText*>(button->children[1]);ck(text->definition==(empty?"UI_WEAPON_TEXT_TEMPLATE":txt)&&text->name=="MENU_ENTRY_TEXT"&&text->text==12+n);
    ck(m.created.size()==(cache==2?3u:4u));
    for(int i=0;i<9;i++){State& ts=text->states[i];ck(ts.Position.X==((i==0||i==3||i==4)?7+off:7));
      State& es=p->states[i];bool normal=i==1||i==4||i==5;ck(es.Position.X==(normal?x:7));
      ck(es.Position.Y==((normal||(pc&&(i==0||i==3)))?y:-9));
      unsigned char expected=alphas[ia]<=0?0:alphas[ia]>=1?255:static_cast<unsigned char>(alphas[ia]*255);
      ck(es.Colour.A==(normal?expected:91));
    }
    ++cases;
}ck(cases==504);puts("00428494_TEST PASS");return 0;}