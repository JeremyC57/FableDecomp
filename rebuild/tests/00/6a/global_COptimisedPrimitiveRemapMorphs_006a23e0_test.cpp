// 006a23e0 NRemap006a23e0::Manager::Remap
// Flags: /O2 /Oy /W3
// Leaf: ?Remap@Manager@NRemap006a23e0@@QAEXXZ
#include <stdlib.h>
#include <new>
namespace NRemap006a23e0 {
struct Control;struct Object {int id;Control* control;};struct Control{Object* object;int refs;};
struct Base {mutable Control* control;__declspec(noinline) Base();__declspec(noinline) virtual ~Base();__declspec(noinline) Object* Get()const;__declspec(noinline) void Set(Object*);};
// Layout: A01b10/A01b50/A01b90/A01c10 use weak Control8 at Base+4; Object owns its control pointer+4. Base destructor clears control and releases only expired last-reference blocks.

struct Ptr:Base {__forceinline Ptr(Object* p){Set(p);}__forceinline Ptr(const Ptr& p){Set(p.Get());}};
#pragma pack(push,1)
struct Entry {Ptr object;bool active;Entry(Object* p):object(p),active(true){}__forceinline Entry(const Entry& e):object(e.object),active(e.active){}__forceinline ~Entry(){}};
#pragma pack(pop)
__forceinline void DestroyRange(Entry* first,Entry* last){for(;first!=last;++first)first->~Entry();}
struct Tag{Tag(){}};struct Entries {Entry *first,*last,*end;Entries():first(0),last(0),end(0){}__forceinline ~Entries(){DestroyRange(first,last);if(first)free(first);}template<class T> __declspec(noinline) void Overflow(Entry* pos,const Entry& value,const T&,int count,int skipTail){unsigned n=first?last-first:0;unsigned cap=n+(n<(unsigned)count?(unsigned)count:n);Entry* fresh=(Entry*)malloc(cap*sizeof(Entry));Entry* out=fresh;Entry* i;for(i=first;i!=pos;++i)new(out++)Entry(*i);for(int k=0;k<count;++k)new(out++)Entry(value);if(!skipTail)for(i=pos;i!=last;++i)new(out++)Entry(*i);for(i=first;i!=last;++i)i->~Entry();free(first);first=fresh;last=out;end=fresh+cap;}__forceinline void Push(const Entry& e){if(last!=end){new(last)Entry(e);++last;}else Overflow(last,e,Tag(),1,1);}};
struct Objects {Object **first,**last,**end;Objects():first(0),last(0),end(0){}~Objects(){if(first)free(first);}};
struct Target {void Apply(Entries&,int);};struct Scene {char pad[6500];Target* target;};struct Root {char pad[24];Scene* scene;};
// Layout: receiver key +0x20; GetRoot returns scene pointer +0x18, target +0x1964.
struct Record {int key;Base object;};
struct Manager {char pad[12];Record* first;Record* last;Record* end;char gap[8];int key;__declspec(noinline) void Gather(Objects&);Root* GetRoot();void Remap();};
// Layout: Gather6a2220 reads records+0xc/+0x10 stride12; Base receiver is record+4. Packed Entry stride9 proven6a3950.
void Manager::Gather(Objects& out){for(unsigned i=0;i<(unsigned)(last-first);++i){if(first[i].object.Get()){Object* x=first[i].object.Get();if(out.last==out.end){unsigned n=out.first?out.last-out.first:0;unsigned cap=n?n*2:1;Object** p=(Object**)malloc(cap*sizeof(Object*));for(unsigned k=0;k<n;++k)p[k]=out.first[k];free(out.first);out.first=p;out.last=p+n;out.end=p+cap;}*out.last++=x;}}}
void Manager::Remap(){Entries entries;Objects objects;Gather(objects);for(Object** p=objects.first;p!=objects.last;++p)entries.Push(Entry(*p));Root* root=GetRoot();Target* target=root->scene->target;target->Apply(entries,key);}
}

namespace NRemap006a23e0 {
Base::Base():control(0){}
Base::~Base(){if(control){--control->refs;if(!control->refs&&!control->object)delete control;control=0;}}
Object* Base::Get()const{if(control){if(control->object)return control->object;--control->refs;if(!control->refs)delete control;control=0;}return 0;}
void Base::Set(Object* p){if(control){if(control->object==p&&p)return;--control->refs;if(!control->refs&&!control->object)delete control;control=0;}if(p){if(p->control){control=p->control;++control->refs;}else{control=new Control;control->object=p;control->refs=1;p->control=control;}}}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
namespace NRemap006a23e0 {static int calls,expected;static Object objects[3];static Root root;static Scene scene;static Target target;
Root* Manager::GetRoot(){assert(key==7);return &root;}void Target::Apply(Entries& e,int key){assert(this==&target&&key==7&&e.last-e.first==expected);for(int i=0;i<expected;++i){assert(e.first[i].active&&e.first[i].object.Get()==&objects[i]);assert(objects[i].control->refs==2);assert((char*)(e.first+i)-(char*)e.first==i*9);}++calls;}
}
int main(){using namespace NRemap006a23e0;assert(sizeof(Base)==8&&sizeof(Ptr)==8&&sizeof(Entry)==9&&sizeof(Record)==12&&offsetof(Manager,key)==32);{Manager m;memset(&m,0,sizeof(m));m.key=7;root.scene=&scene;scene.target=&target;Record records[4];int i;for(i=0;i<4;++i)records[i].key=i;for(i=0;i<3;++i){objects[i].id=10+i;objects[i].control=0;}records[0].object.Set(&objects[0]);records[1].object.Set(0);records[2].object.Set(&objects[1]);records[3].object.Set(&objects[2]);m.first=m.last=m.end=records;expected=0;m.Remap();assert(calls==1);m.last=m.end=records+4;expected=3;m.Remap();assert(calls==2);m.Remap();assert(calls==3);for(i=0;i<3;++i)assert(objects[i].control->refs==1);for(i=0;i<4;++i)assert(records[i].key==i);
Control* expired=objects[2].control;expired->object=0;objects[2].control=0;expected=2;m.Remap();assert(calls==4&&!records[3].object.control);for(i=0;i<2;++i)assert(objects[i].control->refs==1);}
for(int i=0;i<2;++i){assert(objects[i].control->refs==0);objects[i].control->object=0;delete objects[i].control;objects[i].control=0;}puts("006a23e0_TEST PASS");return 0;}