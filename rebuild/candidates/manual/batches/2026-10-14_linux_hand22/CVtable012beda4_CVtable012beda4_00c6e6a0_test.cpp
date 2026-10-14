#include <cstdio>
struct _RTL_CRITICAL_SECTION;
static _RTL_CRITICAL_SECTION* g_initialised = 0;
extern "C" void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION* section) { g_initialised = section; }
struct CBase_012b5868 { virtual void v00() { } CBase_012b5868() { } };
struct CCriticalSectionOwner_012B8FC8 {
    virtual void v00() { }
    unsigned char CriticalSection[0x18];
    CCriticalSectionOwner_012B8FC8()
    {
        InitializeCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(&this->CriticalSection[0]));
    }
};
struct CVtable_012beda4 : CBase_012b5868 {
    CCriticalSectionOwner_012B8FC8 m_lock;
    long          m_count;
    unsigned char m_flag;
    virtual void v00() { }
    CVtable_012beda4();
};
CVtable_012beda4::CVtable_012beda4()
    : CBase_012b5868(), m_lock(), m_count(0), m_flag(0)
{
}
int main()
{
    CVtable_012beda4* o = new CVtable_012beda4();
    if (g_initialised != reinterpret_cast<_RTL_CRITICAL_SECTION*>(&o->m_lock.CriticalSection[0]) || o->m_count != 0 || o->m_flag != 0
        || reinterpret_cast<unsigned char*>(&o->m_lock) - reinterpret_cast<unsigned char*>(o) != 4
        || reinterpret_cast<unsigned char*>(&o->m_count) - reinterpret_cast<unsigned char*>(o) != 0x20) { std::printf("00c6e6a0_TEST FAIL\n"); return 1; }
    std::printf("00c6e6a0_TEST PASS\n");
    return 0;
}
