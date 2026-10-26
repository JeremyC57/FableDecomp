#include <cstdio>
#include <cstdlib>

// Behaviour model of CSkeletalMorphResourceManager::CSkeletalMorphResourceManager (0x004AE2A0): the singleton base,
// then an empty CachedMorphs map: one 0x1C-byte header node, colour red (0), no root, leftmost = rightmost = header,
// count 0.
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
class CCharString { public: void* p; };
struct CCountedPointer { void* data; long* count; };
struct MorphNode : NodeBase { CCharString first; CCountedPointer second; };
static unsigned int g_lastAlloc = 0;
struct CTBaseSingleton { virtual ~CTBaseSingleton() { } virtual int Kind() const { return 0; } protected: CTBaseSingleton() { } };
struct MorphMap {
    MorphNode* header; unsigned int count; char compare;
    MorphMap() : header(0)
    {
        g_lastAlloc = sizeof(MorphNode);
        header = (MorphNode*)std::malloc(sizeof(MorphNode));
        count = 0;
        header->color = false; header->parent = 0; header->left = header; header->right = header;
    }
    ~MorphMap() { std::free(header); }
};
class CSkeletalMorphResourceManager : public CTBaseSingleton { public:
    MorphMap CachedMorphs;
    CSkeletalMorphResourceManager() { }
    int Kind() const { return 1; }
};

int main()
{
    CSkeletalMorphResourceManager m;
    const CTBaseSingleton& base = m;
    MorphNode* h = m.CachedMorphs.header;
    if (base.Kind() != 1) { std::printf("004ae2a0_TEST FAIL\n"); return 1; }                         // derived vtable in place
    if (!h || h->color != false || h->parent != 0 || h->left != h || h->right != h || m.CachedMorphs.count != 0) { std::printf("004ae2a0_TEST FAIL\n"); return 1; }
    if (g_lastAlloc != sizeof(NodeBase) + sizeof(CCharString) + sizeof(CCountedPointer)) { std::printf("004ae2a0_TEST FAIL\n"); return 1; }
    std::printf("004ae2a0_TEST PASS\n");
    return 0;
}