// 005fc2d0 NScreen005fc2d0::Manager::Construct
// Flags: /O2 /Oy /W3
// Leaf: Construct
#include <stdlib.h>
#include <new>
namespace NScreen005fc2d0 {
#pragma pack(push,1)
struct StringData{char* text;int length,capacity;unsigned char owned;int refs;};
#pragma pack(pop)
extern int StringInstances;StringData* NewStringData(const char*,int);void DeleteStringData(StringData*);
struct String {void* data;String(const char*,int=-1);__forceinline String(const String& x):data(x.data){++StringInstances;if(data)++((StringData*)data)->refs;}~String();const char* Text()const{return data?((StringData*)data)->text:"";}};

struct Tag{Tag(){}};struct Vector {void **first,**last,**end;Vector():first(0),last(0),end(0){}__forceinline ~Vector(){if(first)free(first);}template<class T> __declspec(noinline) void Overflow(void** pos,void*const& value,const T&,int count,int skipTail){unsigned old=first?(unsigned)(last-first):0;unsigned cap=old+(old<(unsigned)count?(unsigned)count:old);void** fresh=(void**)malloc(cap*sizeof(void*));void** out=fresh;for(void** i=first;i!=pos;++i)*out++=*i;for(int j=0;j<count;++j)*out++=value;if(!skipTail)for(void** k=pos;k!=last;++k)*out++=*k;free(first);first=fresh;last=out;end=fresh+cap;}__forceinline void Push(void*const& p){if(last!=end){if(last)new(last)void*(p);++last;}else Overflow(last,p,Tag(),1,1);}};
struct PtrBase {void* table;void* p;void* Get();};
struct Node {Node* next;Node* prev;PtrBase value;};struct List {Node* head;__forceinline unsigned Size()const{unsigned n=0;for(Node* p=head->next;p!=head;p=p->next)++n;return n;}};
struct CopiedList:List {CopiedList(const List&);~CopiedList();};
struct Global {char pad[92];List current,past;};extern Global* global;
struct Widget;void __fastcall DestroyWidget(void*);struct Block {int refs;void(__fastcall*destroy)(void*);void* object;Block(void* p):refs(1),destroy(DestroyWidget),object(p){}};
struct Ptr {Widget* p;Block* b;__forceinline Ptr(Widget* w):p(w),b(p?new Block(p):0){}__forceinline ~Ptr(){if(b&&!--b->refs){b->destroy(b->object);delete b;}p=0;b=0;}};
struct Widget {
virtual void v0()=0;
virtual void v1()=0;
virtual void v2()=0;
virtual void v3()=0;
virtual void v4()=0;
virtual void v5()=0;
virtual void v6()=0;
virtual void v7()=0;
virtual void v8()=0;
virtual void v9()=0;
virtual void v10()=0;
virtual void v11()=0;
virtual void v12()=0;
virtual void v13()=0;
virtual void v14()=0;
virtual void v15()=0;
virtual void v16()=0;
virtual void v17()=0;
virtual void v18()=0;
virtual void v19()=0;
virtual void v20()=0;
virtual void v21()=0;
virtual void v22()=0;
virtual void v23()=0;
virtual void v24()=0;
virtual void v25()=0;
virtual void v26()=0;
virtual void v27()=0;
virtual void v28()=0;
virtual void v29()=0;
virtual void v30()=0;
virtual void v31()=0;
virtual void v32()=0;
virtual void v33()=0;
virtual void v34()=0;
virtual void v35()=0;
virtual void v36()=0;
virtual void v37()=0;
virtual void v38()=0;
virtual void v39()=0;
virtual void v40()=0;
virtual void v41()=0;
virtual void v42()=0;
virtual void v43()=0;
virtual void v44()=0;
virtual void v45()=0;
virtual void v46()=0;
virtual void v47()=0;
virtual void v48()=0;
virtual void v49()=0;
virtual void v50()=0;
virtual void v51()=0;
virtual void v52()=0;
virtual void v53()=0;
virtual void v54()=0;
virtual void v55()=0;
virtual void v56()=0;
virtual void v57()=0;
virtual void v58()=0;
virtual void v59()=0;
virtual void v60()=0;
virtual void v61()=0;
virtual void v62()=0;
virtual void v63()=0;
virtual void v64()=0;
virtual void v65()=0;
virtual void v66()=0;
virtual void v67()=0;
virtual void v68()=0;
virtual void v69()=0;
virtual void v70()=0;
virtual void v71()=0;
virtual void v72()=0;
virtual void v73()=0;
virtual void v74()=0;
virtual void v75()=0;
virtual void v76()=0;
virtual void v77()=0;
virtual void v78()=0;
virtual void v79()=0;
virtual void v80()=0;
virtual void v81()=0;
virtual void v82()=0;
virtual void v83()=0;
virtual void v84()=0;
virtual void v85()=0;
virtual void v86()=0;
virtual void v87()=0;
virtual void v88()=0;
virtual void v89()=0;
virtual void v90()=0;
virtual void v91()=0;
virtual void v92()=0;
virtual void v93()=0;
virtual void v94()=0;
virtual void v95()=0;
virtual void v96()=0;
virtual void v97()=0;
virtual void v98()=0;
virtual void v99()=0;
virtual void v100()=0;
virtual void v101()=0;
virtual void v102()=0;
virtual void v103()=0;
virtual void v104()=0;
virtual void v105()=0;
virtual void v106()=0;
virtual void v107()=0;
virtual void v108()=0;
virtual void v109()=0;
virtual void v110()=0;
virtual void v111()=0;
virtual void v112()=0;
virtual void v113()=0;
virtual void v114()=0;
virtual void v115()=0;
virtual void v116()=0;
virtual void v117()=0;
virtual void v118()=0;
virtual void v119()=0;
virtual void v120()=0;
virtual void v121()=0;
virtual void v122()=0;
virtual void v123()=0;
virtual void Attach(Widget*)=0;
virtual void v125()=0;
virtual void v126()=0;
virtual void v127()=0;
virtual void v128()=0;
virtual void v129()=0;
virtual void v130()=0;
virtual void v131()=0;
virtual void v132()=0;
virtual void v133()=0;
virtual void v134()=0;
virtual void v135()=0;
virtual void v136()=0;
virtual void v137()=0;
virtual void v138()=0;
virtual void v139()=0;
virtual void v140()=0;
virtual void v141()=0;
virtual void v142()=0;
virtual void v143()=0;
virtual void v144()=0;
virtual void Add(const Ptr&)=0;
};
struct UI {__declspec(noinline) Widget* Find(const String&,int);};UI* __fastcall GetUI();
Widget* LookupWidget(UI*,const char*,int);
Widget* UI::Find(const String& name,int flag){return LookupWidget(this,name.Text(),flag);}
struct Def {virtual void v0()=0;virtual void Delete()=0;int refs;};
struct Ref {Def* p;Ref():p(0){}__forceinline ~Ref(){Def* d=p;if(d&&!--d->refs)d->Delete();}};
struct Database {__declspec(noinline) int Index(const String&,bool);__declspec(noinline) bool Get(int,Def*&);};Database* __fastcall DatabaseInstance();
int LookupDefinitionIndex(Database*,const char*,bool);Def* LookupDefinition(Database*,int);
int Database::Index(const String& name,bool flag){return LookupDefinitionIndex(this,name.Text(),flag);}
bool Database::Get(int index,Def*& out){if(index<=0)return false;Def* d=LookupDefinition(this,index);if(!d)return false;if(out!=d){if(out){if(!--out->refs)out->Delete();out=0;}out=d;++d->refs;}if(!--d->refs)d->Delete();return true;}
struct Component {void Populate(Vector&,Widget*,int,int);void PopulatePC(int,Vector&,Widget*,int,int);};
struct Pair {int key;Component* component;};struct Map {Pair *first,*last,*end;__declspec(noinline) Pair* Lower(const int& key){Pair* p=first;int n=last-first;while(n>0){int half=n/2;Pair* middle=p+half;if(middle->key<key){p=middle+1;n-=half+1;}else n=half;}return p;}};struct Thing {char pad[32];unsigned flags;char gap[32];Map map;};
struct Parent {virtual void v0()=0;virtual void v1()=0;virtual void v2()=0;virtual Widget* Find(String)=0;};
// Layout: receiver thing+4, thing flags+0x20/map+0x44; current/past lists global+0x5c/+0x60.
struct Manager {void* table;Thing* thing;Widget* Construct(int arg);};
Widget* Manager::Construct(int arg){
 Component* component=0;if(thing->flags&0x2000000){Map* map=&thing->map;int key=25;Pair* p=map->Lower(key);if(p==map->last||key<p->key)p=map->last;component=p->component;}
 List& list=global->current;
 Vector items;for(Node* n=list.head->next;n!=list.head;n=n->next){void* p=n->value.Get();items.Push(p);}if(list.Size()<=0u)return 0;
 Ref def;int index=DatabaseInstance()->Index("UI_MISC_THINGS_DEF",true);DatabaseInstance()->Get(index,def.p);UI* ui=GetUI();Widget* widget=ui->Find("UI_SUB_LIST",0);Ptr title(ui->Find("UI_TITLE_QUEST_CURRENT",0));widget->Add(title);
 component->Populate(items,widget,1,arg);
 return widget;
}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
namespace NScreen005fc2d0 {
int StringInstances;static int dataLive,destroyed,adds,attaches,populates,parents,finds,definitions,defDeleted;static bool noTitle,noDef;static int payload[2]={7,9};
StringData* NewStringData(const char* p,int n){if(n<0)n=strlen(p);StringData* d=new StringData;d->text=new char[n+1];memcpy(d->text,p,n);d->text[n]=0;d->length=n;d->capacity=n;d->owned=1;d->refs=1;++dataLive;return d;}
void DeleteStringData(StringData* d){assert(d->refs<=0);delete[]d->text;d->text=0;d->length=0;d->capacity&=0x80000000;delete d;--dataLive;}
String::String(const char* p,int n):data(0){++StringInstances;if(p&&*p)data=NewStringData(p,n);}
String::~String(){if(data){StringData*d=(StringData*)data;if(--d->refs<=0)DeleteStringData(d);data=0;}--StringInstances;}
struct Link {void* object;unsigned refs;};static int liveLinks;
static Link* MakeLink(void* object,unsigned refs){Link* link=new Link;link->object=object;link->refs=refs;++liveLinks;return link;}
void* PtrBase::Get(){Link* link=(Link*)p;if(link){if(link->object)return link->object;--link->refs;link=(Link*)p;if(!link->refs&&!link->object){delete link;--liveLinks;}p=0;}return 0;}
CopiedList::CopiedList(const List& x){head=(Node*)malloc(sizeof(Node));head->next=head->prev=head;for(Node* i=x.head->next;i!=x.head;i=i->next){Node* p=(Node*)malloc(sizeof(Node));*p=*i;p->prev=head->prev;p->next=head;head->prev->next=p;head->prev=p;}}
CopiedList::~CopiedList(){Node* p=head->next;while(p!=head){Node* q=p->next;free(p);p=q;}free(head);}
Global g;Global* global=&g;static Block* held;static Widget* heldWidget;
struct TestWidget:Widget {void v0(){}void v1(){}void v2(){}void v3(){}void v4(){}void v5(){}void v6(){}void v7(){}void v8(){}void v9(){}void v10(){}void v11(){}void v12(){}void v13(){}void v14(){}void v15(){}void v16(){}void v17(){}void v18(){}void v19(){}void v20(){}void v21(){}void v22(){}void v23(){}void v24(){}void v25(){}void v26(){}void v27(){}void v28(){}void v29(){}void v30(){}void v31(){}void v32(){}void v33(){}void v34(){}void v35(){}void v36(){}void v37(){}void v38(){}void v39(){}void v40(){}void v41(){}void v42(){}void v43(){}void v44(){}void v45(){}void v46(){}void v47(){}void v48(){}void v49(){}void v50(){}void v51(){}void v52(){}void v53(){}void v54(){}void v55(){}void v56(){}void v57(){}void v58(){}void v59(){}void v60(){}void v61(){}void v62(){}void v63(){}void v64(){}void v65(){}void v66(){}void v67(){}void v68(){}void v69(){}void v70(){}void v71(){}void v72(){}void v73(){}void v74(){}void v75(){}void v76(){}void v77(){}void v78(){}void v79(){}void v80(){}void v81(){}void v82(){}void v83(){}void v84(){}void v85(){}void v86(){}void v87(){}void v88(){}void v89(){}void v90(){}void v91(){}void v92(){}void v93(){}void v94(){}void v95(){}void v96(){}void v97(){}void v98(){}void v99(){}void v100(){}void v101(){}void v102(){}void v103(){}void v104(){}void v105(){}void v106(){}void v107(){}void v108(){}void v109(){}void v110(){}void v111(){}void v112(){}void v113(){}void v114(){}void v115(){}void v116(){}void v117(){}void v118(){}void v119(){}void v120(){}void v121(){}void v122(){}void v123(){}void v125(){}void v126(){}void v127(){}void v128(){}void v129(){}void v130(){}void v131(){}void v132(){}void v133(){}void v134(){}void v135(){}void v136(){}void v137(){}void v138(){}void v139(){}void v140(){}void v141(){}void v142(){}void v143(){}void v144(){} void Add(const Ptr& p){++adds;assert(!held);held=p.b;heldWidget=p.p;if(held){assert(held->refs==1&&held->object==p.p);++held->refs;}}void Attach(Widget* p){++attaches;assert(p==heldWidget);}};
static TestWidget sublist,frame;static UI ui;static Database database;static TestWidget* title;
static int heapDefDeleted;struct TestDef:Def{bool heap;TestDef():heap(false){}~TestDef(){if(heap)++heapDefDeleted;}void v0(){}void Delete(){assert(refs==0);++defDeleted;if(heap)delete this;}};static TestDef def;
void __fastcall DestroyWidget(void* p){assert(p==title);delete (TestWidget*)p;title=0;++destroyed;}
Widget* LookupWidget(UI* u,const char* name,int flag){assert(u==&ui&&flag==0);++finds;if(!strcmp(name,"UI_SUB_LIST"))return &sublist;assert(!strcmp(name,"UI_TITLE_QUEST_CURRENT"));if(noTitle)return 0;assert(!title);title=new TestWidget;return title;}
UI* __fastcall GetUI(){return &ui;}
int LookupDefinitionIndex(Database* d,const char* name,bool flag){assert(d==&database&&!strcmp(name,"UI_MISC_THINGS_DEF")&&flag);return 12;}
Def* LookupDefinition(Database* d,int index){assert(d==&database&&index==12);++definitions;if(noDef)return 0;++def.refs;return &def;}
Database* __fastcall DatabaseInstance(){return &database;}
void Component::Populate(Vector& v,Widget* w,int one,int arg){assert(v.last-v.first==2&&v.first[0]==&payload[0]&&v.first[1]==&payload[1]&&w==&sublist&&one==1&&arg==42);++populates;}
void Component::PopulatePC(int arg,Vector& v,Widget* w,int one,int parent){assert(arg==42&&v.last-v.first==2&&v.first[0]==&payload[0]&&v.first[1]==&payload[1]&&w==&sublist&&one==1&&parent);++populates;}
struct TestParent:Parent{void v0(){}void v1(){}void v2(){}Widget* Find(String name){assert(!strcmp(name.Text(),"PC_UI_FRAME_QUESTS"));++parents;return &frame;}};
static void ReleaseHeld(){if(held){assert(held->refs==1);if(!--held->refs)held->destroy(held->object);delete held;held=0;}heldWidget=0;}
}
int main(){using namespace NScreen005fc2d0;
{PtrBase empty;empty.table=0;empty.p=0;assert(empty.Get()==0&&empty.p==0);
PtrBase final;final.table=0;final.p=MakeLink(0,1);assert(final.Get()==0&&final.p==0&&liveLinks==0);
Link* shared=MakeLink(&payload[0],2);PtrBase first,second;first.table=second.table=0;first.p=second.p=shared;
assert(first.Get()==&payload[0]&&second.Get()==&payload[0]&&shared->refs==2);
shared->object=0;assert(first.Get()==0&&first.p==0&&shared->refs==1&&liveLinks==1);
assert(second.Get()==0&&second.p==0&&liveLinks==0);}

{TestDef* prior=new TestDef;prior->heap=true;prior->refs=1;Def* out=prior;def.refs=1;noDef=false;
assert(!database.Get(0,out)&&out==prior&&prior->refs==1&&definitions==0);
noDef=true;assert(!database.Get(12,out)&&out==prior&&prior->refs==1&&def.refs==1&&definitions==1);
noDef=false;assert(database.Get(12,out)&&out==&def&&heapDefDeleted==1&&defDeleted==1&&def.refs==2);
assert(database.Get(12,out)&&out==&def&&def.refs==2&&defDeleted==1);
assert(--out->refs==1);out=0;
assert(database.Get(12,out)&&out==&def&&def.refs==2);assert(--out->refs==1);out=0;
definitions=0;defDeleted=0;}
{String* zero=new String("zero");((StringData*)zero->data)->refs=0;((StringData*)zero->data)->owned=0;delete zero;}assert(StringInstances==0&&dataLive==0);{String base("shared");{String copy(base);assert(copy.data==base.data&&((StringData*)base.data)->refs==2);}assert(((StringData*)base.data)->refs==1);}assert(StringInstances==0&&dataLive==0);Manager m;Thing thing;memset(&m,0,sizeof(m));memset(&thing,0,sizeof(thing));m.thing=&thing;Node sentinel,nodes[2];memset(&sentinel,0,sizeof(sentinel));memset(nodes,0,sizeof(nodes));sentinel.next=sentinel.prev=&sentinel;g.current.head=g.past.head=&sentinel;TestParent parent;assert(!m.Construct(42));assert(!finds&&!definitions&&!populates&&StringInstances==0&&dataLive==0);
Component component;Pair map[2];map[0].key=25;map[0].component=&component;map[1].key=1000;map[1].component=&component;thing.flags=0x2000000;thing.map.first=map;thing.map.last=map+1;thing.map.end=map+2;sentinel.next=&nodes[0];sentinel.prev=&nodes[1];nodes[0].next=&nodes[1];nodes[0].prev=&sentinel;nodes[1].next=&sentinel;nodes[1].prev=&nodes[0];nodes[0].value.p=MakeLink(&payload[0],1);nodes[1].value.p=MakeLink(&payload[1],1);def.refs=1;
assert(m.Construct(42)==&sublist);assert(adds==1&&populates==1&&finds==2&&definitions==1&&held&&held->refs==1&&def.refs==1&&!defDeleted&&StringInstances==0&&dataLive==0);assert(attaches==0&&parents==0);ReleaseHeld();assert(destroyed==1&&!title);
noTitle=true;assert(m.Construct(42)==&sublist);assert(adds==2&&populates==2&&finds==4&&definitions==2&&!held&&!heldWidget&&def.refs==1&&StringInstances==0&&dataLive==0);assert(attaches==0&&parents==0);assert(destroyed==1);noTitle=false;noDef=true;assert(m.Construct(42)==&sublist);assert(adds==3&&populates==3&&finds==6&&definitions==3&&held&&held->refs==1&&def.refs==1&&!defDeleted&&StringInstances==0&&dataLive==0);assert(attaches==0&&parents==0);ReleaseHeld();assert(destroyed==2&&!title);for(int i=0;i<2;++i){Link* link=(Link*)nodes[i].value.p;assert(link&&link->refs==1&&link->object==&payload[i]);link->object=0;assert(nodes[i].value.Get()==0&&nodes[i].value.p==0);}assert(liveLinks==0);puts("005fc2d0_TEST PASS");return 0;}