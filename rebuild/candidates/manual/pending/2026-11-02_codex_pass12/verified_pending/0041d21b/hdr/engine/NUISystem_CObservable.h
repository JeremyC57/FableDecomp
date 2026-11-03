#ifndef FABLE_ENGINE_NUISYSTEM_COBSERVABLE_H
#define FABLE_ENGINE_NUISYSTEM_COBSERVABLE_H
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
// Complete original PDB fields; retail list4 gives native size16.
class CObservable {
public: virtual void SetEvent(NUISystem::EEvent);
public: virtual void SetOwnedEvent(NUISystem::CObserver *,NUISystem::EEvent);
public: virtual void AddObserver(NUISystem::CObserver *);
public: virtual void AddExclusiveObserver(NUISystem::CObserver *);
public: virtual void RemoveExclusiveObserver(NUISystem::CObserver *);
public: virtual void RemoveObserver(NUISystem::CObserver *);
public: virtual void ClearObservers();
public: virtual NUISystem::CObserver * GetExclusiveObserver();
public: virtual bool IsExclusivelyObserved();
public: virtual void AddConcurrentExclusiveObserver(NUISystem::CObserver *);
public: virtual void RemoveConcurrentExclusiveObserver(NUISystem::CObserver *);
protected: std::list<NUISystem::CObserver *,std::allocator<NUISystem::CObserver *> > Observers;
protected: NUISystem::CObserver * m_pExclusiveObserver;
protected: std::list<NUISystem::CObserver *,std::allocator<NUISystem::CObserver *> > ConcurrentExclusiveObservers;
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CObservable)==16);
#endif
