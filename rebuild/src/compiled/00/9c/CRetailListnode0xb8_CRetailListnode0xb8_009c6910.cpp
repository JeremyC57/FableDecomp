// std::list<T> (node 0xb8 bytes)::list(const allocator&) -- retail sentinel-node list constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C6910, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push 0xb8 ; mov dword ptr [esi],0 ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; mov [esi],eax ; add esp,4 ; mov eax,esi ; pop esi ; ret 4
// The retail std::list is a single sentinel-node pointer: the constructor (taking the allocator
// by value, hence `ret 4`) nulls it, allocates one node of 0xb8 bytes (malloc @ 0x00BFEA0E,
// landed-trusted) and links it to itself. The element type is 176 bytes; it is not recoverable
// from the body, so the list is named by its node size. One of 166 bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator { };

struct CRetailList_node0xb8 {
    struct node { node* next; node* prev; unsigned char value[0xb8 - 8]; };
    node* head;
    CRetailList_node0xb8(CRetailAllocator allocator);
};

CRetailList_node0xb8::CRetailList_node0xb8(CRetailAllocator allocator)
    : head(0)
{
    // The node is linked to itself BEFORE it is stored into `head` (retail stores [eax] and
    // [eax+4] first, then [esi]); assigning `head` first re-reads it between the stores.
    node* sentinel = (node*)malloc(sizeof(node));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}