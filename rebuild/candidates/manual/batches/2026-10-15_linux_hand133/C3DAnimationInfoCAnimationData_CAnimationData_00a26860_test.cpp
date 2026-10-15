// Behaviour test for C3DAnimationInfo::CAnimationData::~CAnimationData @ 0x00A26860
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_destroyed = 0;
static void DestroyObject(void* object) { g_destroyed += *(int*)object; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
        }
        m_pObject = 0; m_pCount = 0;
    }
};
template <class T> static void Share(CCountedPointer<T>& p, T* object, SCountBlock* block) { p.m_pObject = object; p.m_pCount = block; }
static SCountBlock* NewBlock(void* object, long refs) { SCountBlock* b = new SCountBlock; b->m_references = refs; b->m_pfnDestroy = DestroyObject; b->m_pObject = object; return b; }
static int g_strings = 0;
class CCharString { public: void* PStringData; ~CCharString() { ++g_strings; } };
struct SRbTreeNode { unsigned char colour; SRbTreeNode* parent; SRbTreeNode* left; SRbTreeNode* right; };
static int g_erased = 0, g_headerFreed = 0, g_baseDestroyed = 0;
struct CAnimationMap {
    SRbTreeNode* m_header; unsigned int m_nodeCount; unsigned char m_keyCompare;
    void Erase(SRbTreeNode* node) { while (node) { SRbTreeNode* l = node->left; Erase(node->right); std::free(node); ++g_erased; node = l; } }
    void clear()
    {
        if (m_nodeCount != 0) { Erase(m_header->parent); m_header->left = m_header; m_header->parent = 0; m_header->right = m_header; m_nodeCount = 0; }
    }
    ~CAnimationMap() { clear(); if (m_header) { ++g_headerFreed; std::free(m_header); } }
};
class CAnimationDataBase { public: virtual void v00() {} ~CAnimationDataBase() { ++g_baseDestroyed; } };
class CAnimationData : public CAnimationDataBase {
public:
    CAnimationMap Animations; CCountedPointer<int> SharedAnimation; CCountedPointer<int> FileData; void* AnimInfo;
    ~CAnimationData() { AnimInfo = 0; }
};
static SRbTreeNode* Node() { SRbTreeNode* n = (SRbTreeNode*)std::malloc(sizeof(SRbTreeNode)); n->colour = 0; n->parent = n->left = n->right = 0; return n; }
int main()
{
    int anim = 5, file = 100;
    static unsigned char storage[sizeof(CAnimationData)];
    CAnimationData* a = new (storage) CAnimationData;
    SRbTreeNode* h = Node(); SRbTreeNode* root = Node(); SRbTreeNode* kid = Node();
    root->left = kid; root->parent = h; kid->parent = root; h->parent = root; h->left = kid; h->right = root;
    a->Animations.m_header = h; a->Animations.m_nodeCount = 2;
    Share(a->SharedAnimation, &anim, NewBlock(&anim, 1)); Share(a->FileData, &file, NewBlock(&file, 1));
    a->AnimInfo = &anim;
    a->~CAnimationData();
    if (g_destroyed != 105 || g_erased != 2 || g_headerFreed != 1 || g_baseDestroyed != 1 || a->AnimInfo || a->Animations.m_nodeCount != 0) {
        std::printf("00a26860_TEST FAIL\n"); return 1;
    }
    std::printf("00a26860_TEST PASS\n");
    return 0;
}
