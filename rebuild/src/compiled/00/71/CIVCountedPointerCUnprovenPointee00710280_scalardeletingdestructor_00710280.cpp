// CIVCountedPointer<CUnprovenPointee_00710280>::`scalar deleting destructor'(unsigned int) -- release the intrusively counted pointee (Delete when the count reaches zero), clear the slot, and free the wrapper when bit 0 of the flags is set
// Leaf: ??_G?$CIVCountedPointer@VCUnprovenPointee_00710280@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00710280, 47 bytes (+ int3 padding), thiscall, `ret 4`, returns this.
// Identity (descriptive): the compiler-generated scalar deleting destructor of a one-pointer intrusive wrapper; no call,
//   jump or data reference reaches 0x00710280 (its deleting site was inlined; VC7.1 still emits the COMDAT and the retail
//   link kept it), so the template argument is unprovable: CUnprovenPointee_00710280. Neighbourhood: landed neighbours 0x007101A0 std::vector ctor, 0x00710310 std::merge, 0x007103C0 std::__rotate (the animation stable_sort chain).
//   Ego_r.pdb folds this body as ??_G?$CDefPointer@$$CBVCHeroAbilityDef@@@@QAEPAXI@Z (the propagated manifest label);
//   FableWin.pdb's unfolded ??_G instantiations (CDefPointer<const T> / CIVCountedPointer<T>) all compile to these bytes.
//   Family K011 (agent954) = agent952's H021 shape (landed 0x00708DA0).
// Callees: the pointee's Delete() const (vtable slot 1), operator delete 0x00BFE9BC.
// Shape: `~CIVCountedPointer() { p = Object; if (p) { if (--p->IVRefCount == 0) p->Delete(); Object = 0; } }` (one load
//   of Object, `dec [ecx+4] ; jne`), then `if (flags & 1) operator delete(this)`.
class CIVCountedPointeeBase {                            // Ego_r.pdb: vptr, IVRefCount
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                         // slot 1
    unsigned long IVRefCount;                            // +0x04
};
class CUnprovenPointee_00710280;                                             // descriptive: the unproven pointee
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;                       // +0x0
    ~CIVCountedPointer()
    {
        CIVCountedPointeeBase* p = Object;
        if (p) {
            if (--p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
};
// The delete expression that makes VC7.1 emit the scalar deleting destructor COMDAT.
void DeleteIVCountedPointer_00710280(CIVCountedPointer<CUnprovenPointee_00710280 >* pointer) { delete pointer; }