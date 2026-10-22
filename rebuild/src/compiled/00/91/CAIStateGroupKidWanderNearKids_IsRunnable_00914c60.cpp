// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: KidMap::LowerBound bound to both 0x40f020 and 0x4365b0 (distinct instantiations))
// CAIStateGroup_KidWanderNearKids::IsRunnable() -- gate idle modes, then find the first eligible child creature.
// Leaf: IsRunnable
// Retail: Fable.exe (TLC, VC7.1) @ 0x00914C60, 279 bytes (virtual; manifest ?IsRunnable@CAIStateGroup_KidWanderNearKids@@UAE_NXZ).
// Callees: the two component-map lookups call two distinct retail copies of CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   -- 0x0040F020 (first lookup, interface 0xd3) and 0x004365B0 (second lookup, interface 0x31). The copies are separate
//   COMDATs in retail, so they must be separate symbols here; they are modelled as the non-const / const overloads of
//   the same lookup (a modelling device: which retail instance is the const one is not proven).
//   CTCIdleScheduler::IsPreferredIdle 0x00801BE0, CTCCreatureModeManager::HasMode(ulong) 0x0073A8A0 (manifest "CPlayer::HasMode"
//   is propagated), CAIStateGroupBase::GetPWorld 0x008FCFA0 (manifest "CPlayer::GetCurrentMode" is propagated; body
//   PBrain->+0x1c->+0x24, as landed), CWorld::GetPRandomSeed 0x0049D810, GFROR13 0x00497890,
//   CAIStateGroupBase::PeekThingSearchTools 0x008FD000, CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_IsNot,
//   CThingFilter_IsChild>,CThingAICreature> 0x00914AA0 (manifest).
// Layout: engine/ headers for the state group, brain, world and search tools; the generated CThingAICreature header is
//   opaque over the native CThing prefix, so the local CThing view records UsingComponents at +0x20 and the interface
//   vector at +0x44 (as landed 0x00914AA0/0x0092E890). The filter types follow 0x00914AA0; its empty child object has a
//   user-provided empty ctor so value-initialization does not zero an unused byte. The *_Methods structs are the lane's
//   method carriers for generated data-only headers.
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_KidWanderNearKids.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_KidWanderNearKids) == sizeof(CAIStateGroupBase));

struct CTCBase { };
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType,CTCBase*>
    struct SEntry { int type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const int* key);                 // 0x0040F020
    const SEntry* LowerBound(const int* key) const;     // 0x004365B0
};
struct CThing {
    unsigned char pad[0x20]; unsigned int UsingComponents[9]; CInterfaceMap Interfaces;
    template<class T> __forceinline T* GetTC(int id) {
        int key=id; CInterfaceMap* m=&Interfaces; CInterfaceMap::SEntry* p=m->LowerBound(&key);
        if (p==m->last || id<p->type) p=m->last; return (T*)p->component;
    }
    template<class T> __forceinline T* GetTCConst(int id) const {
        int key=id; const CInterfaceMap* m=&Interfaces; const CInterfaceMap::SEntry* p=m->LowerBound(&key);
        if (p==m->last || id<p->type) p=m->last; return (T*)p->component;
    }
};
struct CWorld_Methods : CWorld { unsigned int* GetPRandomSeed(); };          // 0x0049D810
struct CTCIdleScheduler : CTCBase { bool IsPreferredIdle(CAIStateGroupBase* group); };   // 0x00801BE0
struct CTCCreatureModeManager : CTCBase { bool HasMode(unsigned long mode); };          // 0x0073A8A0
struct CAIStateGroupBase_Methods : CAIStateGroupBase {
    CWorld* GetPWorld();                                // 0x008FCFA0
    CThingSearchTools* PeekThingSearchTools();          // 0x008FD000
};
struct CAIStateGroup_KidWanderNearKids_Methods : CAIStateGroup_KidWanderNearKids {
    bool IsRunnable();
};
struct CThingFilter_IsNot {
    const CThing* Excluded;
    CThingFilter_IsNot(const CThing* thing):Excluded(thing){}
    bool operator()(const CThingAICreature* creature) const;
};
struct CThingFilter_IsChild {
    CThingFilter_IsChild() { }
    bool operator()(const CThingAICreature*) const;
};
template<class A, class B> struct CFilter_Bind2 {
    const A& First; const B& Second;
    CFilter_Bind2(const A& a,const B& b):First(a),Second(b){}
};
inline CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild> operator&&(const CThingFilter_IsNot& a,const CThingFilter_IsChild& b)
{ return CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild>(a,b); }
struct CThingSearchTools_Methods : CThingSearchTools {
    template<class F, class T> bool GetPFirstThing(const F& filter, T** out);   // <Bind2<IsNot,IsChild>,CThingAICreature>: 0x00914AA0
};
unsigned int __fastcall GFROR13(unsigned int);          // 0x00497890

bool CAIStateGroup_KidWanderNearKids_Methods::IsRunnable() {
    CAIStateGroupBase_Methods* self = (CAIStateGroupBase_Methods*)(CAIStateGroupBase*)this;
    if (((CThing*)self->PBrain->Creature)->UsingComponents[6] & 0x80000) {
        CTCIdleScheduler* scheduler=((CThing*)(self->PBrain->Creature))->GetTC<CTCIdleScheduler>(0xd3);
        if (!scheduler->IsPreferredIdle(self)) return false;
    }
    if (((CThing*)self->PBrain->Creature)->UsingComponents[1] & 0x20000) {
        CTCCreatureModeManager* modes=((CThing*)(self->PBrain->Creature))->GetTCConst<CTCCreatureModeManager>(0x31);
        if (modes->HasMode(8) || modes->HasMode(9)) {
            unsigned int* seed=((CWorld_Methods*)self->GetPWorld())->GetPRandomSeed();
            *seed=*seed*0x24a1+0x24df;
            *seed=GFROR13(*seed);
            if (*seed % 100 > 10) return false;
        }
    }
    CThingAICreature* found;
    return ((CThingSearchTools_Methods*)self->PeekThingSearchTools())->GetPFirstThing(
        CThingFilter_IsNot((const CThing*)self->PBrain->Creature) && CThingFilter_IsChild(),
        &found);
}