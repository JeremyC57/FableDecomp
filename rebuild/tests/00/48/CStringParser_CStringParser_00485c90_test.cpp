#include <cstdio>
#include <cstdlib>
namespace N_0099a2d0 { struct CBase { virtual ~CBase() {} CBase() {} }; } using N_0099a2d0::CBase;
struct CCharString { char* m_pData; CCharString() : m_pData(0) {} };
struct CTokenList {
    struct SNode { SNode* next; SNode* prev; char* text; long length; };
    SNode* m_head;
    CTokenList() : m_head(0) { SNode* node = (SNode*)std::malloc(sizeof(SNode)); node->next = node; node->prev = node; m_head = node; }
    ~CTokenList() { std::free(m_head); }
};
struct CStringParser : CBase { long m_position; long m_length; CCharString m_text; CCharString m_separators; CTokenList m_tokens; CStringParser() : m_text(), m_separators(), m_tokens() { m_position = 0; } };
int main()
{
    CStringParser p;
    if (p.m_position != 0 || p.m_text.m_pData != 0 || p.m_separators.m_pData != 0 || p.m_tokens.m_head == 0 || p.m_tokens.m_head->next != p.m_tokens.m_head || p.m_tokens.m_head->prev != p.m_tokens.m_head) { std::printf("00485c90_TEST FAIL\n"); return 1; }
    std::printf("00485c90_TEST PASS\n"); return 0;
}