// dynamic initializer for CRetailTree_node0x18 g_tree_013bae38 @ 0x013BAE38 -- builds the red-black head node inline, registers the destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F120, oracle length 60 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x18 ; call malloc ; mov [0x13bae38],eax ; xor ecx,ecx ; mov [0x13bae3c],ecx ; mov byte ptr [eax],cl ; (re-read head) mov [eax+4],ecx ; mov [eax+8],eax ; mov [eax+0Ch],eax ; push offset thunk ; call _atexit ; add esp,8 ; ret
// Compiler-generated `??__E` initializer of one static-storage retail std::map/set: the inline
// default constructor allocates the 0x18-byte head node (malloc @ 0x00BFEA0E), zeroes the size,
// clears the node's colour byte and parent and points left/right at the node itself (head is
// re-read through the global between the stores), then the destructor thunk @ 0x01229C40 is
// registered with _atexit @ 0x004012BC. Named by its address. One of 15 bodies.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailTree_node0x18 {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x18 - 16]; };
    node* head;
    unsigned long size;
    CRetailTree_node0x18()
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left = head;
        head->right = head;
    }
    ~CRetailTree_node0x18();
};

CRetailTree_node0x18 g_tree_013bae38;
