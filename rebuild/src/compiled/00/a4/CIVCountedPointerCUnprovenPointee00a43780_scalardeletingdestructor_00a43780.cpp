// CIVCountedPointer<CUnprovenPointee_00a43780>::`scalar deleting destructor'(unsigned int) -- release the intrusively counted pointee (Delete when the count reaches zero), clear the slot, and free the wrapper when bit 0 of the flags is set
// Leaf: ??_G?$CIVCountedPointer@VCUnprovenPointee_00a43780@@@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43780, 47 bytes (+ int3 padding), thiscall, `ret 4`, returns this.
// Identity (descriptive): the compiler-generated scalar deleting destructor of a one-pointer intrusive wrapper; its retail neighbours are the CLipSyncDataBankEntry / NGameText::CDataBankTextEntry TU (0x00A43580..0x00A43B20).
//   No call, jump or data reference reaches this address (its deleting site was inlined, VC7.1 still emits the COMDAT
//   and the retail link kept it), so nothing proves the wrapper's template argument: the pointee is named CUnprovenPointee_00a43780.
//   Ego_r.pdb names the one folded body of this shape ??_G?$CDefPointer@$$CBVCHeroAbilityDef@@@@QAEPAXI@Z (the
//   manifest label, propagated to seven retail bodies; retail has no identical-code folding); FableWin.pdb has ??_G for six
//   CDefPointer<const T> and four CIVCountedPointer<T> instantiations, which all compile to these bytes. The wrapper is
//   modelled as Ego_r.pdb's CIVCountedPointer (Object: CIVCountedPointeeBase*, IVRefCount +4, Delete vtable slot 1).
// Callees: the pointee's Delete() const (vtable slot 1), operator delete 0x00BFE9BC.
// Shape: `~CIVCountedPointer() { p = Object; if (p) { if (--p->IVRefCount == 0) p->Delete(); Object = 0; } }` (the
//   landed Release 0x007090F0 form: one load of Object, `dec [ecx+4] ; jne`), then `if (flags & 1) operator delete(this)`.
class CIVCountedPointeeBase {                            // Ego_r.pdb: vptr, IVRefCount
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                         // slot 1
    unsigned long IVRefCount;                            // +0x04
};
class CUnprovenPointee_00a43780;                                             // descriptive: the unproven pointee
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
void DeleteIVCountedPointer_00a43780(CIVCountedPointer<CUnprovenPointee_00a43780 >* pointer) { delete pointer; }