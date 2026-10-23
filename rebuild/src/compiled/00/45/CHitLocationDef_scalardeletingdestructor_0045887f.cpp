#pragma optimize("s", on)
extern void __fastcall DestructMember_0044f3b2(void* member);
extern void __fastcall CHitLocationDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CHitLocationDef_CompositeScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    DestructMember_0044f3b2(static_cast<unsigned char*>(def) + 60);
    CHitLocationDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}