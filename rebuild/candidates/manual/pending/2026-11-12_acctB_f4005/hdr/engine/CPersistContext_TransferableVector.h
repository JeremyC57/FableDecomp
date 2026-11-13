#ifndef FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTOR_H
#define FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTOR_H
// Retail model (chunk 4004, row 0x00F2D6E0 CScriptDef::Transfer): the text-mode wrapper the named CPersistContext::TransferVector<T>
// hands to TransferVectorSaveText 0x009BAC80 / TransferVectorLoadText 0x009BB110. Ego_r.pdb / FableWin.pdb:
// ??0?$CPersistContext_TransferableVector@<T>@@QAE@AAV?$vector@...@std@@@Z (inline in retail: 0x00F33427 stores the vtable 0x01301C1C and
// the vector pointer into a frame pair) and the four private virtual overrides ?Size@ / ?Resize@ / ?TransferOut@ / ?TransferIn@
// ?$CPersistContext_TransferableVector@<T>@@EAE... (retail <CDefString> copies right after the row's TransferVector<CDefString>:
// Size 0x00F33470, TransferIn 0x00F33480, TransferOut 0x00F334A0). Member TransVector (Ego_r.pdb, +4). The overrides are declared
// only: their bodies are the landed rows of each instance.
#include "rebuild_abi.h"
#include "engine/CPersistContext_TransferableVectorGenericType.h"
#include "stlport/fable_containers.h"
template<class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
public:
    CPersistContext_TransferableVector(std::vector<T,std::allocator<T> >& value) : TransVector(&value) { }
private:
    virtual long Size();
    virtual void Resize(long size);
    virtual void TransferOut(CPersistContext& context,CCharString& out,long index);
    virtual void TransferIn(CPersistContext& context,CStringParser& parser,long index);
    std::vector<T,std::allocator<T> >* TransVector;
};
#endif
