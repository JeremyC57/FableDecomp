extern "C" void __cdecl free(void*);
typedef void (__cdecl *FreeFn)(void*);
extern FreeFn __imp_free_0144011c;
FreeFn __imp_free_0144011c = &free;
void __cdecl operator_delete(void* p)
{
    __imp_free_0144011c(p);
}