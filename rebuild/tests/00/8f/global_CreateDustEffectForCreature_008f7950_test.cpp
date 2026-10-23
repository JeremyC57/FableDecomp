// 008f7950 NDust008f7950::CreateDustEffectForCreature
// Flags: /O2 /Oy /W3 /Gr
// Leaf: CreateDustEffectForCreature
#include <stdlib.h>
namespace NDust008f7950 {
struct V {float x,y,z;__forceinline V& operator=(const V&v){x=v.x;y=v.y;z=v.z;return *this;}};extern bool Enabled;struct Thing;struct Record {unsigned hash;V v;};
struct Records {Record* first;Record* last;Record* end;bool order,dirty;char pad[2];void Add(const Record&);};
// Layout: 6e24d0 installs vptr and initializes two16-byte Values at+4/+20; dirty bytes+17/+33.
struct Base {Base();virtual ~Base();};
struct OtherRecord {unsigned hash;Base value;float x,y;};
struct OtherRecords {OtherRecord*first,*last,*end;bool order,dirty;char pad[2];};
struct Params:Base {Records records;OtherRecords other;Params();virtual ~Params();};
struct Extra {void Set(const Params&);};struct Effect {char pad[40];unsigned mask;char gap[48];Extra* extra;__declspec(noinline) bool GetExtra(Extra*& out){if(mask&0x800){out=extra;return true;}return false;}};
struct Factory {Effect* __fastcall Create(const V&,bool);};struct Resource {virtual void v0()=0;virtual void Delete()=0;int refs;};struct EffectDef:Resource {Factory* Get();};
struct Surface:Resource {char pad[132];int effect;};
struct Thing {const V& Position()const;};struct Map {int Sample(const V&,int&);};struct Definitions {void Lookup(Resource*&,int);__declspec(noinline) bool SurfaceFor(int,Surface*&);__declspec(noinline) bool EffectFor(int,EffectDef*&);};struct Output {Effect** first;Effect** last;Effect** end;__declspec(noinline) void Push(Effect*const& value){if(last==end){unsigned n=first?last-first:0;unsigned cap=n?n*2:1;Effect** fresh=(Effect**)malloc(cap*sizeof(Effect*));for(unsigned i=0;i<n;++i)fresh[i]=first[i];free(first);first=fresh;last=fresh+n;end=fresh+cap;}if(last)*last=value;++last;}};
__declspec(noinline) bool Definitions::SurfaceFor(int id,Surface*& out){if(id<=0)return false;Resource* resource=0;Lookup(resource,id);if(!resource)return false;Surface* value=(Surface*)resource;if(out!=value){if(out&&!--out->refs)out->Delete();out=value;++value->refs;}if(!--value->refs)value->Delete();return true;}
__declspec(noinline) bool Definitions::EffectFor(int id,EffectDef*& out){if(id<=0)return false;Resource* resource=0;Lookup(resource,id);if(!resource)return false;EffectDef* value=(EffectDef*)resource;if(out!=value){if(out&&!--out->refs)out->Delete();out=value;++value->refs;}if(!--value->refs)value->Delete();return true;}

unsigned __fastcall Hash(unsigned,const char*,int);
// Layout: Params record vector+4 (6e24d0); Create6e0880 consumes this ecx, position edx, stack bool.
// Layout: surface effect id+0x8c, intrusive refcount+4; output optional.
void __fastcall CreateDustEffectForCreature(Thing* thing,Map* map,Definitions* defs,const V& vector,Output* out){if(!Enabled)return;int unused=0;int sample=map->Sample(thing->Position(),unused);Surface* surface=0;if(defs->SurfaceFor(sample,surface)){if(surface->effect){EffectDef* def=0;defs->EffectFor(surface->effect,def);Factory* f=def->Get();if(f){Effect* effect=f->Create(thing->Position(),0);if(effect){if(out)out->Push(effect);Extra* extra=0;effect->GetExtra(extra);Record rec;rec.hash=Hash(0,"DIRECTION",9);rec.v=vector;Params params;params.records.Add(rec);extra->Set(params);}}if(def&&!--def->refs)def->Delete();}}if(surface&&!--surface->refs)surface->Delete();}
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
namespace NDust008f7950 {
bool Enabled=false;static int mode=0,calls=0,sets=0,pushes=0,alive=0,deletes=0;static V pos={7,11,13},direction={2,3,5};static Factory factory;static Effect effect;static Extra extra;
struct S:Surface{void v0(){}void Delete(){++deletes;}};struct D:EffectDef{void v0(){}void Delete(){++deletes;}};static S surface;static D definition;
Base::Base(){}Base::~Base(){}
Params::Params(){records.first=records.last=records.end=0;records.dirty=false;memset(records.pad,0,2);other.first=other.last=other.end=0;other.dirty=false;++alive;}Params::~Params(){for(OtherRecord*p=other.first;p!=other.last;++p)p->~OtherRecord();free(other.first);free(records.first);--alive;}
void Records::Add(const Record& r){assert(!first);first=(Record*)malloc(sizeof(Record));*first=r;last=end=first+1;}
void Extra::Set(const Params& p){assert(this==&extra&&alive==1);assert(p.records.last-p.records.first==1);const Record& r=*p.records.first;assert(r.hash==123&&r.v.x==2&&r.v.y==3&&r.v.z==5);++sets;}

Effect* __fastcall Factory::Create(const V& v,bool flag){assert(this==&factory&&!flag&&v.x==7&&v.y==11&&v.z==13);++calls;return mode==4?0:&effect;}
Factory* EffectDef::Get(){assert(this==&definition);return mode==3?0:&factory;}
const V& Thing::Position()const{return pos;}int Map::Sample(const V& v,int& i){assert(v.x==7&&i==0);i=7;return 19;}
void Definitions::Lookup(Resource*& p,int id){assert(p==0);if(id==19){if(mode==0)return;surface.refs=1;surface.effect=mode==1?0:77;p=&surface;}else{assert(id==77);definition.refs=1;p=&definition;}}unsigned __fastcall Hash(unsigned seed,const char* s,int n){assert(seed==0&&n==9&&strcmp(s,"DIRECTION")==0);return 123;}
}
int main(){using namespace NDust008f7950;assert(sizeof(Params)==36&&offsetof(Params,records)==4);CreateDustEffectForCreature(0,0,0,direction,0);Enabled=true;effect.mask=0x800;effect.extra=&extra;Thing t;Map m;Definitions d;Output out;out.first=out.last=out.end=0;
for(mode=0;mode<6;++mode){int prior=deletes;CreateDustEffectForCreature(&t,&m,&d,direction,mode==5?&out:0);assert(alive==0);assert(deletes-prior==(mode==0?0:mode==1?1:2));}assert(calls==3&&sets==2&&out.last-out.first==1&&out.first[0]==&effect);free(out.first);puts("008f7950_TEST PASS");return 0;}