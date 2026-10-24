// std::_Construct(CNavQuadTree::CNavigationLayer*, const CNavQuadTree::CNavigationLayer&) -- placement copy of a nav quad-tree layer: node array, line lists vector and the switchable-line-list vector map (vector base + Dirty flag)
// Retail: Fable.exe @ 0x00A85360, 51 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: vector copy ctors @ 0x00A80B80, 0x00A83170, 0x00A83A30.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00a85360 {
struct CArrayNodes {                                 // retail 12-byte STLport-vector body (void*)
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    CArrayNodes(const CArrayNodes& other);                  // 0x00A80B80
};
struct CLineListVector {                                 // retail 12-byte STLport-vector body (void)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CLineListVector(const CLineListVector& other);                  // 0x00A83170
};
struct CSwitchableLineVector {                                 // retail 12-byte STLport-vector body (void)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CSwitchableLineVector(const CSwitchableLineVector& other);                  // 0x00A83A30
};
struct CKeyPairCompareLess { };
struct CSwitchableLineMap : CSwitchableLineVector {   // CVectorMap<unsigned __int64, list<C2DLineF>, ...>: vector base, Compare +0xC, Dirty +0xD
    CKeyPairCompareLess Compare;                 // +0x0C (empty)
    bool                Dirty;                   // +0x0D
};
struct CNavigationLayer {                        // CNavQuadTree::CNavigationLayer (retail sizeof 0x28)
    CArrayNodes        Nodes;                    // +0x00 CArray<CANavQuadTreeNode*>
    CLineListVector    LineLists;                // +0x0C vector<list<C2DLineF>>
    CSwitchableLineMap SwitchableLineLists;      // +0x18
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CNavigationLayer* p, const CNavigationLayer& value)
{
    if (p != 0)
        new (p) CNavigationLayer(value);
}
}