// CDefPointer<const CUnprovenDef_005baa41>::`scalar deleting destructor'(unsigned int flags) -- release the counted def (Destroy when its count reaches zero), clear the slot, then free the wrapper when bit 0 of the flags is set
// Leaf: ??_G?$CDefPointer@$$CBVCUnprovenDef_005baa41@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005baa41, 42 bytes, thiscall, `ret 4`, returns this (size-optimised TU: unaligned address).
// Identity (descriptive): called from the uninitialized copy 0x005BAEC4 and 0x005BB190 (labelled CNavigatorQueueManager::FrameUpdate, propagated) of a 4-byte counted-def vector; the pointee type is not proven (the manifest's CHeroAbilityDef is propagated).
// Callees: the def's Destroy() (vtable slot 1), operator delete 0x00BFE9BC.
// Shape (family L012): `#pragma optimize("s", on)`; ~CDefPointer must be __forceinline (an /Os TU otherwise calls it out of
//   line from the ??_G), the pointee's inline Release() gives `dec [ecx+4] ; jne`, `Object = 0` is `and [esi],0`.
#pragma optimize("s", on)
class CDefPointeeBase {                                    // vptr; reference count at +4
public:
    virtual ~CDefPointeeBase();
    virtual void Destroy() const;                         // slot 1
    mutable long RefCount;                                // +0x04
    __forceinline void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CUnprovenDef_005baa41;                               // descriptive: the def type is unproven
template <class T> class CDefPointer {                     // Ego_r.pdb, 4 bytes
public:
    const CDefPointeeBase* Object;                        // +0x00
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
// The delete expression that makes VC7.1 emit the scalar deleting destructor COMDAT.
void DeleteDefPointer_005baa41(CDefPointer<const CUnprovenDef_005baa41>* pointer) { delete pointer; }