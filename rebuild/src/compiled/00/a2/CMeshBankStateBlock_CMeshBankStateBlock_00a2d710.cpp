// CMeshBankStateBlock::~CMeshBankStateBlock() -- clear and free the excluded-bone set (AnimationState), destroy the LOD state block, then the inlined empty CBankStateBlock destructor's vtable store
// Leaf: ??1CMeshBankStateBlock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2D710, length 85 bytes (+ int3 padding).
// Callees: the STLport red-black tree _M_erase(node*) @ 0x004ABA70 (labelled "LTextTreeWalkThrough::BuildTreeArray",
// propagated), free @ 0x00BFEA14 (the header node), CLODStateBlock::~CLODStateBlock @ 0x00A2CFE0 (releases the
// CCountedPointer<C3DBoundingBox> and the bone map; labelled "_Dest_val<...>", propagated). Vtable 0x0129C82C =
// CBankStateBlock. Evidence: the callers are the scalar deleting destructor @ 0x00A2D770 (slot 0 of CMeshBankStateBlock's
// vtable 0x0129CE7C, rtti-vtables.tsv) and 0x00A27371. The catalog label "~CPatchTesselationEdgeStrip" is propagated.
// Layout: Ego_r.pdb CMeshBankStateBlock with 12-byte retail maps/sets: State (the 16-byte mesh CStateBlock) +0x04,
// LODState (CLODStateBlock, retail 0x14 bytes) +0x14, AnimationState (CAnimationStateBlock: set<CCharString>
// ExcludedBones = header +0x28, count +0x2c, comparator +0x30) +0x28; retail size 0x34. No engine header.
// Shape: members destroyed in reverse order; the inlined STLport `clear()` tests the count first, erases from the root
// (`header->parent`) and resets leftmost / root / rightmost / count with the header re-read for every store (node
// pointers alias it); the header node is freed with a null test; the derived class is `__declspec(novtable)`, so only
// the empty base destructor's vtable store remains, as the last instruction before `ret`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct SRbNode {                                   // STLport _Rb_tree_node_base (+ the value)
    long     m_color;                              // +0x00
    SRbNode* m_pParent;                            // +0x04
    SRbNode* m_pLeft;                              // +0x08
    SRbNode* m_pRight;                             // +0x0c
};

struct CRetailStringSet {                          // STLport set<CCharString>: header, count, comparator
    SRbNode*      m_pHeader;                       // +0x00
    unsigned long m_count;                         // +0x04
    unsigned long m_compare;                       // +0x08 (empty less<>, padded)
    void Erase(SRbNode* node);                     // 0x004ABA70 (_M_erase)
    void clear()
    {
        if (m_count != 0) {
            Erase(m_pHeader->m_pParent);
            m_pHeader->m_pLeft = m_pHeader;
            m_pHeader->m_pParent = 0;
            m_pHeader->m_pRight = m_pHeader;
            m_count = 0;
        }
    }
    ~CRetailStringSet()
    {
        clear();
        if (m_pHeader)
            free(m_pHeader);
    }
};

struct CMeshStateBlock {                           // CMeshBankStateBlock's CStateBlock: 16 bytes of plain data
    bool EnableLOD;
    bool PreserveMaterials;
    bool GenericAnimation;
    bool UnpackedPositions;
    long MaxLODs;
    long ResourcePhysicsMeshIndex;
    long AnimationVersion;
};
struct CLODStateBlock {                            // retail 0x14 bytes: map<CCharString, long> BoneLODs + CCountedPointer<C3DBoundingBox>
    unsigned char _body[0x14];
    ~CLODStateBlock();                             // 0x00A2CFE0
};
struct CAnimationStateBlock {
    CRetailStringSet ExcludedBones;                // +0x00
};

class CBankStateBlock { public:                           // vtable 0x0129C82C
    virtual void v00();
    ~CBankStateBlock() { }
};

struct __declspec(novtable) CMeshBankStateBlock : CBankStateBlock {   // vtable 0x0129CE7C
    CMeshStateBlock      State;                    // +0x04
    CLODStateBlock       LODState;                 // +0x14
    CAnimationStateBlock AnimationState;           // +0x28
    virtual void v00();
    ~CMeshBankStateBlock();
};

CMeshBankStateBlock::~CMeshBankStateBlock()
{
}