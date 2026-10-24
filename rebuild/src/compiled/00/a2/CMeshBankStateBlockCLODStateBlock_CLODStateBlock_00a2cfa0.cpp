// CMeshBankStateBlock::CLODStateBlock::CLODStateBlock() -- empty bone-LOD map and null bounding-box range pointer
// Leaf: ??0CLODStateBlock@CMeshBankStateBlock@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2CFA0, 51 bytes, plain `ret`.
// Identity: FableWin.pdb `??0CLODStateBlock@CMeshBankStateBlock@@QAE@XZ`. Ego_r.pdb CLODStateBlock = map<CCharString,
//   long> BoneLODs + CCountedPointer<C3DBoundingBox> MinXYZRange; retail is 0x14 bytes (12-byte STLport map, node 0x18 =
//   0x10 link + 8-byte pair), the same block inlined at +0x14 of CMeshBankStateBlock::CMeshBankStateBlock (0x00A2D650);
//   its destructor is 0x00A2CFE0. The catalog label "StdTree_DefaultCtor_ExtraFields_a2cfa0" is descriptive.
// Shape: the inlined STLport _Rb_tree constructor (alloc proxy nulls the header, malloc 0x00BFEA0E of one node, count 0,
//   colour red, root 0, leftmost/rightmost = header with the header re-read per store) then the counted pointer's
//   Data/Info zeroed from the shared zero register.
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

class CMeshBankStateBlock {
public:
    class CLODStateBlock {                              // retail 0x14 bytes
    public:
        CRetailRbTree<SBoneLOD>         BoneLODs;       // +0x00 map<CCharString, long>
        CCountedPointer<C3DBoundingBox> MinXYZRange;    // +0x0C
        CLODStateBlock();
    };
};

CMeshBankStateBlock::CLODStateBlock::CLODStateBlock()
{
}