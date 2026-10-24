// GFDataIsInsideBlock<C4DVector*>(C4DVector*& pointer, void* block, long size) -- true when a (relocatable) data pointer is non-null and lies inside the memory block [block, block + size)
// Leaf: ??$GFDataIsInsideBlock@PAVC4DVector@@@@YI_NAAPAVC4DVector@@PAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFB750, 38 bytes (+ int3 padding), `__fastcall` (pointer reference in ecx,
// block in edx, size on the stack, `ret 4`). Ego_r.pdb name set: ??$GFDataIsInsideBlock@PAVC4DVector@@@@YI_NAAPAVC4DVector@@PAXJ@Z
// (the free template of the movable-resource relocation code; sibling instantiations for unsigned char*, float*, CObjectEntry*, ...).
// Shape: the null test returns a literal `false` (8-bit `xor al,al`); the range test is one `&&` expression whose bool
// VC7.1 materialises in the full register (`mov eax,1` / `xor eax,eax`). Both compares are unsigned pointer compares
// (`jb` / `jae`), the end of the block formed as `block + size` (`add edx,ecx`).
class C4DVector;

template <class T>
bool __fastcall GFDataIsInsideBlock(T& pointer, void* block, long size)
{
    if (pointer == 0)
        return false;
    return (void*)pointer >= block && (void*)pointer < (void*)((char*)block + size);
}

template bool __fastcall GFDataIsInsideBlock<C4DVector*>(C4DVector*&, void*, long);