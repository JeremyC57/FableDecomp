struct S { char pad[0x393]; unsigned char f; };
// Leaf: EnableNavigator
void __fastcall EnableNavigator(S* self, void* edx, unsigned char v)
{
    (void)edx;
    self->f = v;
}