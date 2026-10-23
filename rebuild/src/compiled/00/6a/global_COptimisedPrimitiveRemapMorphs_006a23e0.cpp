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