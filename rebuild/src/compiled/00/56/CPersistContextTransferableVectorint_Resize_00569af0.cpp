// CPersistContext_TransferableVector<int>::Resize(long) -- resizes the transferred vector of int to the given count, filling with int()
// Leaf: ?Resize@?$CPersistContext_TransferableVector@H@@EAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00569AF0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CPersistContext_TransferableVector<int>'s vtable 0x0124C5CC (the slot holds this body in 1 vtable(s), owner CPersistContext_TransferableVector<int>); the Xbox 360 Test PDB virtual order (third source) aligned on 2 TLC-named slots of that vtable puts Resize(long) at this slot [gap kind GS]; Ego_r/FableWin name set: ?Resize@?$CPersistContext_TransferableVector@H@@EAEXJ@Z; `ret` matches the signature.
// Was: CPersistContext_TransferableVector<int>::vfunc_1 (rebuild/src/compiled/00/56/global_UmoveEWeaponClass_00569af0.cpp).
// No shared header for the CPersistContext_TransferableVector<T> template: minimal local definition on its base
// CPersistContext_TransferableVectorGenericType (shared header; slots 0..3 Size / Resize / TransferOut / TransferIn, private
// overrides as Ego_r.pdb mangles them, EAE), with the Ego_r.pdb member TransVector (+0x04). The vector's resize is STLport's,
// called out of line here as in retail (the callee address is shared by several identical instances, COMDAT folding).
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CPersistContext_TransferableVectorGenericType.h"
template<class T> class CPersistContext_TransferableVector : public CPersistContext_TransferableVectorGenericType {
public:
    CPersistContext_TransferableVector(std::vector<T>& vector) : TransVector(&vector) {}
private:
    virtual long Size();                                                    // slot 0
    virtual void Resize(long);                                              // slot 1
    virtual void TransferOut(CPersistContext&, CCharString&, long);         // slot 2
    virtual void TransferIn(CPersistContext&, CStringParser&, long);        // slot 3
    std::vector<T>* TransVector;                                            // +0x04
};
template<class T> void CPersistContext_TransferableVector<T>::Resize(long size)
{
    TransVector->resize(size, T());
}
template void CPersistContext_TransferableVector<int>::Resize(long);
