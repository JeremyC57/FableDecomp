// std::_Dest_val<allocator<CIVCountedPointer<T>>, CIVCountedPointer<T>>(CIVCountedPointer<T>*) -- release one intrusively counted pointer slot
// Leaf: CTriangleList>_>
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403E20, 27 bytes. Callees: vtable slot 1 of the pointee (its self-delete), indirect.
//
// CIVCountedPointer is our intrusive flavour of counted pointer: the object itself carries the
// reference count right after its vtable, so the pointer is a single word. Destroying a vector slot of
// them drops the count on the object and, when that was the last reference, asks the object to delete
// itself through vtable slot 1; the slot is then nulled. An empty slot is left alone. Named here for the
// N3DPrimitive::CTriangleList instantiation, but every CIVCountedPointer<T> compiles to this same body.
//
// Matching: `this` in ecx; `--c->ref == 0` folds to `dec [ecx+4]; jne`; the virtual call is
// `mov eax,[ecx]; call [eax+4]` with the object in ecx.
// Intrusively counted object: vtable then reference count.
struct Counted {
    void (__fastcall **vtbl)(Counted*);     // slot 1 deletes the object
    long ref;                               // reference count
};
// The pointer: one word.
struct Ptr {
    Counted* p;
};

void __fastcall Dest_val(Ptr* self)
{
    Counted* c = self->p;
    if (c != 0) {
        // Last reference: the object deletes itself.
        if (--c->ref == 0) {
            c->vtbl[1](c);
        }
        self->p = 0;
    }
}
