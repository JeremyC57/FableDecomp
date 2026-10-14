#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CEngineComponent { virtual void v00() { } long m_4; bool m_enabled, m_9; CEngineComponent() : m_4(0), m_enabled(true), m_9(false) { } };

int main() {
    CEngineComponent* c = new CEngineComponent; if (c->m_4 != 0 || !c->m_enabled || c->m_9) { std::printf("00b59710_TEST FAIL\n"); return 1; } delete c;
    std::printf("00b59710_TEST PASS\n"); return 0;
}