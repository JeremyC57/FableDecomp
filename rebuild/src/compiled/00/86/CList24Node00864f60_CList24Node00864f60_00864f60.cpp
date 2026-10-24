// CList24Node00864f60::CList24Node00864f60() -- STLport list<T> default constructor for a 0x1C-byte element: zero head, malloc the sentinel, self-link it
// Leaf: ??0CList24Node00864f60
// Retail 0x00864F60, 0x1E bytes (+ int3 padding). Manifest label "LinkedList_Init" (_global, unknown convention) is a
// propagated descriptive name; no caller references the out-of-line copy (xref 0), so the element type is unknown and the
// class name is descriptive (address-tagged). Callee: malloc @ 0x00BFEA0E (STLport __malloc_alloc node allocator).
// Shape: retail STLport list is a single 4-byte header-node pointer. The _List_base constructor zeroes the alloc proxy
// (`mov [esi],0` before the call), allocates one 0x24-byte node (next +0, prev +4, 0x1C-byte value +8), links the
// sentinel to itself through the returned register and only then stores it in the member (a local node pointer: an
// assignment to the member first would store [esi] before the self-links). Returns `this` (constructor).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct SList24Node
{
    SList24Node*  _M_next;                           // +0x00
    SList24Node*  _M_prev;                           // +0x04
    unsigned char _M_data[0x1C];                     // +0x08 element (type unknown, 0x1C bytes)
};

struct CList24Node00864f60                            // std::list<T> with sizeof(T) == 0x1C
{
    SList24Node* _M_node;                            // +0x00 header (sentinel) node
    CList24Node00864f60();
};

CList24Node00864f60::CList24Node00864f60() : _M_node(0)
{
    SList24Node* node = (SList24Node*)malloc(sizeof(SList24Node));
    node->_M_next = node;
    node->_M_prev = node;
    _M_node = node;
}