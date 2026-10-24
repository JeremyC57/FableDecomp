struct S { char pad[0x38C]; void* o; };
// Leaf: AddExclusiveObserver
void __fastcall AddExclusiveObserver(S* self, void* edx, void* arg)
{
    (void)edx;
    self->o = arg;
}