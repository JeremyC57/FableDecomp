#include <new>

// CCountedPointer<T>::CCountedPointer(T* object) -- take shared ownership of a freshly created object
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403D20, 78 bytes. Callees: operator new, deleter 0x00403600 (stored, not called).
//
// Our counted pointer is two words: the object, and a small control block shared by every pointer to
// that object. Constructing one from a raw pointer is how an object first enters shared ownership: we
// store the object, and when it is not null we allocate a 12-byte block holding a count of one, the
// per-type deleter that will `delete` the object when the count drops to zero, and the object itself
// (the deleter is called with it, not with the pointer). If the block cannot be allocated we keep the
// object but no block, which means it will never be deleted by us; a null object gets no block at all.
// The deleter stored here is the one at 0x00403600, which the symbol tables attach to
// CFileInstaller::CFileInfo; the "CProgressDisplay" in this file's name is the older propagated label.
//
// Matching: the two zero-stores to `blk` are not tail-merged: the alloc branch shares "self->blk = r"
// (xor eax,eax + mov [esi+4],eax when r==0) while the null branch is a separate literal store
// (mov [esi+4],0), hence the nested if/else plus separate else; no size-optimise pragma (7-byte
// immediate store, not and,0). The ctor returns `this` and pops its one stack argument (ret 4).

// Control block shared by all pointers to one object.
struct RefBlock { int count; int vtbl; void* ptr; };   // count, deleter routine, object
// The counted pointer itself.
struct CCountedPtr { void* obj; RefBlock* blk; };      // object, control block (null when unowned)

extern "C" CCountedPtr* __fastcall
CCountedPtr_ctor(CCountedPtr* self, int edxdummy, void* arg)
{
    void* a = arg;
    self->obj = a;
    if (a) {
        // First owner: make the control block with a count of one.
        RefBlock* b = (RefBlock*)::operator new(0xc);
        RefBlock* r;
        if (b) {
            void* o = self->obj;
            b->count = 1;
            b->vtbl = 0x403600;
            b->ptr = o;
            r = b;
        } else {
            r = 0;
        }
        self->blk = r;
    } else {
        self->blk = 0;
    }
    return self;
}
