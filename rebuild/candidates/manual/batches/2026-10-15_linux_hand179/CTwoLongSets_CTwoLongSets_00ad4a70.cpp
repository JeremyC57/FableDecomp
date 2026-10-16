// CTwoLongSets::~CTwoLongSets() -- destructor of a 0x1C-byte local record holding two STLport set<long> trees
// Leaf: ??1CTwoLongSets
// Retail 0x00AD4A70, 0x7B bytes (mesh-optimiser TU, 0x00AC/0x00AD block). NOT CWound::Kill (propagated label). It is the
// member-by-member destructor of a stack record used by the primitive optimiser (callers at 0x00AC96BA/0x00AC96C3
// destroy two of them at [esp+0x54]/[esp+0x70]); its constructor is 0x00AD49C0 (0x14-byte header node: colour +0,
// parent +4, left +8, right +0xC, value +0x10). Layout: one trivially destroyed dword +0x00, set A +0x04
// (header +0x04, count +0x08, empty compare +0x0C), set B +0x10 (header +0x10, count +0x14). Each STLport
// _Rb_tree destructor is clear() -- when the count is non-zero, _M_erase(root) (0x004CF810, manifest label
// LTextTreeWalkThrough::BuildTreeArray is a propagation), then leftmost = header, root = 0, rightmost = header,
// count = 0 -- followed by freeing the header node with free (0x00BFEA14) when non-null. Class name is a
// descriptive placeholder.
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71 import thunk)

struct CRbNode
{
    char     Color;                                    // +0x00
    CRbNode* Parent;                                   // +0x04
    CRbNode* Left;                                     // +0x08
    CRbNode* Right;                                    // +0x0C
    long     Value;                                    // +0x10
};

struct CLess {};

struct CLongSet
{
    CRbNode*     _M_header;                            // +0x00
    unsigned int _M_node_count;                        // +0x04
    CLess        _M_key_compare;                       // +0x08

    void _M_erase(CRbNode* x);                         // 0x004CF810
    void clear()
    {
        if (_M_node_count != 0)
        {
            _M_erase(_M_header->Parent);
            _M_header->Left = _M_header;
            _M_header->Parent = 0;
            _M_header->Right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CLongSet()
    {
        clear();
        if (_M_header != 0)
            free(_M_header);
    }
};

struct CTwoLongSets
{
    unsigned long Key;                                 // +0x00
    CLongSet      SetA;                                // +0x04
    CLongSet      SetB;                                // +0x10
    ~CTwoLongSets();
};

CTwoLongSets::~CTwoLongSets()
{
}
