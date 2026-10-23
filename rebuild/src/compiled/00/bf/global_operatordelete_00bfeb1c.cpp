extern "C" void __cdecl free(void*);
typedef void (__cdecl *FreeFn)(void*);
extern FreeFn __imp_free_01440120;
FreeFn __imp_free_01440120 = &free;
void __cdecl operator_delete(void* p)
{
    __imp_free_01440120(p);
}