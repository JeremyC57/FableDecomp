// std::list<T> (node 0x28 bytes)::list(const allocator&) -- retail sentinel-node list constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A17530, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push 0x28 ; mov dword ptr [esi],0 ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; mov [esi],eax ; add esp,4 ; mov eax,esi ; pop esi ; ret 4
// The retail std::list is a single sentinel-node pointer: the constructor (taking the allocator
// by value, hence `ret 4`) nulls it, allocates one node of 0x28 bytes (malloc @ 0x00BFEA0E,
// landed-trusted) and links it to itself. The element type is 32 bytes; it is not recoverable
// from the body, so the list is named by its node size. One of 166 bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator { };

struct CRetailList_node0x28 {
    struct node { node* next; node* prev; unsigned char value[0x28 - 8]; };
    node* head;
    CRetailList_node0x28(CRetailAllocator allocator);
};

CRetailList_node0x28::CRetailList_node0x28(CRetailAllocator allocator)
    : head(0)
{
    // The node is linked to itself BEFORE it is stored into `head` (retail stores [eax] and
    // [eax+4] first, then [esi]); assigning `head` first re-reads it between the stores.
    node* sentinel = (node*)malloc(sizeof(node));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    head = sentinel;
}
