// 008f7bd0 NFoot008f7bd0::CreateFootEffectForCreature
// Flags: /O2 /Oy /W3 /Gr
// Leaf: CreateFootEffectForCreature
#include <new>
#include <stdlib.h>
namespace NFoot008f7bd0 {
struct V {float x,y,z;};extern bool Enabled;struct Thing;struct Record {unsigned hash;V v;};struct Tag{Tag(){}};
#pragma pack(push,1)
struct StringData {char* text;int length,capacity;unsigned char owned;int refs;};
#pragma pack(pop)
void ReleaseData(StringData*);
struct String {StringData* data;String():data(0){}__declspec(noinline) String(const String& x):data(x.data){if(data)++data->refs;}__declspec(noinline) ~String(){if(data&&!--data->refs)ReleaseData(data);}};
extern String LeftFoot,RightFoot;
template<class T> struct Vector {
 T *first,*last,*end;
 __forceinline Vector():first(0),last(0),end(0){}
 __declspec(noinline) ~Vector(){for(T* i=first;i!=last;++i)i->~T();free(first);}
 template<class U> __declspec(noinline) void Overflow(T* pos,const T& value,const U&,int count,int skipTail){unsigned n=first?last-first:0;unsigned cap=n+(n<(unsigned)count?(unsigned)count:n);T* fresh=(T*)malloc(cap*sizeof(T));T* out=fresh;T* i;for(i=first;i!=pos;++i)new(out++)T(*i);for(int k=0;k<count;++k)new(out++)T(value);if(!skipTail)for(i=pos;i!=last;++i)new(out++)T(*i);for(i=first;i!=last;++i)i->~T();free(first);first=fresh;last=out;end=fresh+cap;}
 __forceinline void Push(const T& value){if(last!=end){if(last)new(last)T(value);++last;}else Overflow(last,value,Tag(),1,1);}
};
struct Base {__declspec(noinline) Base();__declspec(noinline) virtual ~Base();};
Base::Base(){} Base::~Base(){}
struct OtherRecord {unsigned hash;Base value;float x,y;};
template<class T> struct Values:Vector<T> {bool order,dirty;__forceinline Values():Vector<T>(),dirty(false){}__forceinline ~Values(){}};
struct Params:Base {Values<Record> values;Values<OtherRecord> other;__forceinline Params():Base(),values(),other(){}__declspec(noinline) virtual ~Params();};
Params::~Params(){}
struct Extra {void Attach(Thing*,int,const String&,int,int);void Set(const Params&);};
struct Effect {char pad[40];unsigned flags;char gap[48];Extra* extra;};
struct Factory {Effect* __fastcall Create(const V&,bool);};struct Thing {const V& Position()const;};
struct Output {Effect **first,**last,**end;template<class U> __declspec(noinline) void Overflow(Effect** pos,Effect*const& value,const U&,int count,int skipTail){unsigned n=first?last-first:0;unsigned cap=n+(n<(unsigned)count?(unsigned)count:n);Effect** fresh=(Effect**)malloc(cap*sizeof(Effect*));Effect** out=fresh;Effect** i;for(i=first;i!=pos;++i)*out++=*i;for(int k=0;k<count;++k)*out++=value;if(!skipTail)for(i=pos;i!=last;++i)*out++=*i;free(first);first=fresh;last=out;end=fresh+cap;}__forceinline void Push(Effect*const& e){if(last!=end){if(last)new(last)Effect*(e);++last;}else Overflow(last,e,Tag(),1,1);}};
__declspec(noinline) bool __fastcall GetBones(Thing*,Vector<String>& names){names.Push(LeftFoot);names.Push(RightFoot);return true;}
unsigned __fastcall Hash(unsigned,const char*,int);
// Layout: GetBones8f78e0 appends two owned Strings via99ec30; vector dtor414ea0 calls99eae0 then free.
// Layout: Effect extra+5c, flags+28 bit800. Attach6e0be0 third arg is String const& (assignment99efb0 at6e0c06).
// Layout: Params36, genuine polymorphic Base4, values16, other16; OtherRecord16 includes Base member+4 per6e2510.
// Layout: after frame4c and four saves, vector arg3 at68/output arg4 at6c; two earlier stack args unused.
// Layout: Create6e0880 uses this ecx, position edx, stack bool. No guessed table writes.
void __fastcall CreateFootEffectForCreature(Thing* thing,Factory* factory,int unused,int unused2,const V& vector,Output* out){if(!Enabled||!factory)return;Vector<String> bones;if(GetBones(thing,bones)){for(String* i=bones.first;i!=bones.last;++i){Effect* e=factory->Create(thing->Position(),false);if(e){if(out)out->Push(e);Extra* extra=0;if(e->flags&0x800)extra=e->extra;extra->Attach(thing,12,*i,0,0);Record rec;rec.hash=Hash(0,"DIRECTION",9);rec.v.x=vector.x;rec.v.y=vector.y;rec.v.z=vector.z;Params params;params.values.Overflow(0,rec,Tag(),1,1);extra->Set(params);}}}}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
namespace NFoot008f7bd0 {
bool Enabled=false;String LeftFoot,RightFoot;static int released,calls,attaches,sets;static bool failFirst;static Thing* expectedThing;static V position={7,11,13};static Extra extras[2];static Effect effects[2];static String retained[2];
void ReleaseData(StringData* p){assert(p->refs==0);free(p->text);free(p);++released;}
static void Init(String& s,const char* name){s.data=(StringData*)malloc(sizeof(StringData));int n=strlen(name);s.data->text=(char*)malloc(n+1);memcpy(s.data->text,name,n+1);s.data->length=s.data->capacity=n;s.data->owned=1;s.data->refs=1;}
Effect* __fastcall Factory::Create(const V& p,bool flag){assert(!flag&&p.x==7&&p.y==11&&p.z==13);int index=calls++%2;return failFirst&&index==0?0:&effects[index];}
const V& Thing::Position()const{assert(this==expectedThing);return position;}
void Extra::Attach(Thing* t,int type,const String& name,int a,int b){assert(t==expectedThing&&type==12&&a==0&&b==0);int index=this==&extras[0]?0:1;assert(name.data==(index==0?LeftFoot.data:RightFoot.data));assert(!retained[index].data);retained[index].data=name.data;++name.data->refs;++attaches;}
void Extra::Set(const Params& p){assert(p.values.last-p.values.first==1);const Record& r=*p.values.first;assert(r.hash==123&&r.v.x==2&&r.v.y==3&&r.v.z==5);assert(!p.other.first&&!p.other.last&&!p.other.end);++sets;}
unsigned __fastcall Hash(unsigned seed,const char* s,int n){assert(seed==0&&n==9&&!strcmp(s,"DIRECTION"));return 123;}
static void ClearRetained(){for(int i=0;i<2;++i)if(retained[i].data){assert(retained[i].data->refs==2);--retained[i].data->refs;retained[i].data=0;}}
}
int main(){using namespace NFoot008f7bd0;assert(sizeof(String)==4&&sizeof(Params)==36&&sizeof(OtherRecord)==16);Init(LeftFoot,"Bip01 L Foot");Init(RightFoot,"Bip01 R Foot");V direction={2,3,5};Thing thing;Factory factory;expectedThing=&thing;Output out;out.first=out.last=out.end=0;for(int i=0;i<2;++i){memset(&effects[i],0,sizeof(Effect));effects[i].flags=0x800;effects[i].extra=&extras[i];}
CreateFootEffectForCreature(0,0,0,0,direction,0);Enabled=true;CreateFootEffectForCreature(&thing,0,0,0,direction,0);assert(!calls);
CreateFootEffectForCreature(&thing,&factory,17,23,direction,&out);assert(calls==2&&attaches==2&&sets==2&&out.last-out.first==2&&out.first[0]==&effects[0]&&out.first[1]==&effects[1]);assert(LeftFoot.data->refs==2&&RightFoot.data->refs==2);ClearRetained();assert(LeftFoot.data->refs==1&&RightFoot.data->refs==1);
failFirst=true;CreateFootEffectForCreature(&thing,&factory,17,23,direction,0);assert(calls==4&&attaches==3&&sets==3&&!retained[0].data&&retained[1].data);ClearRetained();assert(LeftFoot.data->refs==1&&RightFoot.data->refs==1&&!released);free(out.first);StringData* l=LeftFoot.data;StringData* r=RightFoot.data;LeftFoot.data=RightFoot.data=0;assert(!--l->refs);ReleaseData(l);assert(!--r->refs);ReleaseData(r);assert(released==2);puts("008f7bd0_TEST PASS");return 0;}