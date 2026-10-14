#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CThing; struct CThingEventBase { CThing* m_pThing; CThingEventBase(CThing* thing) : m_pThing(thing) { } };
struct CHitThing : CThingEventBase { virtual void v00() { } bool m_8; long m_c; bool m_10; float m_scale; CHitThing(CThing* thing) : CThingEventBase(thing), m_8(false), m_c(0), m_10(true), m_scale(1.0f) { } };

int main() {
    int thing; CHitThing h((CThing*)&thing); if (h.m_pThing != (CThing*)&thing || h.m_8 || h.m_c || !h.m_10 || h.m_scale != 1.0f) { std::printf("0083ae30_TEST FAIL\n"); return 1; }
    std::printf("0083ae30_TEST PASS\n"); return 0;
}