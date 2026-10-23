// 008f7950 CreateDustEffectForCreature (catalog; bsim NCombatActionHelpers::CreateDustEffectForCreature(CThingCreatureBase&,const CWorldMap&,const CGameDefinitionManager&,const C3DVector&,vector<CThing*>*); model kept in NDust008f7950)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: CreateDustEffectForCreature
#include <stdlib.h>
namespace NDust008f7950 {
struct C3DVector {float x,y,z;__forceinline C3DVector& operator=(const C3DVector&v){x=v.x;y=v.y;z=v.z;return *this;}};extern bool Enabled;struct CThingCreatureBase;struct SVectorParam {unsigned hash;C3DVector v;};
struct CVectorParamVector {SVectorParam* first;SVectorParam* last;SVectorParam* end;bool order,dirty;char pad[2];void push_back(const SVectorParam&);};
// Layout: 6e24d0 installs vptr and initializes two16-byte Values at+4/+20; dirty bytes+17/+33.
struct Base {Base();virtual ~Base();};
struct OtherRecord {unsigned hash;Base value;float x,y;};
struct OtherRecords {OtherRecord*first,*last,*end;bool order,dirty;char pad[2];};
struct CParticleUpdateParameters:Base {CVectorParamVector records;OtherRecords other;CParticleUpdateParameters();virtual ~CParticleUpdateParameters();};
struct CThing;struct CTCDParticleEmitter {void SetEmitterUpdateParameters(const CParticleUpdateParameters&);static CThing* __fastcall Create(long,const C3DVector&,bool);};struct CThing {char pad[40];unsigned mask;char gap[48];CTCDParticleEmitter* extra;__declspec(noinline) bool GetTCDriver(CTCDParticleEmitter*& out){if(mask&0x800){out=extra;return true;}return false;}};
struct Resource {virtual void v0()=0;virtual void Delete()=0;int refs;};struct CMaterialDef:Resource {long Get();};
struct CEngineThemeDef:Resource {char pad[132];int effect;};
struct CThingCreatureBase {const C3DVector& GetPosition()const;};struct CWorldMap {int GetStrongestEngineThemeAt(const C3DVector&,int&);};struct CGameDefinitionManager {void Lookup(Resource*&,int);__declspec(noinline) bool GetDef(int,CEngineThemeDef*&);__declspec(noinline) bool GetDef(int,CMaterialDef*&);};struct CThingVector {CThing** first;CThing** last;CThing** end;__declspec(noinline) void push_back(CThing*const& value){if(last==end){unsigned n=first?last-first:0;unsigned cap=n?n*2:1;CThing** fresh=(CThing**)malloc(cap*sizeof(CThing*));for(unsigned i=0;i<n;++i)fresh[i]=first[i];free(first);first=fresh;last=fresh+n;end=fresh+cap;}if(last)*last=value;++last;}};
__declspec(noinline) bool CGameDefinitionManager::GetDef(int id,CEngineThemeDef*& out){if(id<=0)return false;Resource* resource=0;Lookup(resource,id);if(!resource)return false;CEngineThemeDef* value=(CEngineThemeDef*)resource;if(out!=value){if(out&&!--out->refs)out->Delete();out=value;++value->refs;}if(!--value->refs)value->Delete();return true;}
__declspec(noinline) bool CGameDefinitionManager::GetDef(int id,CMaterialDef*& out){if(id<=0)return false;Resource* resource=0;Lookup(resource,id);if(!resource)return false;CMaterialDef* value=(CMaterialDef*)resource;if(out!=value){if(out&&!--out->refs)out->Delete();out=value;++value->refs;}if(!--value->refs)value->Delete();return true;}

struct CCRC{static unsigned __fastcall Calc(unsigned,const char*,int);};
// Layout: CParticleUpdateParameters record vector+4 (6e24d0); Create6e0880 consumes this ecx, position edx, stack bool.
// Layout: surface effect id+0x8c, intrusive refcount+4; output optional.
void __fastcall CreateDustEffectForCreature(CThingCreatureBase* thing,CWorldMap* map,CGameDefinitionManager* defs,const C3DVector& vector,CThingVector* out){if(!Enabled)return;int unused=0;int sample=map->GetStrongestEngineThemeAt(thing->GetPosition(),unused);CEngineThemeDef* surface=0;if(defs->GetDef(sample,surface)){if(surface->effect){CMaterialDef* def=0;defs->GetDef(surface->effect,def);long f=def->Get();if(f){CThing* effect=CTCDParticleEmitter::Create(f,thing->GetPosition(),0);if(effect){if(out)out->push_back(effect);CTCDParticleEmitter* extra=0;effect->GetTCDriver(extra);SVectorParam rec;rec.hash=CCRC::Calc(0,"DIRECTION",9);rec.v=vector;CParticleUpdateParameters params;params.records.push_back(rec);extra->SetEmitterUpdateParameters(params);}}if(def&&!--def->refs)def->Delete();}}if(surface&&!--surface->refs)surface->Delete();}
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
namespace NDust008f7950 {
bool Enabled=false;static int mode=0,calls=0,sets=0,pushes=0,alive=0,deletes=0;static C3DVector pos={7,11,13},direction={2,3,5};static int factory;static CThing effect;static CTCDParticleEmitter extra;
struct S:CEngineThemeDef{void v0(){}void Delete(){++deletes;}};struct D:CMaterialDef{void v0(){}void Delete(){++deletes;}};static S surface;static D definition;
Base::Base(){}Base::~Base(){}
CParticleUpdateParameters::CParticleUpdateParameters(){records.first=records.last=records.end=0;records.dirty=false;memset(records.pad,0,2);other.first=other.last=other.end=0;other.dirty=false;++alive;}CParticleUpdateParameters::~CParticleUpdateParameters(){for(OtherRecord*p=other.first;p!=other.last;++p)p->~OtherRecord();free(other.first);free(records.first);--alive;}
void CVectorParamVector::push_back(const SVectorParam& r){assert(!first);first=(SVectorParam*)malloc(sizeof(SVectorParam));*first=r;last=end=first+1;}
void CTCDParticleEmitter::SetEmitterUpdateParameters(const CParticleUpdateParameters& p){assert(this==&extra&&alive==1);assert(p.records.last-p.records.first==1);const SVectorParam& r=*p.records.first;assert(r.hash==123&&r.v.x==2&&r.v.y==3&&r.v.z==5);++sets;}

CThing* __fastcall CTCDParticleEmitter::Create(long index,const C3DVector& v,bool flag){assert(index==(long)&factory&&!flag&&v.x==7&&v.y==11&&v.z==13);++calls;return mode==4?0:&effect;}
long CMaterialDef::Get(){assert(this==&definition);return mode==3?0:(long)&factory;}
const C3DVector& CThingCreatureBase::GetPosition()const{return pos;}int CWorldMap::GetStrongestEngineThemeAt(const C3DVector& v,int& i){assert(v.x==7&&i==0);i=7;return 19;}
void CGameDefinitionManager::Lookup(Resource*& p,int id){assert(p==0);if(id==19){if(mode==0)return;surface.refs=1;surface.effect=mode==1?0:77;p=&surface;}else{assert(id==77);definition.refs=1;p=&definition;}}unsigned __fastcall CCRC::Calc(unsigned seed,const char* s,int n){assert(seed==0&&n==9&&strcmp(s,"DIRECTION")==0);return 123;}
}
int main(){using namespace NDust008f7950;assert(sizeof(CParticleUpdateParameters)==36&&offsetof(CParticleUpdateParameters,records)==4);CreateDustEffectForCreature(0,0,0,direction,0);Enabled=true;effect.mask=0x800;effect.extra=&extra;CThingCreatureBase t;CWorldMap m;CGameDefinitionManager d;CThingVector out;out.first=out.last=out.end=0;
for(mode=0;mode<6;++mode){int prior=deletes;CreateDustEffectForCreature(&t,&m,&d,direction,mode==5?&out:0);assert(alive==0);assert(deletes-prior==(mode==0?0:mode==1?1:2));}assert(calls==3&&sets==2&&out.last-out.first==1&&out.first[0]==&effect);free(out.first);puts("008f7950_TEST PASS");return 0;}