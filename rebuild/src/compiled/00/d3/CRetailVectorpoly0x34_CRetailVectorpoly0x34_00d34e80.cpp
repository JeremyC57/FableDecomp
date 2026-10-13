// CRetailVector_poly0x34::~CRetailVector_poly0x34() -- destroys the polymorphic 0x34-byte elements through vtable slot 0, frees the buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D34E80, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,ecx ; mov esi,[ebx] ; push edi ; mov edi,[ebx+4] ; cmp esi,edi ; je F ; L: mov eax,[esi] ; push 0 ; mov ecx,esi ; call [eax] ; add esi,0x34 ; cmp esi,edi ; jne L ; F: mov ebx,[ebx] ; test ebx,ebx ; je R ; push ebx ; call free ; add esp,4 ; R: pop edi ; pop esi ; pop ebx ; ret
// Destructor of a retail std::vector {first, last, end} whose elements are polymorphic: each
// element's virtual destructor (vtable slot 0, the deleting destructor called with flag 0 so the
// storage is not freed) runs over [first, last), then the buffer is released with free
// @ 0x00BFEA14 (null-checked). The element type is only known by its size and virtual
// destructor. One of 19 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct CPolyElement {
    virtual ~CPolyElement();
    unsigned char _body[0x34 - 4];
};

struct CRetailVector_poly0x34 {
    CPolyElement* first;
    CPolyElement* last;
    CPolyElement* end;
    ~CRetailVector_poly0x34();
};

CRetailVector_poly0x34::~CRetailVector_poly0x34()
{
    CPolyElement* p = first;
    CPolyElement* stop = last;             // named local: retail keeps `last` in a register across the virtual calls
    for (; p != stop; ++p)
        p->~CPolyElement();
    if (first)
        free(first);
}