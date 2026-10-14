#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CCharStringData { const char* m_pData; unsigned long m_length; };
struct CCharString { CCharStringData* m_pData;
    bool operator==(const CCharString& other) const { CCharStringData* a = m_pData; CCharStringData* b = other.m_pData; if (a == b) return true;
        if (a && b && a->m_length == b->m_length) return std::strcmp(a->m_pData, b->m_pData) == 0; return false; } };
struct CModeBlock { unsigned char _pad[0x48]; CCharString m_name; CCharString& GetName() { return m_name; } };
struct SBrushNode { SBrushNode* m_pNext; SBrushNode* m_pPrev; CModeBlock m_block; };
struct CMapBrushList { unsigned char _pad[0xc]; SBrushNode* m_pHead;
    CModeBlock* GetScriptedMapBrush(const CCharString& name) { SBrushNode* node = m_pHead->m_pNext; for (; node != m_pHead; node = node->m_pNext) { if (node->m_block.GetName() == name) return &node->m_block; } throw 1; } };

int main()
{
    CCharStringData da = { "alpha", 5 }, db = { "beta", 4 }, dc = { "beta", 4 }, dd = { "gamma", 5 };
    SBrushNode head, n1, n2; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; n1.m_block.m_name.m_pData = &da; n2.m_block.m_name.m_pData = &db;
    CMapBrushList l; l.m_pHead = &head; CCharString key; key.m_pData = &dc;
    if (l.GetScriptedMapBrush(key) != &n2.m_block) { std::printf("0081e960_TEST FAIL\n"); return 1; } key.m_pData = &da; if (l.GetScriptedMapBrush(key) != &n1.m_block) { std::printf("0081e960_TEST FAIL\n"); return 1; }
    key.m_pData = &dd; bool thrown = false; try { l.GetScriptedMapBrush(key); } catch (int) { thrown = true; } if (!thrown) { std::printf("0081e960_TEST FAIL\n"); return 1; }
    key.m_pData = 0; thrown = false; try { l.GetScriptedMapBrush(key); } catch (int) { thrown = true; } if (!thrown) { std::printf("0081e960_TEST FAIL\n"); return 1; }
    std::printf("0081e960_TEST PASS\n");
    return 0;
}
