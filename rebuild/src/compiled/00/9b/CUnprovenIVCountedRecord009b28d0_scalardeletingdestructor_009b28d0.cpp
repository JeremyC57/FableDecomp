// CUnprovenIVCountedRecord_009b28d0::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor of a record whose only non-trivial member is the intrusive counted pointer at +0x4: release it (Delete the pointee when its count reaches zero) and clear it, then free the record when bit 0 of the flags is set
// Leaf: ??_GCUnprovenIVCountedRecord_009b28d0@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B28D0, 49 bytes (+ int3 padding), thiscall, `ret 4`, returns this.
// Identity (descriptive): the deleting destructor of a non-polymorphic record {0x4 trivially destructible bytes, an intrusive
//   counted pointer +0x4, possibly more trivial bytes after it}; no call, jump or data reference reaches this address (VC7.1 emits
//   ??_G for every `delete p` / explicit `p->~T()` of a class with a non-trivial destructor, inlines it at the site, and the
//   retail link kept the COMDAT), so neither the record nor the pointee is provable: both are named CUnproven..._009b28d0.
//   FableWin.pdb lists ??_G for pair types of the 4-byte-key shape (pair<const unsigned long, CDefPointer<CDefClassBase> >, pair<long, CDefPointer<const CCon...> >, pair<long, CDefPointer<const CHeroAbili...> >), none of which a reference here can pin. Neighbours: the pair<float, EConversationAttitude> copy (0x009B2830) and CMessageEventFilter_CanBeSeenOrHeard::operator() (0x009B2930).
// Label: the manifest's "`vector_deleting_destructor'" is propagated (family J075: five bodies that differ only in the
//   pointer offset; retail has no identical-code folding, so each address is its own class's COMDAT).
// Callees: the pointee's Delete() const (vtable slot 1, `call [eax+4]`), operator delete 0x00BFE9BC.
// Shape: agent952 H021's CIVCountedPointer release (`p = Object; if (p) { if (--p->IVRefCount == 0) p->Delete(); Object = 0; }`:
//   one load of Object, `dec [ecx+4] ; jne`) inlined as the record's implicit destructor, then `if (flags & 1) operator delete(this)`.
//   The wrapper is modelled as Ego_r.pdb's CIVCountedPointer (CDefPointer<const T> compiles to the same bytes).
class CIVCountedPointeeBase {                            // Ego_r.pdb: vptr, IVRefCount
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                         // slot 1
    unsigned long IVRefCount;                            // +0x04
};
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
class CUnprovenPointee_009b28d0;                                       // descriptive: the counted pointee
class CUnprovenIVCountedRecord_009b28d0 {                              // descriptive: at least 0x8 bytes
public:
    unsigned long Head[1];                                 // +0x0: 0x4 trivially destructible bytes
    CIVCountedPointer<CUnprovenPointee_009b28d0 > Pointer;            // +0x4
};
// The delete expression that makes VC7.1 emit the scalar deleting destructor COMDAT.
void DeleteRecord_009b28d0(CUnprovenIVCountedRecord_009b28d0* p) { delete p; }