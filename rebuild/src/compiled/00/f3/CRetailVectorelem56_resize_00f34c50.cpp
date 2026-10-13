// CRetailVector_elem56::resize(unsigned long, const T&) -- Erase @ 0x00F34CC0 of the tail or Insert @ 0x00F34D70 of the missing elements (56-byte elements, signed size)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F34C50, oracle length 105 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[ecx+4] ; push esi ; push edi ; mov edi,[ecx] ; mov eax,ebx ; sub eax,edi ; push 0x38 ; cdq ; pop esi ; idiv esi ; mov esi,[n] ; cmp esi,eax ; jae G ; lea eax,[esi+esi*2] ; push ebx ; lea eax,[edi+eax*4] ; push eax ; call Erase ; jmp R ; G: mov ebx,[ecx+4] ; push [value] ; mov eax,ebx ; sub eax,edi ; push 0x38 ; pop edi ; cdq ; idiv edi ; sub esi,eax ; push esi ; push ebx ; call Insert ; R: pop edi ; pop esi ; pop ebx ; ret 8
// Retail vector resize for 56-byte elements: the size is a signed element count (`cdq / idiv`),
// `n` below it erases the tail through Erase(first + n, last) @ 0x00F34CC0, otherwise
// Insert(last, n - size, value) @ 0x00F34D70 appends the missing elements (the size is
// re-derived from the re-read `last`). The manifest label (`CArray<...>::push_back`) is
// propagated. One of 9 bodies of this shape.
struct SElement56 {
    unsigned char _body[0x38];
};

struct CRetailVector_elem56 {
    SElement56* first;
    SElement56* last;
    SElement56* end;
    SElement56* Erase(SElement56* f, SElement56* l);   // 0x00F34CC0
    void Insert(SElement56* where, unsigned long count, const SElement56& value);   // 0x00F34D70
    long size() const { return last - first; }
    void resize(unsigned long n, const SElement56& value);
};

void CRetailVector_elem56::resize(unsigned long n, const SElement56& value)
{
    if (n < (unsigned long)size())
        Erase(first + n, last);
    else
        Insert(last, n - size(), value);
}