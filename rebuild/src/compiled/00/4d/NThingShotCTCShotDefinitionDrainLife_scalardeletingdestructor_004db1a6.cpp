#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionDrainLife_Destruct(void* life);
void* __fastcall CTCShotDefinitionDrainLife_ScalarDeletingDestructor(void* life, int, unsigned int flags)
{
    CTCShotDefinitionDrainLife_Destruct(life);
    if (flags & 1)
        operator delete(life);
    return life;
}