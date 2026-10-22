#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionDrainLife_Destruct(void* life);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionDrainLife_ScalarDeletingDestructor(void* life, int, unsigned int flags)
{
    CTCShotDefinitionDrainLife_Destruct(life);
    if (flags & 1)
        OperatorDelete(life);
    return life;
}