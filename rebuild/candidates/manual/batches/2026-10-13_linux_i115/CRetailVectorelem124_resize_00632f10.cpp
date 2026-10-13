// CRetailVector_elem124::resize(unsigned long, const T&) -- Erase @ 0x00632F80 of the tail or Insert @ 0x00632FF0 of the missing elements (124-byte elements, signed size)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00632F10, oracle length 105 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[ecx+4] ; push esi ; push edi ; mov edi,[ecx] ; mov eax,ebx ; sub eax,edi ; push 0x7c ; cdq ; pop esi ; idiv esi ; mov esi,[n] ; cmp esi,eax ; jae G ; lea eax,[esi+esi*2] ; push ebx ; lea eax,[edi+eax*4] ; push eax ; call Erase ; jmp R ; G: mov ebx,[ecx+4] ; push [value] ; mov eax,ebx ; sub eax,edi ; push 0x7c ; pop edi ; cdq ; idiv edi ; sub esi,eax ; push esi ; push ebx ; call Insert ; R: pop edi ; pop esi ; pop ebx ; ret 8
// Retail vector resize for 124-byte elements: the size is a signed element count (`cdq / idiv`),
// `n` below it erases the tail through Erase(first + n, last) @ 0x00632F80, otherwise
// Insert(last, n - size, value) @ 0x00632FF0 appends the missing elements (the size is
// re-derived from the re-read `last`). The manifest label (`CArray<...>::push_back`) is
// propagated. One of 9 bodies of this shape.
struct SElement124 {
    unsigned char _body[0x7c];
};

struct CRetailVector_elem124 {
    SElement124* first;
    SElement124* last;
    SElement124* end;
    SElement124* Erase(SElement124* f, SElement124* l);   // 0x00632F80
    void Insert(SElement124* where, unsigned long count, const SElement124& value);   // 0x00632FF0
    long size() const { return last - first; }
    void resize(unsigned long n, const SElement124& value);
};

void CRetailVector_elem124::resize(unsigned long n, const SElement124& value)
{
    if (n < (unsigned long)size())
        Erase(first + n, last);
    else
        Insert(last, n - size(), value);
}
