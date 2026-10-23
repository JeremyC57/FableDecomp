#pragma optimize("t", on)
extern void __fastcall DestructMember_00450ba0(void* member);
extern void __fastcall CThingBaseDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CThingBaseDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_00450ba0(static_cast<unsigned char*>(def) + 57);
    CThingBaseDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}