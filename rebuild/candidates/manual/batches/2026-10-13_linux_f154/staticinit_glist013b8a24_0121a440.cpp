// dynamic initializer for CRetailList_node0x14 g_list_013b8a24 @ 0x013B8A24 -- allocates and self-links the sentinel node, registers the destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A440, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x14 ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; push offset thunk ; mov [0x13b8a24],eax ; call _atexit ; add esp,8 ; ret
// Compiler-generated `??__E` initializer of one static-storage retail std::list (sentinel node of
// 0x14 bytes, malloc @ 0x00BFEA0E) whose inline default constructor allocates the node and links it
// to itself, then registers the destructor thunk @ 0x01228CE0 with _atexit @ 0x004012BC. The
// object's identifier is not recoverable; it is named by its address. One of 9 bodies.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailList_node0x14 {
    struct node { node* next; node* prev; unsigned char value[0x14 - 8]; };
    node* head;
    CRetailList_node0x14() { node* sentinel = (node*)malloc(sizeof(node)); sentinel->next = sentinel; sentinel->prev = sentinel; head = sentinel; }
    ~CRetailList_node0x14();
};

CRetailList_node0x14 g_list_013b8a24;
