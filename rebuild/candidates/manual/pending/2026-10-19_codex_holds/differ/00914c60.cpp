// Grade: HELD(bytes match retail) -- KidMap::LowerBound bound to both 0x40f020 and 0x4365b0 (distinct instantiations) | Codex source 00/91/CAIStateGroupKidWanderNearKids_IsRunnable_00914c60.cpp, flags , pass pattern 00914c60_TEST PASS
// CAIStateGroup_KidWanderNearKids::IsRunnable() -- gate idle modes, then find the first eligible child creature.
// Retail @ 0x00914c60, 279 bytes. The first and second component-map lookups call byte-identical
// LowerBound bodies at 0x0040f020 and 0x004365b0 respectively. Other callees: CTCIdleScheduler::IsPreferredIdle
// 0x00801be0, CTCCreatureModeManager::IsModeActive 0x0073a8a0 (the propagated CPlayer label is wrong),
// CAIStateGroupBase::GetPThingWorld 0x008fcfa0, CWorld::GetPRandomSeed 0x0049d810, GFROR13 0x00497890,
// PeekThingSearchTools 0x008fd000, CThingSearchTools::GetPFirstThing 0x00914aa0. The generated
// CThingAICreature header is opaque over the native CThing prefix; the local CThing view follows landed
// 0x00914aa0/0x0092e890 and the retail flag/map offsets. The filter types follow 0x00914aa0; its empty
// child object has a user-provided empty ctor so value-initialization does not zero an unused byte.
#ifdef _MSC_VER
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_KidWanderNearKids.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_KidWanderNearKids) == sizeof(CAIStateGroupBase));
#else
#define __forceinline inline
#define __fastcall
struct CThingAICreature;
struct CWorld { };
struct CAIBrain { char pad[0x18]; CThingAICreature* Creature; };
struct CAIStateGroupBase { void* vtable; CAIBrain* PBrain; char rest[0xc]; };
struct CAIStateGroup_KidWanderNearKids { void* vtable; char base[0x10]; };
struct CThingSearchTools { };
#endif

struct CTCBase { };
struct KidEntry { int type; CTCBase* component; };
struct KidMap { KidEntry* first; KidEntry* last; KidEntry* LowerBound(const int* key); }; // 0x0040f020 / 0x004365b0
// CThing is quarantined and CThingAICreature's PDB header has only opaque base bytes here. This
// exact retail prefix view records UsingComponents at +0x20 and the interface vector at +0x44.
struct CThing {
    unsigned char pad[0x20]; unsigned int bits[9]; KidMap map;
    bool HasTC(int id) const { return (bits[id >> 5] & (1U << (id & 31))) != 0; }
    template<class T> __forceinline T* GetTC(int id) {
        int key=id; KidMap* m=&map; KidEntry* p=m->LowerBound(&key);
        if (p==m->last || id<p->type) p=m->last; return (T*)p->component;
    }
};
struct CWorldMethods : CWorld { unsigned int* GetPRandomSeed(); }; // 0x0049d810, actual CWorld method adapter
struct KidScheduler:CTCBase { bool IsPreferredIdle(CAIStateGroupBase* group); }; // 0x00801be0
struct CTCCreatureModeManager:CTCBase { bool IsModeActive(unsigned int mode); }; // 0x0073a8a0
struct KidWanderMethods : CAIStateGroup_KidWanderNearKids {
    CWorld* GetPThingWorld(); // 0x008fcfa0
    CThingSearchTools* PeekThingSearchTools(); // 0x008fd000
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
struct CFilter_Bind2 {
    const CThingFilter_IsNot& First; const CThingFilter_IsChild& Second;
    CFilter_Bind2(const CThingFilter_IsNot& a,const CThingFilter_IsChild& b):First(a),Second(b){}
};
inline CFilter_Bind2 operator&&(const CThingFilter_IsNot& a,const CThingFilter_IsChild& b) { return CFilter_Bind2(a,b); }
struct CThingSearchToolsMethods : CThingSearchTools {
    bool GetPFirstThing(const CFilter_Bind2& filter,CThingAICreature** out); // 0x00914aa0
};
unsigned int __fastcall GFROR13(unsigned int); // 0x00497890

bool KidWanderMethods::IsRunnable() {
    if (((CThing*)((CAIStateGroupBase*)this)->PBrain->Creature)->bits[6] & 0x80000) {
        KidScheduler* scheduler=((CThing*)(((CAIStateGroupBase*)this)->PBrain->Creature))->GetTC<KidScheduler>(0xd3);
        if (!scheduler->IsPreferredIdle((CAIStateGroupBase*)this)) return false;
    }
    if (((CThing*)((CAIStateGroupBase*)this)->PBrain->Creature)->bits[1] & 0x20000) {
        CTCCreatureModeManager* modes=((CThing*)(((CAIStateGroupBase*)this)->PBrain->Creature))->GetTC<CTCCreatureModeManager>(0x31);
        if (modes->IsModeActive(8) || modes->IsModeActive(9)) {
            unsigned int* seed=((CWorldMethods*)GetPThingWorld())->GetPRandomSeed();
            *seed=*seed*0x24a1+0x24df;
            *seed=GFROR13(*seed);
            if (*seed % 100 > 10) return false;
        }
    }
    CThingAICreature* found;
    return ((CThingSearchToolsMethods*)PeekThingSearchTools())->GetPFirstThing(
        CThingFilter_IsNot((const CThing*)((CAIStateGroupBase*)this)->PBrain->Creature) && CThingFilter_IsChild(),
        &found);
}