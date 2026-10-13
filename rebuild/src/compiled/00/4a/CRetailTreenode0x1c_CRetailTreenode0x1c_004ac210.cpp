// std::map/set (node 0x1c bytes) constructor -- retail red-black tree head node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AC210, oracle length 56 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push 0x1c ; mov dword ptr [esi],0 ; call malloc ; mov [esi],eax ; mov dword ptr [esi+4],0 ; mov byte ptr [eax],0 ; (re-read head) mov [eax+4],0 ; mov [eax+8],eax ; mov [eax+0Ch],eax ; mov eax,esi ; pop esi ; ret 8
// The retail map/set is {head, size}: the constructor nulls the head, allocates the 0x1c-byte head
// node (malloc @ 0x00BFEA0E, landed-trusted), zeroes the size, clears the node's colour byte and
// parent, and points left and right at the node itself. `head` is re-read from the object between
// the stores because each store goes through it. One of 81 bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator { };    // empty by-value allocator argument (one stack slot each, hence `ret N`)
struct CRetailCompare { };      // empty by-value comparator argument

struct CRetailTree_node0x1c {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x1c - 16]; };
    node*         head;
    unsigned long size;
    CRetailTree_node0x1c(CRetailCompare compare, CRetailAllocator allocator);
};

CRetailTree_node0x1c::CRetailTree_node0x1c(CRetailCompare compare, CRetailAllocator allocator)
    : head(0)
{
    head = (node*)malloc(sizeof(node));
    size = 0;
    head->colour = 0;
    head->parent = 0;
    head->left   = head;
    head->right  = head;
}