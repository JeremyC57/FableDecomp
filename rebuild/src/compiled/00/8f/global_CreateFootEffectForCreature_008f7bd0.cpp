// 008f7bd0 CreateFootEffectForCreature (catalog; bsim NCombatActionHelpers::CreateFootEffectForCreature(CThingCreatureBase&,long,const CWorldMap&,const CGameDefinitionManager&,const C3DVector&,vector<CThing*>*); model kept in NFoot008f7bd0)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: CreateFootEffectForCreature
#include <new>
#include <stdlib.h>
namespace NFoot008f7bd0 {
struct C3DVector {float x,y,z;};extern bool Enabled;struct CThingCreatureBase;struct SVectorParam {unsigned hash;C3DVector v;};struct Tag{Tag(){}};
#pragma pack(push,1)
struct CCharStringData {char* text;int length,capacity;unsigned char owned;int refs;};
#pragma pack(pop)
void ReleaseData(CCharStringData*);
struct CCharString {CCharStringData* data;CCharString():data(0){}__declspec(noinline) CCharString(const CCharString& x):data(x.data){if(data)++data->refs;}__declspec(noinline) ~CCharString(){if(data&&!--data->refs)ReleaseData(data);}};
extern CCharString LeftFoot,RightFoot;
template<class T> struct vector {
 T *first,*last,*end;
 __forceinline vector():first(0),last(0),end(0){}
 __declspec(noinline) ~vector(){for(T* i=first;i!=last;++i)i->~T();free(first);}
 template<class U> __declspec(noinline) void _M_insert_overflow(T* pos,const T& value,const U&,int count,int skipTail){unsigned n=first?last-first:0;unsigned cap=n+(n<(unsigned)count?(unsigned)count:n);T* fresh=(T*)malloc(cap*sizeof(T));T* out=fresh;T* i;for(i=first;i!=pos;++i)new(out++)T(*i);for(int k=0;k<count;++k)new(out++)T(value);if(!skipTail)for(i=pos;i!=last;++i)new(out++)T(*i);for(i=first;i!=last;++i)i->~T();free(first);first=fresh;last=out;end=fresh+cap;}
 __forceinline void push_back(const T& value){if(last!=end){if(last)new(last)T(value);++last;}else _M_insert_overflow(last,value,Tag(),1,1);}
};
struct CBaseClass {__declspec(noinline) CBaseClass();__declspec(noinline) virtual ~CBaseClass();};
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
struct OtherRecord {unsigned hash;CBaseClass value;float x,y;};
template<class T> struct Values:vector<T> {bool order,dirty;__forceinline Values():vector<T>(),dirty(false){}__forceinline ~Values(){}};
struct CParticleUpdateParameters:CBaseClass {Values<SVectorParam> values;Values<OtherRecord> other;__forceinline CParticleUpdateParameters():CBaseClass(),values(),other(){}__declspec(noinline) virtual ~CParticleUpdateParameters();};
CParticleUpdateParameters::~CParticleUpdateParameters(){}
struct CThing;struct CTCDParticleEmitter {void AttachToThing(CThingCreatureBase*,int,const CCharString&,int,int);void SetEmitterUpdateParameters(const CParticleUpdateParameters&);static CThing* __fastcall Create(long,const C3DVector&,bool);};
struct CThing {char pad[40];unsigned flags;char gap[48];CTCDParticleEmitter* extra;};
struct CThingCreatureBase {const C3DVector& GetPosition()const;};
struct CThingVector {CThing **first,**last,**end;template<class U> __declspec(noinline) void _M_insert_overflow(CThing** pos,CThing*const& value,const U&,int count,int skipTail){unsigned n=first?last-first:0;unsigned cap=n+(n<(unsigned)count?(unsigned)count:n);CThing** fresh=(CThing**)malloc(cap*sizeof(CThing*));CThing** out=fresh;CThing** i;for(i=first;i!=pos;++i)*out++=*i;for(int k=0;k<count;++k)*out++=value;if(!skipTail)for(i=pos;i!=last;++i)*out++=*i;free(first);first=fresh;last=out;end=fresh+cap;}__forceinline void push_back(CThing*const& e){if(last!=end){if(last)new(last)CThing*(e);++last;}else _M_insert_overflow(last,e,Tag(),1,1);}};
__declspec(noinline) bool __fastcall GetBones(CThingCreatureBase*,vector<CCharString>& names){names.push_back(LeftFoot);names.push_back(RightFoot);return true;}
struct CCRC{static unsigned __fastcall Calc(unsigned,const char*,int);};
// Layout: GetBones8f78e0 appends two owned Strings via99ec30; vector dtor414ea0 calls99eae0 then free.
// Layout: CThing extra+5c, flags+28 bit800. Attach6e0be0 third arg is CCharString const& (assignment99efb0 at6e0c06).
// Layout: Params36, genuine polymorphic Base4, values16, other16; OtherRecord16 includes CBaseClass member+4 per6e2510.
// Layout: after frame4c and four saves, vector arg3 at68/output arg4 at6c; two earlier stack args unused.
// Layout: Create6e0880 uses this ecx, position edx, stack bool. No guessed table writes.
void __fastcall CreateFootEffectForCreature(CThingCreatureBase* thing,long effect,int unused,int unused2,const C3DVector& direction,CThingVector* out){if(!Enabled||!effect)return;vector<CCharString> bones;if(GetBones(thing,bones)){for(CCharString* i=bones.first;i!=bones.last;++i){CThing* e=CTCDParticleEmitter::Create(effect,thing->GetPosition(),false);if(e){if(out)out->push_back(e);CTCDParticleEmitter* extra=0;if(e->flags&0x800)extra=e->extra;extra->AttachToThing(thing,12,*i,0,0);SVectorParam rec;rec.hash=CCRC::Calc(0,"DIRECTION",9);rec.v.x=direction.x;rec.v.y=direction.y;rec.v.z=direction.z;CParticleUpdateParameters params;params.values._M_insert_overflow(0,rec,Tag(),1,1);extra->SetEmitterUpdateParameters(params);}}}}
}