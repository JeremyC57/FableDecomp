// CLookDrawRequest_007554c0::CLookDrawRequest_007554c0() -- the inline base constructor (type 8, flags, counts, the empty vector and the empty sentinel tree) then the derived type 10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007554C0, length 95 bytes (+ int3 padding).
// Callees: malloc @ 0x00BFEA0E (the 0x14-byte tree header node).
// Identity: built in a frame slot by CTCLook::OnAppearanceDraw (call at 0x0075436C, right after the
// caller zeroes its own locals); no vtable, no arguments. The manifest label
// "CTCBoastingPodium::CTCBoastingPodium" is propagated; the record is named by its user and its
// members descriptively.
// Shape: the first word is written twice (8, then 10 at the very end) = an inline base constructor
// that sets the type, then the derived constructor body that overrides it. The base zeroes its
// scalars with the ebx zero register in declaration order (the words at +0xc..+0x3b and +0x44..+0x4b
// have trivial constructors), constructs a three-pointer vector (+0x4c) and an STLport-shaped
// red-black tree (+0x58): the tree base nulls the header, allocates it (malloc 0x14), the tree sets its
// node count, then `_M_empty_initialize` colours the header red (0) and links root/leftmost/rightmost,
// re-reading the header member after every store through the node (the stores may alias it).
// The base constructor must be `__forceinline` (with the malloc call VC7.1 keeps it out of line and
// the derived constructor shrinks to `call ; mov [esi],0Ah`); the leaf names the derived constructor.
// Leaf: ??0CLookDrawRequest_007554c0
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct SRbTreeNode {                                        // 0x14 bytes
    unsigned char m_color;                                  // +0x00 (0 = red)
    SRbTreeNode*  m_pParent;                                // +0x04
    SRbTreeNode*  m_pLeft;                                  // +0x08
    SRbTreeNode*  m_pRight;                                 // +0x0c
    long          m_value;                                  // +0x10
};

struct CRbTreeBase {
    SRbTreeNode* m_pHeader;
    CRbTreeBase() : m_pHeader(0)
    {
        m_pHeader = (SRbTreeNode*)malloc(sizeof(SRbTreeNode));
    }
};

struct CRbTree : CRbTreeBase {
    unsigned long m_nodeCount;
    CRbTree() : m_nodeCount(0)
    {
        m_pHeader->m_color = 0;
        m_pHeader->m_pParent = 0;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
};

struct CRetailVector {                                      // first, last, end
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CLookDrawRequestBase {
    long          m_type;                                   // +0x00
    long          m_count;                                  // +0x04
    bool          m_flag08;                                 // +0x08
    bool          m_flag09;                                 // +0x09
    bool          m_flag0a;                                 // +0x0a
    unsigned char _pad0b[0x3c - 0x0b];                      // +0x0b .. +0x3b (trivially constructed)
    long          m_count3c;                                // +0x3c
    bool          m_flag40;                                 // +0x40
    bool          m_flag41;                                 // +0x41
    bool          m_flag42;                                 // +0x42
    unsigned char _pad43[0x4c - 0x43];                      // +0x43 .. +0x4b
    CRetailVector m_items;                                  // +0x4c
    CRbTree       m_tree;                                   // +0x58
    __forceinline CLookDrawRequestBase()
        : m_type(8), m_count(0), m_flag08(false), m_flag09(false), m_flag0a(false)
        , m_count3c(0), m_flag40(false), m_flag41(false), m_flag42(false)
    {
    }
};

struct CLookDrawRequest_007554c0 : CLookDrawRequestBase {
    CLookDrawRequest_007554c0();
};

CLookDrawRequest_007554c0::CLookDrawRequest_007554c0()
{
    m_type = 10;
}
