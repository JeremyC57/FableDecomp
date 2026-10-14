#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CThing { unsigned char _pad[0x91]; unsigned char m_91_0 : 1; unsigned char m_91_1 : 7; }; struct CIntelligentPointer { virtual void v00() { } void* m_pItem; CThing* GetPItem() const { return (CThing*)m_pItem; } };
struct SHitRecord { unsigned char _pad[0x50]; CIntelligentPointer m_hitBy; CThing* GetHitBy() const { return m_hitBy.GetPItem(); } }; struct SHitEvent { unsigned char _pad[8]; SHitRecord* m_pRecord; };
struct CIsHitEventHasHitByThing { bool operator()(const SHitEvent* event) { CThing* thing = event->m_pRecord->GetHitBy(); if (thing && !thing->m_91_0) return true; return false; } };

int main() {
    CThing t; std::memset(&t, 0, sizeof t); SHitRecord r; r.m_hitBy.m_pItem = &t; SHitEvent e; e.m_pRecord = &r; CIsHitEventHasHitByThing f; if (!f(&e)) { std::printf("00810f90_TEST FAIL\n"); return 1; } t.m_91_0 = 1; if (f(&e)) { std::printf("00810f90_TEST FAIL\n"); return 1; } r.m_hitBy.m_pItem = 0; if (f(&e)) { std::printf("00810f90_TEST FAIL\n"); return 1; }
    std::printf("00810f90_TEST PASS\n"); return 0;
}
