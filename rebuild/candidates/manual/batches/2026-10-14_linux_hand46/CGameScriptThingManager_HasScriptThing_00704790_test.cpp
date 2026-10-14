#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CScriptThingRecord { unsigned char _pad[0x18]; long long m_uid; }; struct SUidNode { SUidNode* m_pNext; SUidNode* m_pPrev; long long m_uid; };
struct CGameScriptThingManager { SUidNode* m_pHead; bool HasScriptThing(const CScriptThingRecord& record) { SUidNode* head = m_pHead; for (SUidNode* node = head->m_pNext; node != head; node = node->m_pNext) { if (node->m_uid == record.m_uid) return true; } return false; } };

int main() {
    SUidNode head, n1, n2; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; n1.m_uid = 0x100000005LL; n2.m_uid = 0x200000005LL; CGameScriptThingManager m; m.m_pHead = &head; CScriptThingRecord r;
    r.m_uid = 0x200000005LL; if (!m.HasScriptThing(r)) { std::printf("00704790_TEST FAIL\n"); return 1; } r.m_uid = 0x100000005LL; if (!m.HasScriptThing(r)) { std::printf("00704790_TEST FAIL\n"); return 1; } r.m_uid = 5; if (m.HasScriptThing(r)) { std::printf("00704790_TEST FAIL\n"); return 1; } r.m_uid = 0x100000006LL; if (m.HasScriptThing(r)) { std::printf("00704790_TEST FAIL\n"); return 1; } head.m_pNext = &head; r.m_uid = 0x100000005LL; if (m.HasScriptThing(r)) { std::printf("00704790_TEST FAIL\n"); return 1; }
    std::printf("00704790_TEST PASS\n"); return 0;
}
