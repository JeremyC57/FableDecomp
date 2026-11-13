#ifndef FABLE_ENGINE_CBANKSTATEBLOCK_H
#define FABLE_ENGINE_CBANKSTATEBLOCK_H
// New header (chunk 2018, row 0x00B2D330). RTTI .?AVCBankStateBlock@@ (no bases; vtable 0x0129C82C, slot 0 scalar deleting dtor
// 0x009FCCA0): the polymorphic root of the bank state blocks. Ego_r.pdb ??0CBankStateBlock@@QAE@XZ, ??1CBankStateBlock@@UAE@XZ,
// ?Reset@CBankStateBlock@@QAEXPBV1@@Z; the derived vtables (CGraphicBankStateBlock 0x0129C888) put GetDefault at slot 1, CreateCRC at
// slot 2 and the protected Set at slot 3 (rtti-vtables.tsv, manifest labels of 0x009FCEF0 / 0x00A00940 / 0x009FD020). No data members.
#include "rebuild_abi.h"
class CBankStateBlock {
public:
    CBankStateBlock() { }
    virtual ~CBankStateBlock();                                          // slot 0
    virtual void GetDefault(CBankStateBlock& block) = 0;                 // slot 1
    virtual unsigned long CreateCRC() = 0;                               // slot 2
    void Reset(const CBankStateBlock* block);                            // ?Reset@CBankStateBlock@@QAEXPBV1@@Z
protected:
    virtual void Set(const CBankStateBlock* block) = 0;                  // slot 3
};
FABLE_STATIC_ASSERT(sizeof(CBankStateBlock) == 0x4);
#endif // FABLE_ENGINE_CBANKSTATEBLOCK_H
