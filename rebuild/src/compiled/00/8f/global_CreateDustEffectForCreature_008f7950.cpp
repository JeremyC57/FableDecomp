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