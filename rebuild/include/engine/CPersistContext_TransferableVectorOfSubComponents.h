#ifndef FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTOROFSUBCOMPONENTS_H
#define FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTOROFSUBCOMPONENTS_H
// Retail model (chunk 1018, row 0x00F334E0 CPersistContext::TransferVectorOfSubComponents<CConversation>): the text-load wrapper the
// two-argument TransferVectorOfSubComponents<T> hands to CPersistContext::TransferVectorOfSubComponentsLoadText 0x009BC120.
// Ego_r.pdb / FableWin.pdb: ??0?$CPersistContext_TransferableVectorOfSubComponents@V<T>@@@@QAE@AAV?$vector@...@std@@@Z (inline: the
// 0x00F33621 arm stores the vtable 0x01301C2C and the vector pointer into a frame pair), and the five private virtual overrides
// ?Size@ / ?Resize@ / ?AreDefaultValsApplied@ / ?SetDefaultValsApplied@ / ?Transfer@?$CPersistContext_TransferableVectorOfSubComponents@V<T>@@@@EAE...
// (retail <CConversation> copies right after the row: Size 0x00F33660 (`sub ; imul` by sizeof(T)), AreDefaultValsApplied 0x00F33680
// (bit 2 of +0x24), SetDefaultValsApplied 0x00F336A0, Transfer 0x00F336D0 (element slot 18)). Member TransVector (Ego_r.pdb, +4).
// Needs FABLE_RETAIL_DEF_CLASS_BASE (T is a CDefClassBase).
#include "rebuild_abi.h"
#include "engine/CPersistContext_TransferableVectorOfSubComponentsGenericType.h"
#include "stlport/fable_containers.h"
template<class T> class CPersistContext_TransferableVectorOfSubComponents : public CPersistContext_TransferableVectorOfSubComponentsGenericType {
public:
    CPersistContext_TransferableVectorOfSubComponents(std::vector<T,std::allocator<T> >& value) : TransVector(&value) { }
private:
    virtual long Size() { return TransVector->size(); }
    virtual void Resize(long size) { TransVector->resize(size); }
    virtual bool AreDefaultValsApplied(long index) { return (*TransVector)[index].AreDefaultValsApplied(); }
    virtual void SetDefaultValsApplied(long index,bool value) { (*TransVector)[index].SetDefaultValsApplied(value); }
    virtual void Transfer(CPersistContext& context,long index) { (*TransVector)[index].Transfer(context); }
    std::vector<T,std::allocator<T> >* TransVector;
};
#endif
