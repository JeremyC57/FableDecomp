#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CIdleSchedulerDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CIdleSchedulerDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(def) + 60);
    CIdleSchedulerDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}