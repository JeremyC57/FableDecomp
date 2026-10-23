#pragma optimize("s", on)
extern void __fastcall DestructMember_00414ea0(void* member);
extern void __fastcall CTCNamedUserActionPoints_Destruct(void* points);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCNamedUserActionPoints_CompositeScalarDeletingDestructor(void* points, int, unsigned int flags)
{
    DestructMember_00414ea0(static_cast<unsigned char*>(points) + 52);
    CTCNamedUserActionPoints_Destruct(points);
    if (flags & 1)
        OperatorDelete(points);
    return points;
}