#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionThrow_Destruct(void* definitionThrow);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionThrow_ScalarDeletingDestructor(void* definitionThrow, int, unsigned int flags)
{
    CTCShotDefinitionThrow_Destruct(definitionThrow);
    if (flags & 1)
        OperatorDelete(definitionThrow);
    return definitionThrow;
}