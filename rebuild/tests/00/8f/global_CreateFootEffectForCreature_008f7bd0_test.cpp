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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
namespace NFoot008f7bd0 {
bool Enabled=false;CCharString LeftFoot,RightFoot;static int released,calls,attaches,sets;static bool failFirst;static CThingCreatureBase* expectedThing;static C3DVector position={7,11,13};static CTCDParticleEmitter extras[2];static CThing effects[2];static CCharString retained[2];
void ReleaseData(CCharStringData* p){assert(p->refs==0);free(p->text);free(p);++released;}
static void Init(CCharString& s,const char* name){s.data=(CCharStringData*)malloc(sizeof(CCharStringData));int n=strlen(name);s.data->text=(char*)malloc(n+1);memcpy(s.data->text,name,n+1);s.data->length=s.data->capacity=n;s.data->owned=1;s.data->refs=1;}
CThing* __fastcall CTCDParticleEmitter::Create(long effect,const C3DVector& p,bool flag){assert(!flag&&p.x==7&&p.y==11&&p.z==13);int index=calls++%2;return failFirst&&index==0?0:&effects[index];}
const C3DVector& CThingCreatureBase::GetPosition()const{assert(this==expectedThing);return position;}
void CTCDParticleEmitter::AttachToThing(CThingCreatureBase* t,int type,const CCharString& name,int a,int b){assert(t==expectedThing&&type==12&&a==0&&b==0);int index=this==&extras[0]?0:1;assert(name.data==(index==0?LeftFoot.data:RightFoot.data));assert(!retained[index].data);retained[index].data=name.data;++name.data->refs;++attaches;}
void CTCDParticleEmitter::SetEmitterUpdateParameters(const CParticleUpdateParameters& p){assert(p.values.last-p.values.first==1);const SVectorParam& r=*p.values.first;assert(r.hash==123&&r.v.x==2&&r.v.y==3&&r.v.z==5);assert(!p.other.first&&!p.other.last&&!p.other.end);++sets;}
unsigned __fastcall CCRC::Calc(unsigned seed,const char* s,int n){assert(seed==0&&n==9&&!strcmp(s,"DIRECTION"));return 123;}
static void ClearRetained(){for(int i=0;i<2;++i)if(retained[i].data){assert(retained[i].data->refs==2);--retained[i].data->refs;retained[i].data=0;}}
}
int main(){using namespace NFoot008f7bd0;assert(sizeof(CCharString)==4&&sizeof(CParticleUpdateParameters)==36&&sizeof(OtherRecord)==16);Init(LeftFoot,"Bip01 L Foot");Init(RightFoot,"Bip01 R Foot");C3DVector direction={2,3,5};CThingCreatureBase thing;int factory;expectedThing=&thing;CThingVector out;out.first=out.last=out.end=0;for(int i=0;i<2;++i){memset(&effects[i],0,sizeof(CThing));effects[i].flags=0x800;effects[i].extra=&extras[i];}
CreateFootEffectForCreature(0,0,0,0,direction,0);Enabled=true;CreateFootEffectForCreature(&thing,0,0,0,direction,0);assert(!calls);
CreateFootEffectForCreature(&thing,(long)&factory,17,23,direction,&out);assert(calls==2&&attaches==2&&sets==2&&out.last-out.first==2&&out.first[0]==&effects[0]&&out.first[1]==&effects[1]);assert(LeftFoot.data->refs==2&&RightFoot.data->refs==2);ClearRetained();assert(LeftFoot.data->refs==1&&RightFoot.data->refs==1);
failFirst=true;CreateFootEffectForCreature(&thing,(long)&factory,17,23,direction,0);assert(calls==4&&attaches==3&&sets==3&&!retained[0].data&&retained[1].data);ClearRetained();assert(LeftFoot.data->refs==1&&RightFoot.data->refs==1&&!released);free(out.first);CCharStringData* l=LeftFoot.data;CCharStringData* r=RightFoot.data;LeftFoot.data=RightFoot.data=0;assert(!--l->refs);ReleaseData(l);assert(!--r->refs);ReleaseData(r);assert(released==2);puts("008f7bd0_TEST PASS");return 0;}