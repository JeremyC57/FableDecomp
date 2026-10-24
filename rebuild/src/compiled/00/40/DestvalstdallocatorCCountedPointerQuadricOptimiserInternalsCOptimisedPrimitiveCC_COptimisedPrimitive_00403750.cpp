// std::_Dest_val<allocator<CCountedPointer<T>>, CCountedPointer<T>>(CCountedPointer<T>*) -- release one counted-pointer slot
// Leaf: COptimisedPrimitive>_>
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403750, 53 bytes. Callees: the control block's releaser (indirect), engine free 0x00BFE9BC.
//
// STLport's allocator "destroy" hook over a CCountedPointer element, which inlines the counted-pointer
// destructor. A CCountedPointer is two words: the object and a shared control block holding the reference
// count, a release routine and the owner to call it on. Destroying the slot drops the count; when it
// reaches zero the release routine is invoked for the owner (that is what deletes the object) and the
// control block itself is returned to the engine heap. Both words are then nulled so a stale slot can
// never be released twice. A slot with no control block is already empty and only gets nulled.
//
// The name attached to this address is the QuadricOptimiser instantiation; the body is identical for
// every CCountedPointer<T>, so the same bytes serve many element types.
//
// Matching: `this` in ecx; the count is decremented in memory and compared to zero; the releaser is
// called through the block's member pointer with the block's owner word in ecx; the free is __cdecl.
struct Owner;
// The shared control block behind a counted pointer.
struct Info {
    long rc;                                    // reference count
    void (__fastcall *release)(Owner* self);    // routine that destroys the pointee
    Owner* owner;                               // object the releaser is invoked on
};
// CCountedPointer<T>: the pointee and its control block.
struct Slot {
    void* p;        // pointee
    Info* info;     // control block (null when the pointer is empty)
    void DestVal();
};
// Engine heap release for the control block.
void operator delete(void*);   // 0x00bfe9bc
void Slot::DestVal() {
    if (this->info) {
        // Last reference away: destroy the pointee and the block.
        --this->info->rc;
        if (this->info->rc == 0) {
            this->info->release(this->info->owner);
            operator delete(this->info);
        }
    }
    // Leave the slot empty.
    this->p = 0;
    this->info = 0;
}
