// CMeshBankStateBlock::CMeshBankStateBlock() -- empty bone-LOD map, null bounding-box range and empty excluded-bone set
// Leaf: ??0CMeshBankStateBlock@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2D650, 98 bytes, plain `ret`.
// Identity: Ego_r.pdb `??0CMeshBankStateBlock@@QAE@XZ`; the vtable stored is CMeshBankStateBlock's 0x0129CE7C
//   (rtti-vtables.tsv; slot 0 is its scalar deleting destructor 0x00A2D770). Callers 0x00A271AB (CMeshDataBank) and
//   0x00B3BD64. The catalog label "CMeshDataBank_Initialize" is a descriptive guess.
// Layout (retail 0x34): CBankStateBlock vptr +0x00, State (16 bytes of plain data, left uninitialised) +0x04, LODState
//   (CLODStateBlock: BoneLODs map node 0x18 +0x14, MinXYZRange CCountedPointer +0x20) +0x14, AnimationState
//   (CAnimationStateBlock: set<CCharString> ExcludedBones, node 0x14) +0x28 -- as in the landed destructor 0x00A2D710.
// Shape: the derived vtable store first (the inline CBankStateBlock constructor's own store is dead and VC7.1 drops it,
//   so no __declspec(novtable) is needed), then both STLport tree constructors and the counted pointer inline.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E

class CCharString { public: void* PStringData; };
class C3DBoundingBox;
class CCPPointerInfo;

template <class _Value> class CRetailRbTree {           // STLport _Rb_tree: header, node count, empty compare (12 bytes)
public:
    struct node {
        char   colour;
        node*  parent;
        node*  left;
        node*  right;
        _Value value;
    };
    struct less { };
    node*         m_header;
    unsigned long m_nodeCount;
    less          m_compare;
    CRetailRbTree() : m_header(0)
    {
        m_header = (node*)malloc(sizeof(node));
        m_nodeCount = 0;
        m_header->colour = 0;
        m_header->parent = 0;
        m_header->left = m_header;
        m_header->right = m_header;
    }
};

struct SBoneLOD { CCharString first; long second; };   // pair<const CCharString, long>

template <class T> class CCountedPointer {              // 8 bytes
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};

class CMeshStateBlock {                                 // CMeshBankStateBlock::CStateBlock, 16 bytes of plain data
public:
    bool EnableLOD;
    bool PreserveMaterials;
    bool GenericAnimation;
    bool UnpackedPositions;
    long MaxLODs;
    long ResourcePhysicsMeshIndex;
    long AnimationVersion;
};

class CBankStateBlock {                                 // vtable 0x0129C82C
public:
    virtual ~CBankStateBlock() {}
};

class CMeshBankStateBlock : public CBankStateBlock {    // vtable 0x0129CE7C, 0x34 bytes
public:
    class CLODStateBlock {
    public:
        CRetailRbTree<SBoneLOD>         BoneLODs;       // +0x00
        CCountedPointer<C3DBoundingBox> MinXYZRange;    // +0x0C
        CLODStateBlock() {}
    };
    class CAnimationStateBlock {
    public:
        CRetailRbTree<CCharString> ExcludedBones;       // +0x00 set<CCharString>
    };
    CMeshStateBlock      State;                         // +0x04
    CLODStateBlock       LODState;                      // +0x14
    CAnimationStateBlock AnimationState;                // +0x28
    CMeshBankStateBlock();
    virtual ~CMeshBankStateBlock();
};

CMeshBankStateBlock::CMeshBankStateBlock()
{
}