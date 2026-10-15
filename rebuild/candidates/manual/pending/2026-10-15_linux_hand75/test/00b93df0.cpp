#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
struct SRbTreeNodeBase { long m_color; SRbTreeNodeBase* m_parent; SRbTreeNodeBase* m_left; SRbTreeNodeBase* m_right; };
static int g_erased = 0; static SRbTreeNodeBase* g_erasedRoot = 0;
struct CBoneConfigTree {
    SRbTreeNodeBase* m_header; unsigned long m_nodeCount; unsigned char m_compare;
    void Erase(SRbTreeNodeBase* node) { ++g_erased; g_erasedRoot = node; }
    SRbTreeNodeBase*& Root() { return m_header->m_parent; }
    SRbTreeNodeBase*& Leftmost() { return m_header->m_left; }
    SRbTreeNodeBase*& Rightmost() { return m_header->m_right; }
    void clear()
    {
        if (m_nodeCount != 0) { Erase(Root()); Leftmost() = m_header; Root() = 0; Rightmost() = m_header; m_nodeCount = 0; }
    }
};
struct SBoneConfigRecord { long m_first; long m_second; long m_third; SBoneConfigRecord() : m_first(0), m_second(0), m_third(0) { } };
struct CSkeletalMorphResourceManager {
    CBoneConfigTree   m_boneConfigs;
    SBoneConfigRecord m_boneConfigRecord;
    void UnloadBoneConfigs() { m_boneConfigs.clear(); m_boneConfigRecord = SBoneConfigRecord(); }
};
int main()
{
    SRbTreeNodeBase header, root, left;
    header.m_parent = &root; header.m_left = &left; header.m_right = &root;
    CSkeletalMorphResourceManager m; m.m_boneConfigs.m_header = &header; m.m_boneConfigs.m_nodeCount = 2;
    m.m_boneConfigRecord.m_first = 1; m.m_boneConfigRecord.m_second = 2; m.m_boneConfigRecord.m_third = 3;
    m.UnloadBoneConfigs();
    if (g_erased != 1 || g_erasedRoot != &root || header.m_parent != 0 || header.m_left != &header || header.m_right != &header || m.m_boneConfigs.m_nodeCount != 0
        || m.m_boneConfigRecord.m_first || m.m_boneConfigRecord.m_second || m.m_boneConfigRecord.m_third) { std::printf("00b93df0_TEST FAIL\n"); return 1; }
    m.m_boneConfigRecord.m_second = 5; m.UnloadBoneConfigs();
    if (g_erased != 1 || m.m_boneConfigRecord.m_second != 0) { std::printf("00b93df0_TEST FAIL\n"); return 1; }
    std::printf("00b93df0_TEST PASS\n");
    return 0;
}
