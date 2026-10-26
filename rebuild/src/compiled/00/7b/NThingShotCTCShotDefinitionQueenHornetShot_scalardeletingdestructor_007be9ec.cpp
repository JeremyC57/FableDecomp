#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionQueenHornetShot_Destruct(void* shot);
void* __fastcall CTCShotDefinitionQueenHornetShot_ScalarDeletingDestructor(void* shot, int, unsigned int flags)
{
    CTCShotDefinitionQueenHornetShot_Destruct(shot);
    if (flags & 1)
        operator delete(shot);
    return shot;
}