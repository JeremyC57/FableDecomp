// Behaviour test for 0x00991750 (catalog CThing::OnDie): active flag cleared before its virtual, then every entry object shut down.
#include <cstdio>
struct CEntryObject { int shut; CEntryObject() : shut(0) { } virtual void Shutdown() { ++shut; } };
struct SEntry { unsigned long m_key; CEntryObject* m_pObject; unsigned char pad[28]; };
class CThing { public:
    bool m_active; int deactivations; bool activeDuringCall;
    SEntry* m_entriesFirst; SEntry* m_entriesLast;
    virtual void Deactivate() { ++deactivations; activeDuringCall = m_active; }
    void OnDie()
    {
        if (m_active) { m_active = false; Deactivate(); }
        for (SEntry* it = m_entriesFirst; it != m_entriesLast; ++it)
            it->m_pObject->Shutdown();
    }
};
int main()
{
    CEntryObject a, b; SEntry v[2]; v[0].m_pObject = &a; v[1].m_pObject = &b;
    CThing t; t.m_active = true; t.deactivations = 0; t.activeDuringCall = true; t.m_entriesFirst = v; t.m_entriesLast = v + 2;
    t.OnDie();
    if (t.m_active || t.deactivations != 1 || t.activeDuringCall || a.shut != 1 || b.shut != 1) { std::printf("00991750_TEST FAIL\n"); return 1; }
    t.OnDie();
    if (t.deactivations != 1 || a.shut != 2) { std::printf("00991750_TEST FAIL\n"); return 1; }
    std::printf("00991750_TEST PASS\n");
    return 0;
}