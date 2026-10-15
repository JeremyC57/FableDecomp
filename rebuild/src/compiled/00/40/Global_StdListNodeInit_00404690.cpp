// std_list_node_Init(void** nodeRef) -- STL list construction: allocate the sentinel node and link it to itself (empty list)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404690, 32 bytes. Callees: malloc 0x00BFEA0E.
//
// Every std::list we use starts life here. The list object is a single word pointing at a 16-byte
// sentinel node; a node is next, prev and 8 bytes of payload. We clear the word first (so a failed
// allocation leaves an obviously empty list rather than garbage), allocate the sentinel, point both of
// its links at itself, and store it. "Empty" is then simply "next == sentinel". The two trailing
// parameters are the allocator and the ignored value argument of the STLport constructor and are not
// used. Returns the list object so the caller can chain.
//
// Matching: the allocation size is the literal 0x10; the self-links are two stores through the fresh
// node; the unused parameters keep the __fastcall register assignment of the real constructor.
#include <stdlib.h>

// One list node: links first, then the element storage.
struct StdListNodeInitNode
{
    StdListNodeInitNode* next;
    StdListNodeInitNode* previous;
    unsigned char reserved[8];   // element payload (unused in the sentinel)
};

void** __fastcall std_list_node_Init(void** nodeRef, void*, void*)
{
    *nodeRef = 0;
    StdListNodeInitNode* node =
        static_cast<StdListNodeInitNode*>(malloc(0x10));
    // A sentinel that points at itself is the empty list.
    node->next = node;
    node->previous = node;
    *nodeRef = node;
    return nodeRef;
}
