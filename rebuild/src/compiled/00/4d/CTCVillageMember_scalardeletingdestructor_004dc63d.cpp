#pragma optimize("s", on)
extern void __fastcall CTCVillageMember_Destruct(void* member);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCVillageMember_ScalarDeletingDestructor(void* member, int, unsigned int flags)
{
    CTCVillageMember_Destruct(member);
    if (flags & 1)
        OperatorDelete(member);
    return member;
}