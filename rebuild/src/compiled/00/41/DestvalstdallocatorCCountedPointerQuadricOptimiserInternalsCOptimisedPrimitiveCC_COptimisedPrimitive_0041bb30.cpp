struct RefCounted {
    int count;      /* +0 */
    void (__fastcall *dtor)(void*); /* +4 */
    void* arg;      /* +8 */
};
struct CCountedPointer {
    void* self;         /* +0 */
    RefCounted* p;      /* +4 */
};
void operator delete(void*);

void __fastcall _Dest_val(CCountedPointer* self)
{
    RefCounted* p = self->p;
    if (p) {
        --p->count;
        p = self->p;
        if (p->count == 0) {
            p->dtor(p->arg);
            operator delete(self->p);
        }
    }
    self->self = 0;
    self->p = 0;
}