#ifndef FABLE_ENGINE_CINTELLIGENTPOINTER_CREATURESIZES_H
#define FABLE_ENGINE_CINTELLIGENTPOINTER_CREATURESIZES_H
#include "engine/CIntelligentPointer.h"
#ifdef FABLE_RETAIL_BUY_FROM_SELLER_SIZE_OWNERS_PASS30
// Fidelity: scaffolding, not retail. The linked creature-pointer deleting destructor owners
// 004D6915 and 004D6931 each set optimize("s", on). Keep the already-instantiated game
// object and STLport members in command-line mode before opening the two creature types.
#include "engine/CThingGameObject.h"
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#endif

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
#ifdef FABLE_RETAIL_BUY_FROM_SELLER_SIZE_OWNERS_PASS30
// Used with the existing CREATUREBASE_SIZE_PASS27 specialization above.
class CThingAICreature;
template<> class CIntelligentPointer<CThingAICreature> : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(CThingAICreature* pointer);
    CIntelligentPointer(const CIntelligentPointer& other) { SetPointer(other.GetPointer()); }
    template<class U> CIntelligentPointer(CIntelligentPointer<U>& other) { SetPointer(other.Get()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) {
        CBaseObject* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    void Set(CThingAICreature* pointer) { SetPointer(pointer); }
    CThingAICreature& operator*() const { return *GetPointer(); }
    CThingAICreature* Get() const { return GetPointer(); }
    CThingAICreature* operator->() const { return GetPointer(); }
private:
    void SetPointer(const CThingAICreature* pointer);
    CThingAICreature* GetPointer() const;
};
#endif
#pragma pack(pop)
#undef FABLE_RETAIL_IPTR_SIZE_POINTEE
#endif

#ifdef FABLE_RETAIL_BUY_FROM_SELLER_SIZE_OWNERS_PASS30
#include "engine/CThingAICreature.h"
inline void CIntelligentPointer<CThingCreatureBase>::SetPointer(const CThingCreatureBase* pointer) {
    SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer)));
}
inline CThingCreatureBase* CIntelligentPointer<CThingCreatureBase>::GetPointer() const {
    return static_cast<CThingCreatureBase*>(GetPItem());
}
inline void CIntelligentPointer<CThingAICreature>::SetPointer(const CThingAICreature* pointer) {
    SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer)));
}
inline CThingAICreature* CIntelligentPointer<CThingAICreature>::GetPointer() const {
    return static_cast<CThingAICreature*>(GetPItem());
}

// Restore the command-line /O2 mode for both pointer constructors and Update.
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
inline CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase* pointer) { SetPointer(pointer); }
inline CIntelligentPointer<CThingAICreature>::CIntelligentPointer(CThingAICreature* pointer) { SetPointer(pointer); }
#endif

#endif
