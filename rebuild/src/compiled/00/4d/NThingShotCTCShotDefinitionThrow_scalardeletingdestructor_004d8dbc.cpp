#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionThrow_Destruct(void* definitionThrow);
void* __fastcall CTCShotDefinitionThrow_ScalarDeletingDestructor(void* definitionThrow, int, unsigned int flags)
{
    CTCShotDefinitionThrow_Destruct(definitionThrow);
    if (flags & 1)
        operator delete(definitionThrow);
    return definitionThrow;
}