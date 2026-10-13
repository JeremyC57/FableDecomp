// CRetailVector_elem60::resize(unsigned long, const T&) -- Erase @ 0x00F28170 of the tail or Insert @ 0x00F281E0 of the missing elements (60-byte elements, signed size)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F28100, oracle length 105 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[ecx+4] ; push esi ; push edi ; mov edi,[ecx] ; mov eax,ebx ; sub eax,edi ; push 0x3c ; cdq ; pop esi ; idiv esi ; mov esi,[n] ; cmp esi,eax ; jae G ; lea eax,[esi+esi*2] ; push ebx ; lea eax,[edi+eax*4] ; push eax ; call Erase ; jmp R ; G: mov ebx,[ecx+4] ; push [value] ; mov eax,ebx ; sub eax,edi ; push 0x3c ; pop edi ; cdq ; idiv edi ; sub esi,eax ; push esi ; push ebx ; call Insert ; R: pop edi ; pop esi ; pop ebx ; ret 8
// Retail vector resize for 60-byte elements: the size is a signed element count (`cdq / idiv`),
// `n` below it erases the tail through Erase(first + n, last) @ 0x00F28170, otherwise
// Insert(last, n - size, value) @ 0x00F281E0 appends the missing elements (the size is
// re-derived from the re-read `last`). The manifest label (`CArray<...>::push_back`) is
// propagated. One of 9 bodies of this shape.
struct SElement60 {
    unsigned char _body[0x3c];
};

struct CRetailVector_elem60 {
    SElement60* first;
    SElement60* last;
    SElement60* end;
    SElement60* Erase(SElement60* f, SElement60* l);   // 0x00F28170
    void Insert(SElement60* where, unsigned long count, const SElement60& value);   // 0x00F281E0
    long size() const { return last - first; }
    void resize(unsigned long n, const SElement60& value);
};

void CRetailVector_elem60::resize(unsigned long n, const SElement60& value)
{
    if (n < (unsigned long)size())
        Erase(first + n, last);
    else
        Insert(last, n - size(), value);
}