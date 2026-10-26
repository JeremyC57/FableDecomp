#pragma optimize("s",on)
// Leaf: COptimisedPrimitive>_>
struct Ctrl { int refcount; void (__fastcall *destroy)(void*); void* arg; };
struct CCP { void* val; Ctrl* ctrl; };


void __fastcall Dest_val(CCP* self)
{
    Ctrl* c = self->ctrl;
    if (c) {
        c->refcount--;
        c = self->ctrl;
        if (c->refcount == 0) {
            c->destroy(c->arg);
            operator delete(self->ctrl);
        }
    }
    self->val = 0;
    self->ctrl = 0;
}