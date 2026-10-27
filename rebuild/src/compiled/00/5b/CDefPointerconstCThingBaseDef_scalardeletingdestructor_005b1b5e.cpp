// CDefPointer<const CThingBaseDef>::`scalar deleting destructor'(unsigned int flags) -- release the counted def (Destroy when its count reaches zero), clear the slot, then free the wrapper when bit 0 of the flags is set
// Leaf: ??_G?$CDefPointer@$$CBVCThingBaseDef@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005b1b5e, 42 bytes, thiscall, `ret 4`, returns this (size-optimised TU: unaligned address).
// Identity (probable): called from the /Os destroy range 0x005B26E9, which the landed std::vector<CDefPointer<const CThingBaseDef> >::operator= 0x00721150 / _M_clear 0x005B31EC, CTCHeroMorph::Copy / AddTattoo (Ego_r.pdb CTCHeroMorph::Tattoos) and ~CTCObjectAugmentations (CTCObjectAugmentations::Augmentations), both vector<CDefPointer<const CThingBaseDef> >, call; and from the map-node constructor 0x005B206C.
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
class CThingBaseDef;
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
void DeleteDefPointer_005b1b5e(CDefPointer<const CThingBaseDef>* pointer) { delete pointer; }