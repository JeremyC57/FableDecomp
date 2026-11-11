#ifdef FABLE_RETAIL_HEARING
#ifndef FABLE_ENGINE_CTHINGNOISE_H
#define FABLE_ENGINE_CTHINGNOISE_H
#include "engine/CThing.h"
// Layout: complete original Ego3e459/FW43716, no own fields; native152 from original base.
class CThingNoise:public CThing{
public:
 static CClass * FABLE_FASTCALL Alloc();
 static long FABLE_FASTCALL StatGetType(){return 14;}
 static CCharString FABLE_FASTCALL StatGetDefClassName();
 static bool FABLE_FASTCALL StatIsLocal();
 static unsigned long FABLE_FASTCALL StatGetTypeFlags();
 CThingNoise(const CThingNoise & p0);
 CThingNoise();
 virtual bool OnCreate();
 static CThingNoise * FABLE_FASTCALL Create(long p0, const C3DVector & p1, float p2);
 virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0);
 virtual bool WorldUpdate();
 void SetCreator(const CThing & p0);
private:
 bool ConstructFromParams(long p0, const C3DVector & p1);
public:
 virtual ~CThingNoise();
 CThingNoise & operator=(const CThingNoise & p0);
};
FABLE_STATIC_ASSERT(sizeof(CThingNoise)==152);
#endif
#else
#ifndef FABLE_ENGINE_CTHINGNOISE_H
#define FABLE_ENGINE_CTHINGNOISE_H
#include "engine/CThing.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// No own members; native allocation527380.
class CThingNoise:public CThing {
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
// Evidence: both original PDB types and donor026fd4f0; native007c1000 accesses.
public:
static CThingNoise* FABLE_FASTCALL Create(long,const C3DVector&,float);void SetCreator(const CThing&);
#endif

public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingNoise)==0x98);
#endif

#endif
