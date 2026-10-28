// Test: CAmbientTrigger::UpdateVolume -- with a live handle (>= 0) the source's handle table gets the source's volume for that handle; a negative handle does nothing.
#include <cstdio>
struct CTable { int h; float v; int calls; void SetVolume(int handle, float vol) { h = handle; v = vol; ++calls; } };
struct CSource { CTable table; float vol; float GetVolume() { return vol; } };
struct CTrigger { int m_handle; CTable* GetHandleTable(CSource* s) { return &s->table; }
    void UpdateVolume(CSource* s) { if (m_handle >= 0) { CTable* t = GetHandleTable(s); t->SetVolume(m_handle, s->GetVolume()); } } };
int main()
{
    CSource s; s.table.calls = 0; s.vol = 0.75f; CTrigger t; t.m_handle = -1; t.UpdateVolume(&s);
    bool ok = s.table.calls == 0;
    t.m_handle = 0; t.UpdateVolume(&s); ok = ok && s.table.calls == 1 && s.table.h == 0 && s.table.v == 0.75f;
    std::printf(ok ? "00c624c0_TEST PASS\n" : "00c624c0_TEST FAIL\n");
    return ok ? 0 : 1;
}