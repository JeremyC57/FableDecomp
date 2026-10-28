// _Rb_tree::_Rb_tree(const _Rb_tree& other) -- STLport red-black tree copy constructor (a map with 8-byte values): allocate the header node, then either mark the tree empty or clone the other tree's nodes and set leftmost/rightmost, finally take the node count
// Leaf: ??0_Rb_tree@NPairLongMapTree@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F97B3, length 126 bytes (ret 4). Callers: 0x004F9955 (a node
// constructor copying a pair<long, map> whose map sits at +4) and eight sites in 0x006DBCC8..0x006DD2E0.
// Callees: malloc @ 0x00BFEA0E (the 0x18-byte header node, `__cdecl`, `pop ecx`), _Rb_tree::_M_copy(x, p) @ 0x00461533
// (manifest "Tree_CloneRecursive"; `__fastcall`, ret 8).
// Label: the manifest's "CBinaryTree_Alt::CopyConstructor" is a placeholder; the shape is STLport's
//   _Rb_tree(const _Rb_tree& __x) : _Rb_tree_base(__x.get_allocator()), _M_node_count(0) {
//       if (__x._M_root() == 0) { color(header) = red; root = 0; leftmost = rightmost = header; }
//       else { color(header) = red; root = _M_copy(__x._M_root(), header);
//              leftmost = _S_minimum(root); rightmost = _S_maximum(root); }
//       _M_node_count = __x._M_node_count; }
// Node (STLport _Rb_tree_node, 0x18 bytes): color byte +0, parent +4, left +8, right +0xc, the 8-byte value +0x10.
// Shape: a size-optimised TU (`pop ecx` after malloc, the rotated `jmp ; mov eax,edx` minimum/maximum walks):
// `#pragma optimize("s", on)` with the base constructor `__forceinline` (else it stays out of line); the base
// constructor zeroes the header word before the allocation and stores the node after it; the root/leftmost/rightmost
// accessors return REFERENCES, so `_M_root() = _M_copy(...)` takes the header into edi before the arguments; the red
// colour store (0) is common to both arms and hoisted above the branch; one zero register (ebx) serves every null.
#pragma optimize("s", on)

// Identity: a second instantiation of the tree (its _M_copy is 0x00461533, not 0x004F99DE as for the landed
// 0x004F9960 copy ctor), so it lives in its own namespace and its symbols bind to one retail address each.
namespace NPairLongMapTree {

struct SRbNode {
    unsigned char Color;                      // +0x00 (0 = red)
    SRbNode*      Parent;                     // +0x04
    SRbNode*      Left;                       // +0x08
    SRbNode*      Right;                      // +0x0c
    long          Key;                        // +0x10
    long          Value;                      // +0x14
};
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct _Rb_tree_base {
    SRbNode* Header;                          // +0x00 (_M_header._M_data)
    __forceinline _Rb_tree_base() : Header(0) { Header = (SRbNode*)malloc(sizeof(SRbNode)); }
};

class _Rb_tree : public _Rb_tree_base {
public:
    unsigned long NodeCount;                  // +0x04
    _Rb_tree(const _Rb_tree& other);
private:
    SRbNode* _M_copy(SRbNode* x, SRbNode* p); // 0x00461533 (this tree type's _M_copy)
public:
    static SRbNode* _S_minimum(SRbNode* x) { while (x->Left != 0) x = x->Left; return x; }
    static SRbNode* _S_maximum(SRbNode* x) { while (x->Right != 0) x = x->Right; return x; }
    protected:
    SRbNode*& _M_root() const { return Header->Parent; }
    public:
    protected:
    SRbNode*& _M_leftmost() const { return Header->Left; }
    public:
    protected:
    SRbNode*& _M_rightmost() const { return Header->Right; }
    public:
    static unsigned char& _S_color(SRbNode* x) { return x->Color; }
};

_Rb_tree::_Rb_tree(const _Rb_tree& other)
    : _Rb_tree_base(), NodeCount(0)
{
    if (other._M_root() == 0) {
        _S_color(Header) = 0;
        _M_root() = 0;
        _M_leftmost() = Header;
        _M_rightmost() = Header;
    } else {
        _S_color(Header) = 0;
        _M_root() = _M_copy(other._M_root(), Header);
        _M_leftmost() = _S_minimum(_M_root());
        _M_rightmost() = _S_maximum(_M_root());
    }
    NodeCount = other.NodeCount;
}

} // namespace NPairLongMapTree
