#include <cstdio>
struct CComponent { virtual long GetComponentType() = 0; };
struct CText : CComponent { long GetComponentType() { return 6; } }; struct COther : CComponent { long GetComponentType() { return 2; } };
struct SChildEntry { CComponent* m_pComponent; long m_tag; };
struct CEditBox { unsigned char _pad[0xb0]; SChildEntry* m_pChildrenFirst; SChildEntry* m_pChildrenLast;
    unsigned long GetChildCount() const { return m_pChildrenLast - m_pChildrenFirst; }
    CComponent* GetTextComponent() { for (unsigned long i = 0; i != GetChildCount(); ++i) if (m_pChildrenFirst[i].m_pComponent->GetComponentType() == 6) return m_pChildrenFirst[i].m_pComponent; return 0; } };
int main() { COther o; CText t; SChildEntry e[2] = { { &o, 0 }, { &t, 1 } }; CEditBox b; b.m_pChildrenFirst = e; b.m_pChildrenLast = e + 2;
    if (b.GetTextComponent() != &t) { std::printf("005402c0_TEST FAIL\n"); return 1; } b.m_pChildrenLast = e + 1; if (b.GetTextComponent() != 0) { std::printf("005402c0_TEST FAIL\n"); return 1; } b.m_pChildrenLast = e; if (b.GetTextComponent() != 0) { std::printf("005402c0_TEST FAIL\n"); return 1; } std::printf("005402c0_TEST PASS\n"); return 0; }
