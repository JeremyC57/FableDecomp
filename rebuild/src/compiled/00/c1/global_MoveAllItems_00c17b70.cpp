// _global::MoveAllItems(void* owner, PKCContainer_00c17b70* source, PKCContainer_00c17b70* dest) -- reserves room in dest for both containers' items, then moves every item the source still yields (its first item, re-queried after each move) from source to dest under the given owner
// Leaf: MoveAllItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17B70, 103 bytes (+ int3 padding), __fastcall (owner in ecx, source in
//   edx, dest on the stack, `ret 4`).
// Identity: libpk's abstract container interface (the one the event set at +0x2c of the sound-event manager and the
//   +0x10 set of 0x00C14FA0 use: slot 11 (+0x2c) yields the first item): slot 4 (+0x10) reserves, slot 5 (+0x14) adds
//   (owner, item), slot 6 (+0x18) removes (owner, item), slot 10 (+0x28) counts. Callers: 0x00C17D60, 0x00C189EF.
// Shape: `dest->Reserve(source->GetCount() + dest->GetCount())` fetches dest's vtable before both counts (the nested
//   virtual call); the owner (ecx) survives the calls in the `push ecx` frame word and moves to ebp once the sum is
//   dead; the item stays in edi.
// Label correction: the catalog's `MergeContainers_ProcessQueues` is descriptive; names descriptive.
class PKCContainer_00c17b70 {
public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03();
    virtual void Reserve(long count);                          // slot 4 (+0x10)
    virtual void Add(void* owner, void* item);                 // slot 5 (+0x14)
    virtual void Remove(void* owner, void* item);              // slot 6 (+0x18)
    virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual long GetCount();                                   // slot 10 (+0x28)
    virtual void* GetFirst();                                  // slot 11 (+0x2c)
};

void __fastcall MoveAllItems(void* owner, PKCContainer_00c17b70* source, PKCContainer_00c17b70* dest)
{
    dest->Reserve(source->GetCount() + dest->GetCount());
    for (void* item = source->GetFirst(); item; item = source->GetFirst()) {
        source->Remove(owner, item);
        dest->Add(owner, item);
    }
}