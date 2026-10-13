// std::map/set (node 0x14 bytes) constructor -- retail red-black tree head node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2D050, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push 0x14 ; mov dword ptr [esi],0 ; call malloc ; mov [esi],eax ; mov dword ptr [esi+4],0 ; mov byte ptr [eax],0 ; (re-read head) mov [eax+4],0 ; mov [eax+8],eax ; mov [eax+0Ch],eax ; mov eax,esi ; pop esi ; ret
// The retail map/set is {head, size}: the constructor nulls the head, allocates the 0x14-byte head
// node (malloc @ 0x00BFEA0E, landed-trusted), zeroes the size, clears the node's colour byte and
// parent, and points left and right at the node itself. `head` is re-read from the object between
// the stores because each store goes through it. One of 109 bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator { };

struct CRetailTree_node0x14 {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x14 - 16]; };
    node*         head;
    unsigned long size;
    CRetailTree_node0x14();
};

CRetailTree_node0x14::CRetailTree_node0x14()
    : head(0)
{
    head = (node*)malloc(sizeof(node));
    size = 0;
    head->colour = 0;
    head->parent = 0;
    head->left   = head;
    head->right  = head;
}