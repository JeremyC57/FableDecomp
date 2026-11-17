// 008f7950 CreateDustEffectForCreature (catalog; bsim NCombatActionHelpers::CreateDustEffectForCreature(CThingCreatureBase&,const CWorldMap&,const CGameDefinitionManager&,const C3DVector&,vector<CThing*>*); model kept in NDust008f7950)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: CreateDustEffectForCreature
#include "fable_compiler.h"
#include <stdlib.h>
// NF49 (round 7): the def classes, the def handle and the manager come from the shared headers, at global scope (were Resource /
// CMaterialDef / CEngineThemeDef / CGameDefinitionManager models inside NDust008f7950 with two same-TU noinline GetDef stand-in bodies
// over a made-up Lookup callee): the lookups are the member template instances
// ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z (0x0063F3A0) and
// ??$GetDef@VCMaterialDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCMaterialDef@@@@@Z (0x0066C340) (FableWin.pdb), with the
// header's same-TU body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, the stand-ins' role; landed 0x007F0300 shape); the raw
// def pointers released by hand are CDefPointer<const T> handles released by their scope-end destructors. The material call
// 0x008EE5A0 is ?GetRandomSlideFootParticleEffect@CMaterialDef@@QBEJXZ (Ego_r.pdb; overlay opt-in
// FABLE_RETAIL_CMATERIALDEF_GETRANDOMSLIDEFOOTPARTICLEEFFECT), was a made-up CMaterialDef::Get. CEngineThemeDef has no shared header
// (engine/_quarantine only): modelled on the shared CDefClassBase with MaterialDef +0x8c as landed 0x007F0300. The rest of the TU
// (the function's own namespace and parameter models) is unchanged.
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMATERIALDEF_GETRANDOMSLIDEFOOTPARTICLEEFFECT
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMaterialDef.h"
class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x8c - 0x25];
    long          MaterialDef;                                  // +0x8c
};
namespace NDust008f7950 {
struct C3DVector {float x,y,z;FABLE_FORCEINLINE C3DVector& operator=(const C3DVector&v){x=v.x;y=v.y;z=v.z;return *this;}};extern bool Enabled;struct CThingCreatureBase;struct SVectorParam {unsigned hash;C3DVector v;};
struct CVectorParamVector {SVectorParam* first;SVectorParam* last;SVectorParam* end;bool order,dirty;char pad[2];void push_back(const SVectorParam&);};
// Layout: 6e24d0 installs vptr and initializes two16-byte Values at+4/+20; dirty bytes+17/+33.
struct Base {Base();virtual ~Base();};
struct OtherRecord {unsigned hash;Base value;float x,y;};
struct OtherRecords {OtherRecord*first,*last,*end;bool order,dirty;char pad[2];};
struct CParticleUpdateParameters:Base {CVectorParamVector records;OtherRecords other;CParticleUpdateParameters();virtual ~CParticleUpdateParameters();};
struct CThing;struct CTCDParticleEmitter {void SetEmitterUpdateParameters(const CParticleUpdateParameters&);static CThing* __fastcall Create(long,const C3DVector&,bool);};struct CThing {char pad[40];unsigned mask;char gap[48];CTCDParticleEmitter* extra;__declspec(noinline) bool GetTCDriver(CTCDParticleEmitter*& out){if(mask&0x800){out=extra;return true;}return false;}};
struct CThingCreatureBase {const C3DVector& GetPosition()const;};struct CWorldMap {int GetStrongestEngineThemeAt(const C3DVector&,int&);};struct CThingVector {CThing** first;CThing** last;CThing** end;__declspec(noinline) void push_back(CThing*const& value){if(last==end){unsigned n=first?last-first:0;unsigned cap=n?n*2:1;CThing** fresh=(CThing**)malloc(cap*sizeof(CThing*));for(unsigned i=0;i<n;++i)fresh[i]=first[i];free(first);first=fresh;last=fresh+n;end=fresh+cap;}if(last)*last=value;++last;}};

struct CCRC{static unsigned __fastcall Calc(unsigned,const char*,int);};
// Layout: CParticleUpdateParameters record vector+4 (6e24d0); Create6e0880 consumes this ecx, position edx, stack bool.
// Layout: surface material def id +0x8c (CEngineThemeDef::MaterialDef); output optional.
void __fastcall CreateDustEffectForCreature(CThingCreatureBase* thing,CWorldMap* map,CGameDefinitionManager* defs,const C3DVector& vector,CThingVector* out){if(!Enabled)return;int unused=0;int sample=map->GetStrongestEngineThemeAt(thing->GetPosition(),unused);CDefPointer<const CEngineThemeDef> surface;if(defs->GetDef(sample,&surface)){if(surface->MaterialDef){CDefPointer<const CMaterialDef> def;defs->GetDef(surface->MaterialDef,&def);long f=def->GetRandomSlideFootParticleEffect();if(f){CThing* effect=CTCDParticleEmitter::Create(f,thing->GetPosition(),0);if(effect){if(out)out->push_back(effect);CTCDParticleEmitter* extra=0;effect->GetTCDriver(extra);SVectorParam rec;rec.hash=CCRC::Calc(0,"DIRECTION",9);rec.v=vector;CParticleUpdateParameters params;params.records.push_back(rec);extra->SetEmitterUpdateParameters(params);}}}}}
}