// CLODStateBlock::~CLODStateBlock() -- release the counted bounding-box range, then destroy the bone-LOD map (clear the tree, free its header)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2CFE0, length 100 bytes (padded to 112).
// Leaf: ??1CLODStateBlock
// Label correction: the catalog "_Dest_val<..., std::pair<unsigned long, CCountedPointer<CFaction>>>" is propagated.
// The only caller is ~CMeshBankStateBlock @ 0x00A2D710 (slot 0 of vtable 0x0129CE7C is its deleting destructor
// 0x00A2D770), which runs this on its member at +0x14 between the set<CCharString> at +0x28 and the CBankStateBlock base.
// Ego_r.pdb CMeshBankStateBlock: State +4, LODState (CLODStateBlock) +0x14, AnimationState +0x2c (PDB offsets of the
// 12-byte retail trees: set at +0x28); CLODStateBlock = BoneLODs (map<CCharString, long>) +0, MinXYZRange
// (CCountedPointer<C3DBoundingBox>) +0x10 in the PDB, +0x0c with the 12-byte retail map.
// Callees: map<CCharString, long>::_M_erase(node) @ 0x0043020A, the count block's destroy (+4, object at +8 in ecx),
//          operator delete @ 0x00BFE9BC (count block), free @ 0x00BFEA14 (tree header).
// Shape: an empty user destructor out of line (the owner calls it); members die in reverse order: the counted pointer
// (inline release, both words nulled), then the STLport tree (inline clear(): erase from the root, leftmost / root /
// rightmost reset, count zeroed; then the header freed). One zero register (edi) serves every test and store.
extern "C" void __cdecl free(void* p);      // 0x00BFEA14
void __cdecl operator delete(void* block);  // 0x00BFE9BC
struct CCPPointerInfo {
    long  RefCount;                         // +0x00
    void  (__fastcall* DeleteFunc)(void* data);   // +0x04
    void* Data;                             // +0x08
};
template <class T> struct CCountedPointer {
    T*              Data;                   // +0x00
    CCPPointerInfo* Info;                   // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct _Rb_tree_node_base {
    char _M_color;                          // +0x00
    _Rb_tree_node_base* _M_parent;          // +0x04
    _Rb_tree_node_base* _M_left;            // +0x08
    _Rb_tree_node_base* _M_right;           // +0x0c
};
struct less_CCharString { };
struct CBoneLODMap {                        // retail STLport map<CCharString, long> (12 bytes)
    _Rb_tree_node_base* _M_header;          // +0x00
    unsigned long       _M_node_count;      // +0x04
    less_CCharString    _M_key_compare;     // +0x08
    void _M_erase(_Rb_tree_node_base* x);   // 0x0043020A
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CBoneLODMap()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
};
struct C3DBoundingBox;
struct CLODStateBlock {
    CBoneLODMap                     BoneLODs;      // +0x00
    CCountedPointer<C3DBoundingBox> MinXYZRange;   // +0x0c
    ~CLODStateBlock();
};

CLODStateBlock::~CLODStateBlock()
{
}