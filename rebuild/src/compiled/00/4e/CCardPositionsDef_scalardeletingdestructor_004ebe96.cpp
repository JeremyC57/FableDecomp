#pragma optimize("s", on)
extern void __fastcall DestructMember_00440d60(void* member);
extern void __fastcall CCardPositionsDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CCardPositionsDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_00440d60(static_cast<unsigned char*>(def) + 60);
    CCardPositionsDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}