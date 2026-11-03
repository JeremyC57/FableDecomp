// Behaviour test for CEventTriggerRef::GetDriverCount (0x00C1EE30): the referenced trigger's driver list (virtual slot 5) asked for
// its count (virtual slot 5); a standalone model with real virtual tables.
#include <cstdio>
class DriverList { public: unsigned long n;
    virtual void a(); virtual void b(); virtual void c(); virtual void d(); virtual void e();
    virtual unsigned long GetCount() { return n; } };
void DriverList::a() {} void DriverList::b() {} void DriverList::c() {} void DriverList::d() {} void DriverList::e() {}
class Trigger { public: DriverList* list;
    virtual void a(); virtual void b(); virtual void c(); virtual void d(); virtual void e();
    virtual DriverList* GetDrivers() { return list; } };
void Trigger::a() {} void Trigger::b() {} void Trigger::c() {} void Trigger::d() {} void Trigger::e() {}
struct TriggerRef { void* registry; Trigger* trigger;
    unsigned long GetDriverCount() { return trigger->GetDrivers()->GetCount(); } };
int main()
{
    DriverList l; l.n = 3;
    Trigger t; t.list = &l;
    TriggerRef r = { 0, &t };
    if (r.GetDriverCount() != 3) { std::printf("00c1ee30_TEST FAIL\n"); return 1; }
    l.n = 0;
    if (r.GetDriverCount() != 0) { std::printf("00c1ee30_TEST FAIL\n"); return 1; }
    std::printf("00c1ee30_TEST PASS\n");
    return 0;
}