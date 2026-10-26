#include <cstdio>
#include <cstdlib>
#include <cstring>
class CEngineComponent { public: virtual void v00() { } long GlobalPointerAddress; bool m_enabled, KeepAlive; CEngineComponent() : GlobalPointerAddress(0), m_enabled(true), KeepAlive(false) { } };

int main() {
    CEngineComponent* c = new CEngineComponent; if (c->GlobalPointerAddress != 0 || !c->m_enabled || c->KeepAlive) { std::printf("00b59710_TEST FAIL\n"); return 1; } delete c;
    std::printf("00b59710_TEST PASS\n"); return 0;
}