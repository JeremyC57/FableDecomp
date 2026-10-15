// Behaviour test for CBankFile::FindIndexBySymbol(const CCharString&) const @ 0x009CCDC0
#include <cstdio>
#include <string.h>

struct CCharString { const char* m_text; };

struct CSymbolIndexMap {
    struct SNode { CCharString m_key; unsigned long m_index; };
    SNode* m_pHead;                                // the head node doubles as end()
    SNode* m_pNodes; int m_count;
    SNode* find(const CCharString& key) const
    {
        for (int i = 0; i < m_count; ++i)
            if (strcmp(m_pNodes[i].m_key.m_text, key.m_text) == 0) return &m_pNodes[i];
        return m_pHead;
    }
};

struct CBankFile {
    CSymbolIndexMap SymbolIndices;
    unsigned long FindIndexBySymbol(const CCharString& symbol) const
    {
        const CSymbolIndexMap* indices = &SymbolIndices;
        CSymbolIndexMap::SNode* it = indices->find(symbol);
        if (it != indices->m_pHead)
            return it->m_index;
        return 0;
    }
};

int main()
{
    CSymbolIndexMap::SNode head, nodes[2];
    head.m_index = 999;
    nodes[0].m_key.m_text = "MESH_A"; nodes[0].m_index = 12;
    nodes[1].m_key.m_text = "MESH_B"; nodes[1].m_index = 40;
    CBankFile bank; bank.SymbolIndices.m_pHead = &head; bank.SymbolIndices.m_pNodes = nodes; bank.SymbolIndices.m_count = 2;
    CCharString b = { "MESH_B" }, a = { "MESH_A" }, x = { "MESH_X" };
    if (bank.FindIndexBySymbol(b) != 40 || bank.FindIndexBySymbol(a) != 12 || bank.FindIndexBySymbol(x) != 0) { std::printf("009ccdc0_TEST FAIL\n"); return 1; }
    std::printf("009ccdc0_TEST PASS\n");
    return 0;
}