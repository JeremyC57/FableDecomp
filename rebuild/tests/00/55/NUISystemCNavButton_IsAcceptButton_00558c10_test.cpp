#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CComp { CComp* m_pParent; long m_type; CComp* m_pAccept; CComp() : m_pParent(0), m_type(0), m_pAccept(0) { } virtual CComp* GetParent() { return m_pParent; } virtual long GetType() { return m_type; } virtual CComp* GetAcceptButton() { return m_pAccept; } };
struct CNavButton : CComp { bool IsAcceptButton() { CComp* c = GetParent(); while (c) { if (c->GetParent() == 0) { if (c->GetType() == 10 && c->GetAcceptButton() == this) return true; return false; } c = c->GetParent(); } return false; } };

int main() {
    CComp root, mid; CNavButton b; root.m_pParent = 0; root.m_type = 10; root.m_pAccept = &b; mid.m_pParent = &root; b.m_pParent = &mid; if (!b.IsAcceptButton()) { std::printf("00558c10_TEST FAIL\n"); return 1; } root.m_type = 9; if (b.IsAcceptButton()) { std::printf("00558c10_TEST FAIL\n"); return 1; } root.m_type = 10; root.m_pAccept = &mid; if (b.IsAcceptButton()) { std::printf("00558c10_TEST FAIL\n"); return 1; } b.m_pParent = 0; if (b.IsAcceptButton()) { std::printf("00558c10_TEST FAIL\n"); return 1; }
    std::printf("00558c10_TEST PASS\n"); return 0;
}