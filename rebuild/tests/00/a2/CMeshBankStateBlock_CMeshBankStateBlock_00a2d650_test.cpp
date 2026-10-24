// Behaviour test for CMeshBankStateBlock::CMeshBankStateBlock (0x00A2D650): the object is a CMeshBankStateBlock
// (virtual dispatch reaches the derived class), its bone-LOD map and excluded-bone set are empty, well-formed trees
// with distinct headers, and the bounding-box range pointer is null.
#include <cstdio>
#include <cstdlib>

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
    int IsEmptyTree() const
    {
        return m_header != 0 && m_header->colour == 0 && m_header->parent == 0 &&
               m_header->left == m_header && m_header->right == m_header && m_nodeCount == 0;
    }
};

struct SBoneLOD { CCharString first; long second; };

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};

class CMeshStateBlock {
public:
    bool EnableLOD;
    bool PreserveMaterials;
    bool GenericAnimation;
    bool UnpackedPositions;
    long MaxLODs;
    long ResourcePhysicsMeshIndex;
    long AnimationVersion;
};

class CBankStateBlock {
public:
    virtual ~CBankStateBlock() {}
    virtual int Kind() const { return 0; }
};

class CMeshBankStateBlock : public CBankStateBlock {
public:
    class CLODStateBlock {
    public:
        CRetailRbTree<SBoneLOD>         BoneLODs;
        CCountedPointer<C3DBoundingBox> MinXYZRange;
        CLODStateBlock() {}
    };
    class CAnimationStateBlock {
    public:
        CRetailRbTree<CCharString> ExcludedBones;
    };
    CMeshStateBlock      State;
    CLODStateBlock       LODState;
    CAnimationStateBlock AnimationState;
    CMeshBankStateBlock();
    virtual ~CMeshBankStateBlock()
    {
        std::free(LODState.BoneLODs.m_header);
        std::free(AnimationState.ExcludedBones.m_header);
    }
    virtual int Kind() const { return 1; }
};

CMeshBankStateBlock::CMeshBankStateBlock()
{
}

int main()
{
    int ok = 1;
    CMeshBankStateBlock* m = new CMeshBankStateBlock;
    CBankStateBlock* b = m;
    if (b->Kind() != 1) ok = 0;
    if (!m->LODState.BoneLODs.IsEmptyTree()) ok = 0;
    if (!m->AnimationState.ExcludedBones.IsEmptyTree()) ok = 0;
    if ((void*)m->LODState.BoneLODs.m_header == (void*)m->AnimationState.ExcludedBones.m_header) ok = 0;
    if (m->LODState.MinXYZRange.Data != 0 || m->LODState.MinXYZRange.Info != 0) ok = 0;
    delete b;

    if (!ok) { std::printf("00a2d650_TEST FAIL\n"); return 1; }
    std::printf("00a2d650_TEST PASS\n");
    return 0;
}