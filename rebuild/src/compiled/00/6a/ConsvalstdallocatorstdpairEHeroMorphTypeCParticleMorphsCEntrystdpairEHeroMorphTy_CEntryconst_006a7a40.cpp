struct Pair; struct Alloc;
extern void __fastcall cons_construct_0076d4f0(void* dst, Pair* val);
void __fastcall Cons_val(Alloc* self, Pair* dst, Pair* val)
{
    (void)dst;
    cons_construct_0076d4f0(*reinterpret_cast<void**>(self), val);
}