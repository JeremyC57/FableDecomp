#pragma optimize("s",on)
// Leaf: COptimisedPrimitive>_>
// _Dest_val<allocator<CCountedPointer<...>>> — destroy one counted-pointer slot.
// __fastcall this=ecx (no args). this+0 = object ptr, this+4 = refcount block.
// Block layout: [0]=refcount, [4]=releaser member-fn ptr, [8]=releaser `this`.
struct Owner;
struct Info {
    long rc;
    void (__fastcall *release)(Owner* self);
    Owner* owner;
};
struct Slot {
    void* p;
    Info* info;
    void DestVal();
};
void operator delete(void*);   // 0x00bfe9bc
void Slot::DestVal() {
    if (this->info) {
        --this->info->rc;
        if (this->info->rc == 0) {
            this->info->release(this->info->owner);
            operator delete(this->info);
        }
    }
    this->p = 0;
    this->info = 0;
}