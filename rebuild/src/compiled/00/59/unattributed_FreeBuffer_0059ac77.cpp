// CPointerArrayOwner_0059ac77::FreeBuffer() -- free the owned pointer elements, then the block itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059AC77, oracle length 40 bytes.
// Callees: the element freeing loop @ 0x0059AA64 (`__stdcall`-shaped: ret 8, frees every non-null pointer in [first, last)),
//          free @ 0x00BFEA14 (__cdecl, `pop ecx`).  Size-optimised translation unit (`#pragma optimize("s", on)`).
//
// Retail bytes:  push esi ; mov esi,ecx ; cmp dword ptr [esi+20h],0 ; je E ; mov eax,[esi+1Ch] ; add eax,4 ; push eax ; push dword ptr [esi+0Ch] ; call FreeElements ;
//                mov esi,[esi+20h] ; test esi,esi ; je E ; push esi ; call free ; pop ecx ; E: pop esi ; ret
// The same shape as the landed CBankFileAsyncData::FreeBuffer bodies (0x0040F470 ..) built for size:
// the argument is pushed straight from memory and the free's argument popped into ecx.  The
// element range is [m_pElements, m_pElementsEnd + 4) -- one past the stored end pointer.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                                  // 0x00BFEA14
void __stdcall FreeElements(void** first, void** last);                      // 0x0059AA64

struct CPointerArrayOwner_0059ac77 {
    unsigned char _pad[0x0c];
    void** m_pElements;        // +0x0c
    unsigned char _pad2[0x0c];
    void** m_pElementsEnd;     // +0x1c
    void*  m_pBlock;           // +0x20
    void FreeBuffer();         // 0x0059AC77
};

void CPointerArrayOwner_0059ac77::FreeBuffer()
{
    if (m_pBlock != 0) {
        FreeElements(m_pElements, m_pElementsEnd + 1);
        if (m_pBlock != 0)
            free(m_pBlock);
    }
}