#include <cstdio>
struct _RTL_CRITICAL_SECTION;
static _RTL_CRITICAL_SECTION* g_deleted = 0;
extern "C" void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* section) { g_deleted = section; }
struct CBase_012b5868 { virtual void v00() { } ~CBase_012b5868() { } };
struct CCriticalSectionOwner_012B8FC8 {
    virtual void v00() { }
    unsigned char CriticalSection[0x18];
    ~CCriticalSectionOwner_012B8FC8()
    {
        DeleteCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(&this->CriticalSection[0]));
    }
};
struct CVtable_012beda4 : CBase_012b5868 {
    CCriticalSectionOwner_012B8FC8 m_lock;
    long          m_count;
    unsigned char m_flag;
    virtual void v00() { }
    ~CVtable_012beda4();
};
CVtable_012beda4::~CVtable_012beda4()
{
}
int main()
{
    _RTL_CRITICAL_SECTION* expected;
    {
        CVtable_012beda4 o;
        expected = reinterpret_cast<_RTL_CRITICAL_SECTION*>(&o.m_lock.CriticalSection[0]);
    }
    if (g_deleted != expected) { std::printf("00c6e630_TEST FAIL\n"); return 1; }
    std::printf("00c6e630_TEST PASS\n");
    return 0;
}
