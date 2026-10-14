#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CEvent { virtual void v000() { } virtual bool IsButton(long button) { return m_button == button; } long m_button; };

int main() {
    CEvent e; e.m_button = 3; if (!e.IsButton(3) || e.IsButton(2)) { std::printf("00447a90_TEST FAIL\n"); return 1; }
    std::printf("00447a90_TEST PASS\n"); return 0;
}