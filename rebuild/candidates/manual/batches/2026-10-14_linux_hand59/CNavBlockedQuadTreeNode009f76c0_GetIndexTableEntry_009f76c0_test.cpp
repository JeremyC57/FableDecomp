#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SRec { long m_0, m_4, m_entry; }; struct CNode { SRec* m_pRecord; long* GetIndexTableEntry() { return &m_pRecord->m_entry; } };

int main() {
    SRec r; r.m_entry = 5; CNode n; n.m_pRecord = &r; if (n.GetIndexTableEntry() != &r.m_entry) { std::printf("009f76c0_TEST FAIL\n"); return 1; }
    std::printf("009f76c0_TEST PASS\n"); return 0;
}
