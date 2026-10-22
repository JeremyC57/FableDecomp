// CRetailString::_M_deallocate_block() -- releases the string's heap block with the CRT `free` when the start pointer is set
// Leaf: _M_deallocate_block
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567E90, 14 bytes. Callees: free (MSVCR71 import thunk 0x00BFEA14).
// Unreferenced in the image (recovered from a mis-cut catalog row). The neighbouring string range append
// 0x00567400 releases its old block with the same `test eax,eax ; je ; push eax ; call free` sequence, so
// this is that string's block release (start pointer at +0).
// Shape: `if (_M_start) free(_M_start);` -- the call result is dropped (`pop ecx`).
extern "C" void __cdecl free(void* block);

struct CRetailString {
    char* _M_start;                   // +0
    char* _M_finish;                  // +4
    char* _M_end_of_storage;          // +8
    void _M_deallocate_block();
};

void CRetailString::_M_deallocate_block()
{
    if (_M_start)
        free(_M_start);
}
