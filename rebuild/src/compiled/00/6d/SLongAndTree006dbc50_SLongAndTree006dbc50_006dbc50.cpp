// SLongAndTree_006dbc50::SLongAndTree_006dbc50() -- default constructor of a {long, STLport tree} pair: zero the long, build the tree's empty head
// Leaf: ??0SLongAndTree_006dbc50@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DBC50, length 65 bytes (+ int3 padding), plain `ret`, no callers or
// absolute references (an unreferenced COMDAT; 0x006DBC00 is a byte-identical twin). The manifest label
// "StdList_CtorHeadAtField1" is descriptive and half wrong: the self-linked 0x18-byte head with a colour byte, parent
// cleared and left/right pointing at itself is the STLport `_Rb_tree` empty-initialise (not a list), at +4 with the
// node count at +8 -- the same body as the landed tree constructors 0x006DB230/0x006DB770, behind one extra zeroed
// dword at +0. That is a value-initialised `long` member ahead of a tree member (STLport `pair<const long, set/map>()`
// shape: `first(T1()), second(T2())`); the key/value types are not recoverable (node value 8 bytes), so the name is
// descriptive.
// Callees: malloc @ 0x00BFEA0E (STLport node allocator).
// Shape: member-init `first(0)` then the tree constructor inlined: `head(0)` before the allocation (retail zeroes
// +4 between the push and the call), then head = malloc(0x18), size = 0, colour/parent cleared, left/right self-linked.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct SRetailTree_node0x18 {
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x18 - 16]; };
    node*         head;                       // +0x00 (the alloc-proxy header pointer)
    unsigned long size;                       // +0x04 (node count)
    SRetailTree_node0x18()
        : head(0)
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left   = head;
        head->right  = head;
    }
};

struct SLongAndTree_006dbc50 {
    long                 first;               // +0x00
    SRetailTree_node0x18 second;              // +0x04
    SLongAndTree_006dbc50();
};

SLongAndTree_006dbc50::SLongAndTree_006dbc50()
    : first(0)
{
}