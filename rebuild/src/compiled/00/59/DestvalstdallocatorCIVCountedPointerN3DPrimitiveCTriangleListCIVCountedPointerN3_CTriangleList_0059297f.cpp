#pragma optimize("s",on)
// Leaf: ?Dest_val_CTriangleList___@@YIXPAUCIVCountedPointer_CTriangleList___@@@Z
struct Counted {
    void** vtable;
    int    refcount;
};

struct CIVCountedPointer_CTriangleList___ {
    Counted* ptr;
};

void __fastcall Dest_val_CTriangleList___(CIVCountedPointer_CTriangleList___* self)
{
    Counted* c = self->ptr;
    if (c) {
        if (--c->refcount == 0) {
            typedef void (__fastcall *pfn)(Counted*);
            ((pfn)c->vtable[1])(c);
        }
        self->ptr = 0;
    }
}