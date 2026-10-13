// CRetailVector_pod8::erase(_pod8*, _pod8*) -- move the tail down with the inlined SGI counted copy, set `last`, return the first iterator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B4B40, oracle length 59 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[f] ; push esi ; mov esi,[ecx+4] ; push edi ; mov edi,[l] ; sub esi,edi ; sar esi,3 ; test esi,esi ; mov edx,eax ; jle E ; sub edi,eax ; push ebx ; L: (copy 8 bytes from [edi+edx] to [edx]) ; add edx,0x8 ; dec esi ; jne L ; pop ebx ; E: pop edi ; mov [ecx+4],edx ; pop esi ; ret 8
// Retail vector erase for 8-byte trivially-copyable elements: `last = copy(l, last, f)` with the
// SGI counted `__copy` inlined (the compiler folds the source pointer into an offset from the
// destination), then the first iterator is returned. One of 9 bodies of this shape.
struct SElement8 {
    unsigned char _body[0x8];
};

struct CRetailVector_pod8 {
    SElement8* first;
    SElement8* last;
    SElement8* end;
    static SElement8* Copy(SElement8* f, SElement8* l, SElement8* result)
    {
        for (long n = l - f; n > 0; --n) {
            *result = *f;
            ++f;
            ++result;
        }
        return result;
    }
    SElement8* erase(SElement8* f, SElement8* l);
};

SElement8* CRetailVector_pod8::erase(SElement8* f, SElement8* l)
{
    SElement8* s = Copy(l, last, f);
    last = s;
    return f;
}
