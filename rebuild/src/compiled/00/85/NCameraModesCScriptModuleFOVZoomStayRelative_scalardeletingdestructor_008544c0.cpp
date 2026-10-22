#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CScriptModuleFOVZoomStayRelative_Destruct(void* relative);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CScriptModuleFOVZoomStayRelative_CompositeScalarDeletingDestructor(void* relative, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(relative) + 132);
    CScriptModuleFOVZoomStayRelative_Destruct(relative);
    if (flags & 1)
        OperatorDelete(relative);
    return relative;
}