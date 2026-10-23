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