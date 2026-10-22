#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCTavernTankard_Destruct(void* tankard);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTavernTankard_CompositeScalarDeletingDestructor(void* tankard, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(tankard) + 52);
    CTCTavernTankard_Destruct(tankard);
    if (flags & 1)
        OperatorDelete(tankard);
    return tankard;
}