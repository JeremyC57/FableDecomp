#ifndef FABLE_ENGINE_NUISYSTEM_COBSERVABLE_H
#define FABLE_ENGINE_NUISYSTEM_COBSERVABLE_H
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
// Complete original PDB fields; retail list4 gives native size16.
class CObservable {
#ifdef FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY
// NF226 opt-in (2026-11-22, q12567; row 0x0041E3F6 NUISystem::CManager::CManager): Ego_r.pdb (0x0043A1C1) / FableWin.pdb (0x01AE4FA4)
// ??0CObservable@NUISystem@@QAE@XZ, out of line in retail at 0x0042BE7B (46 bytes: the two observer lists, the vtable 0x01230044,
// m_pExclusiveObserver = 0), which the CManager constructor calls (`mov ecx,esi ; call 0x0042BE7B`). Declaration only; no layout change.
public: CObservable();                                                     // 0x0042BE7B
#endif
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
FABLE_STATIC_ASSERT_AT(23, sizeof(NUISystem::CObservable)==16);
#endif
