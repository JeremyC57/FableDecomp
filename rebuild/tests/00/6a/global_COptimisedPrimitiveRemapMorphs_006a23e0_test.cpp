// 006a23e0 NRemap006a23e0::CTCCarrying::ResetAllCarriedThingsAsBypassFilter (FableWin method; catalog COptimisedPrimitive_RemapMorphs is wrong)
// Flags: /O2 /Oy /W3
// Leaf: ?ResetAllCarriedThingsAsBypassFilter@CTCCarrying@NRemap006a23e0@@QAEXXZ
#include <stdlib.h>
#include <new>
namespace NRemap006a23e0 {
struct Control;struct Object {int id;Control* control;};struct Control{Object* object;int refs;};
struct CBaseIntelligentPointer {mutable Control* control;__declspec(noinline) CBaseIntelligentPointer();__declspec(noinline) virtual ~CBaseIntelligentPointer();__declspec(noinline) Object* GetPItem()const;__declspec(noinline) void SetPItem(Object*);};
// Layout: A01b10/A01b50/A01b90/A01c10 use weak Control8 at CBaseIntelligentPointer+4; Object owns its control pointer+4. CBaseIntelligentPointer destructor clears control and releases only expired last-reference blocks.

struct Ptr:CBaseIntelligentPointer {__forceinline Ptr(Object* p){SetPItem(p);}__forceinline Ptr(const Ptr& p){SetPItem(p.GetPItem());}};
#pragma pack(push,1)
struct Entry {Ptr object;bool active;Entry(Object* p):object(p),active(true){}__forceinline Entry(const Entry& e):object(e.object),active(e.active){}__forceinline ~Entry(){}};
#pragma pack(pop)
__forceinline void _Destroy(Entry* first,Entry* last){for(;first!=last;++first)first->~Entry();}
struct __false_type{__false_type(){}};struct vector {Entry *first,*last,*end;vector():first(0),last(0),end(0){}__forceinline ~vector(){_Destroy(first,last);if(first)free(first);}template<class T> __declspec(noinline) void _M_insert_overflow(Entry* pos,const Entry& value,const T&,int count,int skipTail){unsigned n=first?last-first:0;unsigned cap=n+(n<(unsigned)count?(unsigned)count:n);Entry* fresh=(Entry*)malloc(cap*sizeof(Entry));Entry* out=fresh;Entry* i;for(i=first;i!=pos;++i)new(out++)Entry(*i);for(int k=0;k<count;++k)new(out++)Entry(value);if(!skipTail)for(i=pos;i!=last;++i)new(out++)Entry(*i);for(i=first;i!=last;++i)i->~Entry();free(first);first=fresh;last=out;end=fresh+cap;}__forceinline void push_back(const Entry& e){if(last!=end){new(last)Entry(e);++last;}else _M_insert_overflow(last,e,__false_type(),1,1);}};
struct Objects {Object **first,**last,**end;Objects():first(0),last(0),end(0){}~Objects(){if(first)free(first);}};
struct CEnvironmentSpellEffectInterfaceMethods {void SetBypassScreenFilter(vector&,int);};struct Scene {char pad[6500];CEnvironmentSpellEffectInterfaceMethods* target;};struct CWorld {char pad[24];Scene* scene;};
// Layout: receiver Key +0x20; GetWorld returns scene pointer +0x18, target +0x1964.
struct Record {int Key;CBaseIntelligentPointer object;};
struct CTCCarrying {char pad[12];Record* first;Record* last;Record* end;char gap[8];int Key;__declspec(noinline) void GetAllCarriedThings(Objects&);CWorld* GetWorld();void ResetAllCarriedThingsAsBypassFilter();};
// Layout: Gather6a2220 reads records+0xc/+0x10 stride12; CBaseIntelligentPointer receiver is record+4. Packed Entry stride9 proven6a3950.
void CTCCarrying::GetAllCarriedThings(Objects& out){for(unsigned i=0;i<(unsigned)(last-first);++i){if(first[i].object.GetPItem()){Object* x=first[i].object.GetPItem();if(out.last==out.end){unsigned n=out.first?out.last-out.first:0;unsigned cap=n?n*2:1;Object** p=(Object**)malloc(cap*sizeof(Object*));for(unsigned k=0;k<n;++k)p[k]=out.first[k];free(out.first);out.first=p;out.last=p+n;out.end=p+cap;}*out.last++=x;}}}
void CTCCarrying::ResetAllCarriedThingsAsBypassFilter(){vector entries;Objects objects;GetAllCarriedThings(objects);for(Object** p=objects.first;p!=objects.last;++p)entries.push_back(Entry(*p));CWorld* root=GetWorld();CEnvironmentSpellEffectInterfaceMethods* target=root->scene->target;target->SetBypassScreenFilter(entries,Key);}
}

namespace NRemap006a23e0 {
CBaseIntelligentPointer::CBaseIntelligentPointer():control(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){if(control){--control->refs;if(!control->refs&&!control->object)delete control;control=0;}}
Object* CBaseIntelligentPointer::GetPItem()const{if(control){if(control->object)return control->object;--control->refs;if(!control->refs)delete control;control=0;}return 0;}
void CBaseIntelligentPointer::SetPItem(Object* p){if(control){if(control->object==p&&p)return;--control->refs;if(!control->refs&&!control->object)delete control;control=0;}if(p){if(p->control){control=p->control;++control->refs;}else{control=new Control;control->object=p;control->refs=1;p->control=control;}}}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
namespace NRemap006a23e0 {static int calls,expected;static Object objects[3];static CWorld root;static Scene scene;static CEnvironmentSpellEffectInterfaceMethods target;
CWorld* CTCCarrying::GetWorld(){assert(Key==7);return &root;}void CEnvironmentSpellEffectInterfaceMethods::SetBypassScreenFilter(vector& e,int Key){assert(this==&target&&Key==7&&e.last-e.first==expected);for(int i=0;i<expected;++i){assert(e.first[i].active&&e.first[i].object.GetPItem()==&objects[i]);assert(objects[i].control->refs==2);assert((char*)(e.first+i)-(char*)e.first==i*9);}++calls;}
}
int main(){using namespace NRemap006a23e0;assert(sizeof(CBaseIntelligentPointer)==8&&sizeof(Ptr)==8&&sizeof(Entry)==9&&sizeof(Record)==12&&offsetof(CTCCarrying,Key)==32);{CTCCarrying m;memset(&m,0,sizeof(m));m.Key=7;root.scene=&scene;scene.target=&target;Record records[4];int i;for(i=0;i<4;++i)records[i].Key=i;for(i=0;i<3;++i){objects[i].id=10+i;objects[i].control=0;}records[0].object.SetPItem(&objects[0]);records[1].object.SetPItem(0);records[2].object.SetPItem(&objects[1]);records[3].object.SetPItem(&objects[2]);m.first=m.last=m.end=records;expected=0;m.ResetAllCarriedThingsAsBypassFilter();assert(calls==1);m.last=m.end=records+4;expected=3;m.ResetAllCarriedThingsAsBypassFilter();assert(calls==2);m.ResetAllCarriedThingsAsBypassFilter();assert(calls==3);for(i=0;i<3;++i)assert(objects[i].control->refs==1);for(i=0;i<4;++i)assert(records[i].Key==i);
Control* expired=objects[2].control;expired->object=0;objects[2].control=0;expected=2;m.ResetAllCarriedThingsAsBypassFilter();assert(calls==4&&!records[3].object.control);for(i=0;i<2;++i)assert(objects[i].control->refs==1);}
for(int i=0;i<2;++i){assert(objects[i].control->refs==0);objects[i].control->object=0;delete objects[i].control;objects[i].control=0;}puts("006a23e0_TEST PASS");return 0;}