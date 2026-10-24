void operator delete(void*);
// Leaf: COptimisedPrimitive>_>

struct Counter {
    long refcount;
    void (__fastcall *dtor)(void*);
    void* obj;
};

struct CCP {
    void* p0;
    Counter* pctr;
};

void __fastcall _Dest_val(CCP* self)
{
    Counter* c = self->pctr;
    if (c) {
        (self->pctr->refcount)--;
        c = self->pctr;
        if (c->refcount == 0) {
            (c->dtor)(c->obj);
            operator delete(self->pctr);
        }
    }
    self->p0 = 0;
    self->pctr = 0;
}