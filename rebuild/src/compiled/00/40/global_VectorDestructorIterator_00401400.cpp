// _global::VectorDestructorIterator(void* ptr, unsigned int size, int count, void (*pDtor)(void*)) -- `vector destructor iterator': destroy `count` array elements of `size` bytes, last to first
// Leaf: VectorDestructorIterator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00401400, 49 bytes. No direct callees (the element
// destructor is called through the pointer in ebp).
//
// Retail bytes:  mov eax,[count] ; push ebx ; mov ebx,[size] ; mov ecx,ebx ; imul ecx,eax ; push esi ;
//   mov esi,[ptr] ; add esi,ecx ; dec eax ; js X ; push ebp ; mov ebp,[pDtor] ; push edi ;
//   lea edi,[eax+1] ; L: sub esi,ebx ; mov ecx,esi ; call ebp ; dec edi ; jne L ; pop edi ; pop ebp ;
//   X: pop esi ; pop ebx ; ret 0x10
// The manifest label "`vector constructor iterator'" is wrong: the walk starts one past the end
// (ptr + size*count) and steps BACK before every call, i.e. the reverse-order compiler helper
// `vector destructor iterator' (??_I, the non-EH twin of the CRT's __ehvec_dtor): __stdcall with
// four arguments (`ret 0x10`), the element destructor a __fastcall pointer (`mov ecx,esi ; call
// ebp`; modelled __fastcall, which passes the one argument in ecx the same way). The compiler
// rewrites `while (--count >= 0)` into a `dec ; js` guard plus an edi down-counter.
typedef void (__fastcall *PFNElementDestructor)(void* element);   // __fastcall in retail

void __stdcall VectorDestructorIterator(void* ptr, unsigned int size, int count, PFNElementDestructor pDtor)
{
    ptr = (char*)ptr + size * count;
    while (--count >= 0)
    {
        ptr = (char*)ptr - size;
        (*pDtor)(ptr);
    }
}