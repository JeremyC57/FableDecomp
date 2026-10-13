// std::list<T> (node 0xc bytes)::Initialise() -- re-creates the sentinel node (no allocator argument, no `this` return)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDAE80, oracle length 28 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push 0xc ; mov dword ptr [esi],0 ; call malloc ; add esp,4 ; mov [eax],eax ; mov [eax+4],eax ; mov [esi],eax ; pop esi ; ret
// Same work as the retail list constructor (family 6) but as a plain member: no allocator
// argument (`ret`) and no `this` returned in eax. Nulls the head, allocates one node of 0xc
// bytes (malloc @ 0x00BFEA0E, landed-trusted) and links it to itself. The element type is not
// recoverable from the body. One of 7 bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailList_node0xc {
    struct node { node* next; node* prev; unsigned char value[0xc - 8]; };
    node* head;
    void Initialise();
};

void CRetailList_node0xc::Initialise()
{
    head = 0;
    node* sentinel = (node*)malloc(sizeof(node));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}
