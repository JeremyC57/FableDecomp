// CDefPointer<const CHeroAbilityDef>::`scalar deleting destructor'(unsigned int flags) -- release the counted def (Destroy when its count reaches zero), clear the slot, then free the wrapper when bit 0 of the flags is set
// Leaf: ??_G?$CDefPointer@$$CBVCHeroAbilityDef@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005f861e, 42 bytes, thiscall, `ret 4`, returns this (size-optimised TU: unaligned address).
// Identity (probable): called from the /Os destroy range 0x005F939F, which the landed std::vector<CDefPointer<const CHeroAbilityDef> >::operator= 0x005F9F5A and vector<pair<long, CDefPointer<const CHeroAbilityDef> > >::_M_allocate_and_copy 0x005F9A3C call, and from the partition 0x005F8CE2 of the same TU.
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
class CHeroAbilityDef;
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
void DeleteDefPointer_005f861e(CDefPointer<const CHeroAbilityDef>* pointer) { delete pointer; }