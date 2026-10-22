#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionNymphFun_Destruct(void* fun);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionNymphFun_ScalarDeletingDestructor(void* fun, int, unsigned int flags)
{
    CTCShotDefinitionNymphFun_Destruct(fun);
    if (flags & 1)
        OperatorDelete(fun);
    return fun;
}