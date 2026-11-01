// Behaviour test for CSettingsProxy_012b29f8::SetOptionCD (0x00C14BC0, vtable 0x012B29F8 slot 2): under the
// PKCProtectionInstance guard, store the option byte (+cd) and set the record's dirty flag only when the value changes;
// the guard is entered and left exactly once on both paths.
#include <stdio.h>
static int enters, leaves;
struct Guard { Guard() { ++enters; } ~Guard() { ++leaves; } };
struct Record { unsigned long m_00; bool m_dirty; unsigned char pad[0xc7]; bool m_optionCC; bool m_optionCD; };
struct Proxy {
    void* vt; unsigned long m_04; Record* m_pSettings;
    void Set(bool value) { Guard lock; if (m_pSettings->m_optionCD == value) return; m_pSettings->m_optionCD = value; m_pSettings->m_dirty = true; }
};
int main()
{
    Record r = Record(); Proxy p; p.m_pSettings = &r;
    p.Set(false);
    if (r.m_dirty || r.m_optionCD || enters != 1 || leaves != 1) { printf("00c14bc0_TEST FAIL\n"); return 1; }
    p.Set(true);
    if (!r.m_dirty || !r.m_optionCD || enters != 2 || leaves != 2) { printf("00c14bc0_TEST FAIL\n"); return 1; }
    r.m_dirty = false; p.Set(true);
    if (r.m_dirty || !r.m_optionCD || leaves != 3) { printf("00c14bc0_TEST FAIL\n"); return 1; }
    printf("00c14bc0_TEST PASS\n"); return 0;
}