// std::pair<const long, SMarkerSet>::pair(const pair&) -- copy constructor of a map value: the long key, then the record (its STLport tree copy-constructed, then its two trailing words)
// Leaf: pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004F9784, length 47 bytes (`ret 4`, returns this).
// Caller: the tree node builder 0x004F9763 (mallocs a 0x28-byte node and constructs the value at +0x10).
// Callees: the STLport _Rb_tree copy constructor @ 0x004F97B3 (allocates the 0x18-byte header, then copies
// the nodes; manifest "CUIBinaryTree::CopyConstructor").
// Label: the catalog name "CUIMiniMapMarker::CUIMiniMapMarker" is propagated; the body is the copy
// constructor of a map value pair<const long, record>, the record being 0x14 bytes: a 12-byte STLport
// tree (header, node count, empty comparator) and two words. The record's identity is not known, so it
// is named descriptively.
// Shape: the key is read and the source record's address formed (`lea esi,[eax+4]`) before the key store;
// the record's copy constructor is inlined (tree copy call, then the two words through esi/edi, both
// through eax). This is a size-optimised TU (`#pragma optimize("s", on)`: the speed form colours this /
// source / destination as esi/edi/ebx and copies through edx+eax), where the record's memberwise copy
// constructor must be `__forceinline` (an implicit or plain inline one is called out of line under /Os).
#pragma optimize("s", on)
struct SMarkerTree {                               // STLport _Rb_tree: 12 bytes
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    SMarkerTree(const SMarkerTree& other);         // 0x004F97B3
};

struct SMarkerSet {
    SMarkerTree m_tree;                            // +0x00
    long        m_first;                           // +0x0c
    long        m_second;                          // +0x10
    __forceinline SMarkerSet(const SMarkerSet& other) : m_tree(other.m_tree), m_first(other.m_first), m_second(other.m_second) { }
};

namespace std {

template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

template struct pair<const long, SMarkerSet>;

}
