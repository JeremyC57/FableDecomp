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