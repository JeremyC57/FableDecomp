#include <cstdio>
#include <cstdlib>
#include <cstring>
class CBankFileAsyncData { public: long m_priority; bool m_finished; void* m_pData; void ChangeLoadingPriority(long p) { m_priority = p; } bool IsFinished() { return m_finished; } void* GetData() { return m_pData; } };
struct SBlockNode_00be0400 { unsigned char _pad[0x14]; long m_pending; bool m_highPriority; unsigned char _pad19[3]; CBankFileAsyncData* m_pData; };
static SBlockNode_00be0400 g_node, g_end; static long g_key = 0;
struct CRetailBlockSet { SBlockNode_00be0400* m_pEnd; void FindInsertionPoint(SBlockNode_00be0400*& r, const long& k) { g_key = k; r = &g_node; } void Find(SBlockNode_00be0400*& r, const long& k) { g_key = k; r = k == 9 ? m_pEnd : &g_node; } };
class CEngineStaticMapFileBlockManager { public: unsigned char _pad[8]; CRetailBlockSet m_blocks; void ReleaseRequest(long block) { SBlockNode_00be0400* node; m_blocks.FindInsertionPoint(node, block); --node->m_pending; } };
int main() {
    CEngineStaticMapFileBlockManager m; g_node.m_pending = 3; m.ReleaseRequest(5); if (g_key != 5 || g_node.m_pending != 2) { std::printf("00be0400_TEST FAIL\n"); return 1; }
    std::printf("00be0400_TEST PASS\n"); return 0;
}