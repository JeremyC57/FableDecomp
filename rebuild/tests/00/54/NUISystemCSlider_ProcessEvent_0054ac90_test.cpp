#include <vector>
#include <list>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void require(bool x,int line){if(!x){std::printf("FAIL %d case %lu\n",line,0UL);std::exit(1);}}
#define CHECK(x) require((x),__LINE__)
static unsigned bits(float f){unsigned b;std::memcpy(&b,&f,4);return b;}
static float frombits(unsigned b){float f;std::memcpy(&f,&b,4);return f;}
static bool same(float a,float b){return bits(a)==bits(b)||(a!=a&&b!=b);}
struct C2DVector{float X,Y;bool operator!=(const C2DVector& rhs)const{return!(*this==rhs);}bool operator==(const C2DVector& v)const{if(!(std::fabs(X-v.X)<=frombits(0x38d1b717)))return false;return std::fabs(Y-v.Y)<=frombits(0x38d1b717);}};
struct CRGBColour{unsigned char B,G,R,A;};
struct CCharString{int id;};
struct Entry{int code,id,move;float value;Entry(int c,int i=0,float v=0,int m=0):code(c),id(i),move(m),value(v){}};
static std::vector<Entry> calls;
static void record(int c,int i=0,float v=0,int m=0){calls.push_back(Entry(c,i,v,m));}
namespace NUISystem {
enum EEvent { UI_EVENT_SELECT=4, UI_EVENT_BACK=5, UI_EVENT_SLIDER_LEFT=20, UI_EVENT_SLIDER_RIGHT=21, UI_EVENT_MOUSE_MOVEMENT=25 };
struct CUIInterface{virtual ~CUIInterface(){};};
struct CObserver{virtual ~CObserver(){};virtual void ProcessEvent(EEvent)=0;};
struct CActionParam{virtual ~CActionParam(){};};
struct CActionParamFloat:CActionParam{float Value;explicit CActionParamFloat(float f):Value(f){}};
struct CAction{int id;std::list<CActionParam*> UIActionParams;int refs;explicit CAction(int i):id(i),refs(0){}};
}
template<class T>class CCountedPointer{T*p;public:explicit CCountedPointer(T*q=0):p(q){if(p)++p->refs;}CCountedPointer(const CCountedPointer&o):p(o.p){if(p)++p->refs;}~CCountedPointer(){if(p)--p->refs;}CCountedPointer&operator=(const CCountedPointer&o){if(o.p)++o.p->refs;if(p)--p->refs;p=o.p;return *this;}T*operator->()const{return p;}T&operator*()const{return *p;}T*Get()const{return p;}};
namespace NUISystem {
struct CUIMiscThingsDef{CCharString SoundBack,SoundForward;int refs;CUIMiscThingsDef():refs(1){SoundBack.id=71;SoundForward.id=72;}};
}
template<class T>struct CDefPointer{T*p;explicit CDefPointer(T*q):p(q){++p->refs;}CDefPointer(const CDefPointer&o):p(o.p){++p->refs;}~CDefPointer(){--p->refs;}T*operator->()const{return p;}};
namespace NUISystem {
struct CClickable{bool observing;CRGBColour colour;CClickable():observing(false){colour.B=11;colour.G=22;colour.R=33;colour.A=44;}bool IsObservingEvent(EEvent e){CHECK(e==UI_EVENT_MOUSE_MOVEMENT);record(10);return observing;}void ObserveEvent(EEvent e){CHECK(e==UI_EVENT_MOUSE_MOVEMENT);record(11);observing=true;}CRGBColour GetColour(){record(12);return colour;}void ChangeColour(CRGBColour c,float t,bool b){CHECK(same(t,0.2f)&&!b&&c.A==255&&c.B==11&&c.G==22&&c.R==33);colour=c;record(13);}};
struct CList{int id;explicit CList(int i):id(i){}void ChangeState(long n){record(20,id,float(n));}static std::list<CList*> previous;static std::list<CList*>&GetPreviousSelected(){return previous;}};
std::list<CList*> CList::previous;
struct CSlider:CUIInterface,CObserver,CActionParam{C2DVector Min,Max,Step,Value,OldValue;bool Move;std::vector<CCountedPointer<CAction> >Actions;CClickable*PAcceptButton;std::list<CCountedPointer<CAction> >*PActionsOnBack,*PActionsOnSelect;void ProcessEvent(EEvent);};
struct CManager{enum EChange{UI_CHANGE_ACCEPT=0,UI_CHANGE_REJECT=1};CSlider*slider;CAction*extra;int mode,dispatches,gets,miscgets,removes,changes;CUIMiscThingsDef misc;CManager():slider(0),extra(0),mode(0),dispatches(0),gets(0),miscgets(0),removes(0),changes(0){}static CManager*instance;static CManager*GetInstance(){++instance->gets;return instance;}void Action(CCountedPointer<CAction>& h){CAction*a=h.Get();CHECK(a&&a->refs>0);CActionParamFloat*p=static_cast<CActionParamFloat*>(a->UIActionParams.front());record(30,a->id,p->Value,slider->Move);++dispatches;if(dispatches==1){if(mode==1)slider->Actions.push_back(CCountedPointer<CAction>(extra));if(mode==2)slider->Actions.clear();if(mode==3)slider->Value.X+=0.5f;}}void PlaySoundW(CCharString&s,long n){CHECK(n==1&&misc.refs>=2);record(40,s.id);}CDefPointer<CUIMiscThingsDef>GetMiscThingsDef(){++miscgets;return CDefPointer<CUIMiscThingsDef>(&misc);}void RemoveObserver(CObserver*p){CHECK(p==static_cast<CObserver*>(slider));++removes;record(21);}void SetChange(EChange e){++changes;record(41,int(e));}};
CManager*CManager::instance=0;
void CSlider::ProcessEvent(EEvent event)
{
    switch (event) {
    case UI_EVENT_SLIDER_LEFT: {
        OldValue = Value;
        Value.X = (Value.X-Step.X >= Min.X) ? Value.X-Step.X : Min.X;
        if (OldValue != Value) {
            if (PAcceptButton && !PAcceptButton->IsObservingEvent(UI_EVENT_MOUSE_MOVEMENT)) {
                PAcceptButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
                CRGBColour colour = PAcceptButton->GetColour();
                colour.A = 255;
                PAcceptButton->ChangeColour(colour, 0.2f, false);
            }
            for (unsigned long count = 0; count < Actions.size(); ++count) {
                CActionParamFloat* pparam = static_cast<CActionParamFloat*>(Actions[count]->UIActionParams.front());
                pparam->Value = Value.X;
                CManager::GetInstance()->Action(Actions[count]);
            }
            Move = true;
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1);
        }
        break;
    }
    case UI_EVENT_SLIDER_RIGHT: {
        OldValue = Value;
        Value.X = (Value.X+Step.X <= Max.X) ? Value.X+Step.X : Max.X;
        if (OldValue != Value) {
            if (PAcceptButton && !PAcceptButton->IsObservingEvent(UI_EVENT_MOUSE_MOVEMENT)) {
                PAcceptButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
                CRGBColour colour = PAcceptButton->GetColour();
                colour.A = 255;
                PAcceptButton->ChangeColour(colour, 0.2f, false);
            }
            for (unsigned long count = 0; count < Actions.size(); ++count) {
                CActionParamFloat* pparam = static_cast<CActionParamFloat*>(Actions[count]->UIActionParams.front());
                pparam->Value = Value.X;
                CManager::GetInstance()->Action(Actions[count]);
            }
            Move = true;
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
        }
        break;
    }
    case UI_EVENT_BACK:
        if (CList::GetPreviousSelected().size() > 1) {
            CList::GetPreviousSelected().front()->ChangeState(6);
            CList::GetPreviousSelected().pop_front();
            CList::GetPreviousSelected().front()->ChangeState(3);
            CManager::GetInstance()->RemoveObserver(this);
        }
        CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1);
        CManager::GetInstance()->SetChange(CManager::UI_CHANGE_REJECT);
        if (PActionsOnBack) {
            for (std::list<CCountedPointer<CAction> >::iterator it = PActionsOnBack->begin(); it != PActionsOnBack->end(); ++it)
                CManager::GetInstance()->Action(*it);
        }
        break;
    case UI_EVENT_SELECT:
        if (CList::GetPreviousSelected().size() > 1) {
            CList::GetPreviousSelected().front()->ChangeState(6);
            CList::GetPreviousSelected().pop_front();
            CList::GetPreviousSelected().front()->ChangeState(3);
            CManager::GetInstance()->RemoveObserver(this);
        }
        CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
        CManager::GetInstance()->SetChange(CManager::UI_CHANGE_ACCEPT);
        if (PActionsOnSelect) {
            for (std::list<CCountedPointer<CAction> >::iterator it = PActionsOnSelect->begin(); it != PActionsOnSelect->end(); ++it)
                CManager::GetInstance()->Action(*it);
        }
        break;
    }
}
}
using namespace NUISystem;
static void compare(const std::vector<Entry>& want){if(calls.size()!=want.size()){std::printf("counts got%u want%u\n",unsigned(calls.size()),unsigned(want.size()));for(unsigned k=0;k<calls.size();++k)std::printf("got %d %d %08x %d\n",calls[k].code,calls[k].id,bits(calls[k].value),calls[k].move);for(unsigned k=0;k<want.size();++k)std::printf("want %d %d %08x %d\n",want[k].code,want[k].id,bits(want[k].value),want[k].move);}CHECK(calls.size()==want.size());for(unsigned i=0;i<want.size();++i){CHECK(calls[i].code==want[i].code&&calls[i].id==want[i].id&&calls[i].move==want[i].move&&same(calls[i].value,want[i].value));}}
static unsigned long scenarios=0,dispatches=0;
static void step_case(int ev,float x,float step,float y,int move,int button,int count,int mode){
 CManager mgr;CManager::instance=&mgr;CSlider s;mgr.slider=&s;mgr.mode=mode;CClickable accept;accept.observing=button==2;s.PAcceptButton=button?&accept:0;s.Min.X=-1;s.Min.Y=-10;s.Max.X=1;s.Max.Y=10;s.Step.X=step;s.Step.Y=99;s.Value.X=x;s.Value.Y=y;s.OldValue.X=77;s.OldValue.Y=88;s.Move=move!=0;s.PActionsOnBack=s.PActionsOnSelect=0;
 CAction a0(0),a1(1),a2(2),extra(3);CAction*as[4]={&a0,&a1,&a2,&extra};CActionParamFloat p0(-64),p1(-64),p2(-64),p3(-64),sentinel(123);CActionParamFloat*ps[4]={&p0,&p1,&p2,&p3};for(int i=0;i<4;++i){as[i]->UIActionParams.push_back(ps[i]);as[i]->UIActionParams.push_back(&sentinel);}for(int i=0;i<count;++i)s.Actions.push_back(CCountedPointer<CAction>(as[i]));mgr.extra=&extra;
 float v=ev==UI_EVENT_SLIDER_LEFT?float(x-step):float(x+step);v=ev==UI_EVENT_SLIDER_LEFT?(v>=-1?v:-1):(v<=1?v:1);bool changed=((bits(x)&0x7f800000U)==0x7f800000U)||((bits(v)&0x7f800000U)==0x7f800000U)||((bits(y)&0x7f800000U)==0x7f800000U);if(!changed)changed=std::fabs(double(x)-double(v))>double(frombits(0x38d1b717));float finalv=v;float expected[4]={-64,-64,-64,-64};std::vector<Entry>want;int n=changed?count:0;if(changed&&button){want.push_back(Entry(10));if(button==1){want.push_back(Entry(11));want.push_back(Entry(12));want.push_back(Entry(13));}}
 for(int i=0;i<n;++i){expected[i]=finalv;want.push_back(Entry(30,i,finalv,move));if(i==0){if(mode==1)n=count+1;if(mode==2)n=1;if(mode==3)finalv+=0.5f;}}
 // Extra has id3, even when an initial vector of one or zero actions grows.
 if(changed&&count&&mode==1){for(unsigned i=0;i<want.size();++i)if(want[i].code==30&&want[i].id==count){want[i].id=3;expected[3]=want[i].value;if(count!=3)expected[count]=-64;}}
 if(changed)want.push_back(Entry(40,ev==UI_EVENT_SLIDER_LEFT?71:72));calls.clear();CObserver*obs=&s;obs->ProcessEvent(EEvent(ev));if(calls.size()!=want.size())std::printf("case%lu ev%d x%08x st%08x y%08x m%d b%d c%d mode%d changed%d\n",scenarios,ev,bits(x),bits(step),bits(y),move,button,count,mode,int(changed));compare(want);CHECK(same(s.Value.X,finalv)&&same(s.Value.Y,y)&&same(s.OldValue.X,x)&&same(s.OldValue.Y,y)&&s.Move==(changed?true:move!=0));for(int i=0;i<4;++i)CHECK(same(ps[i]->Value,expected[i]));CHECK(sentinel.Value==123);int actualn=changed?(count?(mode==1?count+1:mode==2?1:count):0):0;CHECK(mgr.dispatches==actualn&&mgr.gets==(changed?actualn+2:0)&&mgr.miscgets==(changed?1:0)&&mgr.removes==0&&mgr.changes==0&&mgr.misc.refs==1);CHECK(accept.observing==(button==2||(changed&&button==1)));CHECK(accept.colour.A==(changed&&button==1?255:44));s.Actions.clear();for(int i=0;i<4;++i)CHECK(as[i]->refs==0);++scenarios;dispatches+=mgr.dispatches;
}
static void finish_case(int ev,int depth,int listmode,int count,int move){CManager mgr;CManager::instance=&mgr;CSlider s;mgr.slider=&s;s.PAcceptButton=0;s.Value.X=3;s.Value.Y=4;s.OldValue.X=5;s.OldValue.Y=6;s.Move=move!=0;CList l0(0),l1(1),l2(2),l3(3);CList*ls[4]={&l0,&l1,&l2,&l3};CList::previous.clear();for(int i=0;i<depth;++i)CList::previous.push_back(ls[i]);CAction a0(10),a1(11),a2(12);CAction*as[3]={&a0,&a1,&a2};CActionParamFloat p0(-10),p1(-11),p2(-12);CActionParamFloat*ps[3]={&p0,&p1,&p2};std::list<CCountedPointer<CAction> > list;for(int i=0;i<3;++i)as[i]->UIActionParams.push_back(ps[i]);for(int i=0;i<count;++i)list.push_back(CCountedPointer<CAction>(as[i]));s.PActionsOnBack=listmode?&list:0;s.PActionsOnSelect=listmode?&list:0;std::vector<Entry>want;bool finish=ev==UI_EVENT_BACK||ev==UI_EVENT_SELECT;if(finish){if(depth>1){want.push_back(Entry(20,0,6));want.push_back(Entry(20,1,3));want.push_back(Entry(21));}want.push_back(Entry(40,ev==UI_EVENT_BACK?71:72));want.push_back(Entry(41,ev==UI_EVENT_BACK?1:0));if(listmode)for(int i=0;i<count;++i)want.push_back(Entry(30,10+i,float(-10-i),move));}calls.clear();static_cast<CObserver*>(&s)->ProcessEvent(EEvent(ev));compare(want);CHECK(s.Value.X==3&&s.Value.Y==4&&s.OldValue.X==5&&s.OldValue.Y==6&&s.Move==(move!=0));CHECK(int(CList::previous.size())==depth-(finish&&depth>1?1:0));if(!CList::previous.empty())CHECK(CList::previous.front()==ls[finish&&depth>1?1:0]);int n=finish&&listmode?count:0;CHECK(mgr.dispatches==n&&mgr.removes==(finish&&depth>1?1:0)&&mgr.changes==(finish?1:0)&&mgr.miscgets==(finish?1:0)&&mgr.gets==(finish?n+3+(depth>1?1:0):0)&&mgr.misc.refs==1);for(int i=0;i<3;++i)CHECK(ps[i]->Value==float(-10-i));list.clear();for(int i=0;i<3;++i)CHECK(as[i]->refs==0);CList::previous.clear();++scenarios;dispatches+=mgr.dispatches;}
int main(){float xs[]={-2,-1,-0.0f,0,0.5f,1,2,frombits(0x7f800000),frombits(0xff800000),frombits(0x7fc00000)};float steps[]={0,0.25f,-0.25f,2,-2,frombits(0x38d1b716),frombits(0x38d1b717),frombits(0x38d1b718),frombits(0x7f800000),frombits(0x7fc00000)};float ys[]={-0.0f,3,frombits(0x7fc00000),frombits(0x7f800000)};int counts[]={0,1,3};for(int e=20;e<=21;++e)for(int x=0;x<10;++x)for(int st=0;st<10;++st)for(int y=0;y<4;++y)for(int m=0;m<2;++m)for(int b=0;b<3;++b)for(int c=0;c<3;++c)for(int mode=0;mode<4;++mode)step_case(e,xs[x],steps[st],ys[y],m,b,counts[c],mode);int events[]={4,5,0,1,3,6,19,22,30,255};for(int e=0;e<10;++e)for(int d=0;d<=4;++d)for(int lm=0;lm<2;++lm)for(int n=0;n<=3;++n)for(int m=0;m<2;++m)finish_case(events[e],d,lm,n,m);CHECK(scenarios==58400&&dispatches==69816);std::puts("0054ac90_TEST PASS");return 0;}