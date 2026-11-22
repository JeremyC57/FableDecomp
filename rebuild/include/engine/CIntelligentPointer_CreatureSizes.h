#ifndef FABLE_ENGINE_CINTELLIGENTPOINTER_CREATURESIZES_H
#define FABLE_ENGINE_CINTELLIGENTPOINTER_CREATURESIZES_H
#include "engine/CIntelligentPointer.h"

#if defined(FABLE_RETAIL_CINTELLIGENTPOINTER_CREATUREBASE_SIZE_PASS27)
class CThingCreatureBase;
#define FABLE_RETAIL_IPTR_SIZE_POINTEE CThingCreatureBase
#elif defined(FABLE_RETAIL_CINTELLIGENTPOINTER_AICREATURE_SIZE_PASS27)
class CThingAICreature;
#define FABLE_RETAIL_IPTR_SIZE_POINTEE CThingAICreature
#endif

#ifdef FABLE_RETAIL_IPTR_SIZE_POINTEE
// Fidelity: scaffolding, not retail. Specialize the same packed pointer API
// so its constructor can keep the linked speed body and its generated
// deleting destructor can keep the linked size body.
#pragma pack(push, 1)
template<> class CIntelligentPointer<FABLE_RETAIL_IPTR_SIZE_POINTEE> : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(FABLE_RETAIL_IPTR_SIZE_POINTEE* pointer);
    template<class U> CIntelligentPointer(CIntelligentPointer<U>& other) { SetPointer(other.Get()); }
    void Set(FABLE_RETAIL_IPTR_SIZE_POINTEE* pointer) { SetPointer(pointer); }
    FABLE_RETAIL_IPTR_SIZE_POINTEE& operator*() const { return *GetPointer(); }
    FABLE_RETAIL_IPTR_SIZE_POINTEE* Get() const { return GetPointer(); }
    FABLE_RETAIL_IPTR_SIZE_POINTEE* operator->() const { return GetPointer(); }
private:
    void SetPointer(const FABLE_RETAIL_IPTR_SIZE_POINTEE* pointer);
    FABLE_RETAIL_IPTR_SIZE_POINTEE* GetPointer() const;
};
#pragma pack(pop)
#undef FABLE_RETAIL_IPTR_SIZE_POINTEE
#endif

#endif
