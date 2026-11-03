#ifndef FABLE_ENGINE_CRESOURCEBANK_H
#define FABLE_ENGINE_CRESOURCEBANK_H
// Retail layout (hand-written, chunk 712; no generated header existed). RTTI .?AVCResourceBank@@ : CBaseClassNonCopyable :
// CBaseClass (Fable.exe COL of vtable 0x0129C814, one slot: the scalar deleting dtor 0x009FC750). Members = Ego_r.pdb
// CResourceBank (0x7c, no STL container, so the offsets are retail's): ResourceList (CResourceList, 0x3c) +0x04, LoadingQueue
// (CResourceList) +0x40. Retail check: FrameEnd 0x009FC430 increments ResourceList.CurrentFrame (+0x30 = +0x04 + 0x2c) and
// LoadingQueue.CurrentFrame (+0x6c = +0x40 + 0x2c) and zeroes both UnloadedThisFrame (+0x3c / +0x78); ProcessLoadingQueue
// 0x009FC410 works on `this + 0x40`; the four banks of CMeshDataBank sit 0x7c apart (+0x170 / +0x1ec / +0x268 / +0x2e4).
// Member functions: the Ego_r.pdb / FableWin.pdb set (QAE; the dtor UAE). Retail addresses where known.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CResourceList.h"

class CResource;

class CResourceBank : public CBaseClassNonCopyable {
public:
    CResourceBank();                                                     // ??0CResourceBank@@QAE@XZ
    virtual ~CResourceBank();                                            // slot 0 (scalar deleting 0x009FC750)
    void AddResource(CResource& resource);                               // ?AddResource@CResourceBank@@QAEXAAVCResource@@@Z
    void RemoveResource(CResource& resource);                            // ?RemoveResource@CResourceBank@@QAEXAAVCResource@@@Z
    void AddToLoadingQueue(CResource& resource);                         // ?AddToLoadingQueue@CResourceBank@@QAEXAAVCResource@@@Z
    void ProcessLoadingQueue(bool flag, double time);                    // 0x009FC410 ?ProcessLoadingQueue@CResourceBank@@QAEX_NN@Z
    void FrameEnd();                                                     // 0x009FC430 ?FrameEnd@CResourceBank@@QAEXXZ
    CResourceList ResourceList;                                          // +0x04
    CResourceList LoadingQueue;                                          // +0x40
};
FABLE_STATIC_ASSERT(sizeof(CResourceBank) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CResourceBank, ResourceList) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CResourceBank, LoadingQueue) == 0x40);

#endif // FABLE_ENGINE_CRESOURCEBANK_H
