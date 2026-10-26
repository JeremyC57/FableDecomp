#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionNymphFun_Destruct(void* fun);
void* __fastcall CTCShotDefinitionNymphFun_ScalarDeletingDestructor(void* fun, int, unsigned int flags)
{
    CTCShotDefinitionNymphFun_Destruct(fun);
    if (flags & 1)
        operator delete(fun);
    return fun;
}