#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_009a4400(void* member);
extern void __fastcall DestructSecondMember_006e46f0(void* member);
extern void __fastcall CData_Destruct(void* data);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CData_CompositeScalarDeletingDestructor(void* data, int, unsigned int flags)
{
    DestructFirstMember_009a4400(static_cast<unsigned char*>(data) + 36);
    DestructSecondMember_006e46f0(static_cast<unsigned char*>(data) + 28);
    CData_Destruct(data);
    if (flags & 1)
        OperatorDelete(data);
    return data;
}