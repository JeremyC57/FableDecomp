// _Dest_val body inlined into a vector deleting destructor: release the counted-pointer
// Leaf: COptimisedPrimitive>_>
// slot, clear both words, then optionally free the object itself.
// __fastcall this=ecx, flags=stack (ret 4).
struct Owner;
struct Info {
    long rc;
    void (__fastcall *release)(Owner* self);
    Owner* owner;
};
struct Slot {
    void* p;
    Info* info;
    void* VecDel(unsigned flags);
};
void operator delete(void*);   // 0x00bfe9bc
void* Slot::VecDel(unsigned flags) {
    if (this->info) {
        --this->info->rc;
        if (this->info->rc == 0) {
            this->info->release(this->info->owner);
            operator delete(this->info);
        }
    }
    this->p = 0;
    this->info = 0;
    if (flags & 1) operator delete(this);
    return this;
}