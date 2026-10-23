#pragma optimize("s", on)
extern void __fastcall DestructMember_0045645d(void* member);
extern void __fastcall CRegionDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CRegionDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_0045645d(static_cast<unsigned char*>(def) + 80);
    CRegionDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}