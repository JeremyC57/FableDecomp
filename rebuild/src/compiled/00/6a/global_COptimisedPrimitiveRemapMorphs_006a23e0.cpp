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