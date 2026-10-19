void operator delete(void*);

struct RefBlock {
    int count;
    void (__fastcall *dtor)(void*);
    void* arg;
};

struct CCountedPointer {
    void* ptr;
    RefBlock* ref;
};

void __fastcall Dest_val(CCountedPointer* self)
{
    RefBlock* r = self->ref;
    if (r) {
        --r->count;
        r = self->ref;
        if (r->count == 0) {
            r->dtor(r->arg);
            operator delete(self->ref);
        }
    }
    self->ptr = 0;
    self->ref = 0;
}