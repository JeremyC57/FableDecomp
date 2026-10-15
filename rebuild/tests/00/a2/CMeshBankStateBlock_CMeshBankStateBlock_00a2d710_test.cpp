#include <cstdio>
#include <cstdlib>
#include <cstring>
// CMeshBankStateBlock::~CMeshBankStateBlock: the excluded-bone set cleared (erase from the root, header reset) and its
// header freed, then the LOD state block destroyed, then the empty CBankStateBlock destructor
static char g_log[32]; static void* g_erasedRoot; static void* g_freedHeader;
static void Log(const char* s) { std::strcat(g_log, s); }
struct SRbNode { long m_color; SRbNode* m_pParent; SRbNode* m_pLeft; SRbNode* m_pRight; };
struct CRetailStringSet {
    SRbNode* m_pHeader; unsigned long m_count; unsigned long m_compare;
    void Erase(SRbNode* node) { g_erasedRoot = node; Log("e"); }
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
        if (m_pHeader) {
            bool reset = m_pHeader->m_pLeft == m_pHeader && m_pHeader->m_pRight == m_pHeader && m_pHeader->m_pParent == 0 && m_count == 0;
            Log(reset ? "h" : "X");
            g_freedHeader = m_pHeader;
            std::free(m_pHeader);
        }
    }
};
struct CMeshStateBlock { bool EnableLOD, PreserveMaterials, GenericAnimation, UnpackedPositions; long MaxLODs, ResourcePhysicsMeshIndex, AnimationVersion; };
struct CLODStateBlock { ~CLODStateBlock() { Log("L"); } };
struct CAnimationStateBlock { CRetailStringSet ExcludedBones; };
struct CBankStateBlock { virtual void v00() { } ~CBankStateBlock() { Log("B"); } };
struct CMeshBankStateBlock : CBankStateBlock {
    CMeshStateBlock State; CLODStateBlock LODState; CAnimationStateBlock AnimationState;
    virtual void v00() { }
    ~CMeshBankStateBlock() { }
};
int main()
{
    SRbNode root; root.m_pParent = 0; root.m_pLeft = 0; root.m_pRight = 0;
    SRbNode* header = (SRbNode*)std::malloc(sizeof(SRbNode));
    header->m_pParent = &root; header->m_pLeft = &root; header->m_pRight = &root;
    {
        CMeshBankStateBlock b;
        b.AnimationState.ExcludedBones.m_pHeader = header;
        b.AnimationState.ExcludedBones.m_count = 1;
    }
    if (std::strcmp(g_log, "ehLB") != 0 || g_erasedRoot != &root || g_freedHeader != header) { std::printf("00a2d710_TEST FAIL\n"); return 1; }
    std::printf("00a2d710_TEST PASS\n");
    return 0;
}