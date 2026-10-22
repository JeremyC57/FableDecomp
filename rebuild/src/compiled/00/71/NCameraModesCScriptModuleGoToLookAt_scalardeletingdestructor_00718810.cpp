#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CScriptModuleGoToLookAt_Destruct(void* lookAt);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CScriptModuleGoToLookAt_CompositeScalarDeletingDestructor(void* lookAt, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(lookAt) + 76);
    CScriptModuleGoToLookAt_Destruct(lookAt);
    if (flags & 1)
        OperatorDelete(lookAt);
    return lookAt;
}