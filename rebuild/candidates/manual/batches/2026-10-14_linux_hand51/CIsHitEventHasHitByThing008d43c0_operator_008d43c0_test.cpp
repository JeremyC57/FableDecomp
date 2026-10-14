#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CThing { unsigned char _pad[0x8c]; unsigned short m_id; unsigned char _pad8e[3]; unsigned char m_91_0 : 1; unsigned char m_91_1 : 7; }; struct SHitEvent { CThing* m_pThing; CThing* GetHitThing() const { return m_pThing; } };
struct CIsHitEventHasHitByThing { long m_id; bool operator()(const SHitEvent* event) { CThing* thing = event->GetHitThing(); if (thing && !thing->m_91_0 && thing->m_id == m_id) return true; return false; } };

int main() {
    CThing t; std::memset(&t, 0, sizeof t); t.m_id = 7; SHitEvent e; e.m_pThing = &t; CIsHitEventHasHitByThing f; f.m_id = 7; if (!f(&e)) { std::printf("008d43c0_TEST FAIL\n"); return 1; } f.m_id = 8; if (f(&e)) { std::printf("008d43c0_TEST FAIL\n"); return 1; } f.m_id = 7; t.m_91_0 = 1; if (f(&e)) { std::printf("008d43c0_TEST FAIL\n"); return 1; } t.m_91_0 = 0; e.m_pThing = 0; if (f(&e)) { std::printf("008d43c0_TEST FAIL\n"); return 1; }
    std::printf("008d43c0_TEST PASS\n"); return 0;
}
