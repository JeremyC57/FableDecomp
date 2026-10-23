#pragma optimize("t", on)
extern void __fastcall DestructMember_00739db0(void* member);
extern void __fastcall CModeCarryingPlateStand_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeCarryingPlateStand_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739db0(static_cast<unsigned char*>(mode) + 104);
    CModeCarryingPlateStand_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}