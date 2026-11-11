// Behaviour test for CVtable_012b4f74::SetDesc (0x00C3C4B0): state bit 2 selects the re-apply virtual, otherwise the setup virtual;
// the descriptor and the result pass straight through.
#include <stdio.h>
struct SNotificationDesc { int id; };
struct CVtable_012b4f74 {
    unsigned long m_state;
    const SNotificationDesc* seenSetup; const SNotificationDesc* seenChange;
    CVtable_012b4f74() : m_state(0), seenSetup(0), seenChange(0) { }
    virtual ~CVtable_012b4f74() { }
    virtual long SetupEventNotification(const SNotificationDesc* d) { seenSetup = d; return 11; }
    virtual long ChangeEventNotification(const SNotificationDesc* d) { seenChange = d; return -9; }
    unsigned char IsRegisteredA() const { return (unsigned char)(m_state >> 2) & 1; }
    long SetDesc(const SNotificationDesc* desc);
};
long CVtable_012b4f74::SetDesc(const SNotificationDesc* desc)
{
    if (IsRegisteredA())
        return ChangeEventNotification(desc);
    return SetupEventNotification(desc);
}
int main(void)
{
    SNotificationDesc d; d.id = 1;
    CVtable_012b4f74 fresh; fresh.m_state = 0x0b;          // every bit but bit 2
    CVtable_012b4f74 live; live.m_state = 0x04;
    long a = fresh.SetDesc(&d), b = live.SetDesc(&d);
    if (a != 11 || fresh.seenSetup != &d || fresh.seenChange != 0 || b != -9 || live.seenChange != &d || live.seenSetup != 0) {
        printf("00c3c4b0_TEST FAIL\n"); return 1;
    }
    printf("00c3c4b0_TEST PASS\n");
    return 0;
}