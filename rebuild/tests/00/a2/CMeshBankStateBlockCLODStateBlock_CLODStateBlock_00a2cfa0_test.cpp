// Behaviour test for CMeshBankStateBlock::CLODStateBlock::CLODStateBlock (0x00A2CFA0): an empty, well-formed bone-LOD
// tree (header allocated, red, no root, leftmost/rightmost = header, count 0) and a null bounding-box range pointer.
#include <cstdio>
#include <cstdlib>
#include <new>

class CCharString { public: void* PStringData; };
class C3DBoundingBox;
class CCPPointerInfo;

template <class _Value> class CRetailRbTree {
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
        m_header = (node*)std::malloc(sizeof(node));
        m_nodeCount = 0;
        m_header->colour = 0;
        m_header->parent = 0;
        m_header->left = m_header;
        m_header->right = m_header;
    }
};

struct SBoneLOD { CCharString first; long second; };

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};

class CMeshBankStateBlock {
public:
    class CLODStateBlock {
    public:
        CRetailRbTree<SBoneLOD>         BoneLODs;
        CCountedPointer<C3DBoundingBox> MinXYZRange;
        CLODStateBlock();
    };
};

CMeshBankStateBlock::CLODStateBlock::CLODStateBlock()
{
}

int main()
{
    int ok = 1;
    unsigned char raw[sizeof(CMeshBankStateBlock::CLODStateBlock)];
    for (unsigned int i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    CMeshBankStateBlock::CLODStateBlock* b = new (raw) CMeshBankStateBlock::CLODStateBlock;
    CRetailRbTree<SBoneLOD>::node* h = b->BoneLODs.m_header;
    if (h == 0) ok = 0;
    else if (h->colour != 0 || h->parent != 0 || h->left != h || h->right != h) ok = 0;
    if (b->BoneLODs.m_nodeCount != 0) ok = 0;
    if (b->MinXYZRange.Data != 0 || b->MinXYZRange.Info != 0) ok = 0;
    std::free(h);

    if (!ok) { std::printf("00a2cfa0_TEST FAIL\n"); return 1; }
    std::printf("00a2cfa0_TEST PASS\n");
    return 0;
}