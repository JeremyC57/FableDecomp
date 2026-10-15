#include <cstdio>
#include <cstring>
#include <new>
static int g_sectionInits = 0; static void* g_section = 0;
static void InitializeCriticalSectionStub(void* section) { ++g_sectionInits; g_section = section; }
struct CHandle_00c5eca0 { virtual void v00() { } void* m_hHandle; CHandle_00c5eca0() : m_hHandle((void*)-1) { } };
struct CHandleEx_00c5eca0 : CHandle_00c5eca0 { virtual void v04() { } CHandleEx_00c5eca0() { } };
struct CEvent_00c5eca0 { virtual void v00() { } void* m_hHandle; CEvent_00c5eca0() : m_hHandle(0) { } };
struct CMutex_00c5eca0 { virtual void v00() { } void* m_hHandle; CMutex_00c5eca0() : m_hHandle(0) { } };
struct CSection_00c5eca0 { virtual void v00() { } unsigned char m_section[0x18]; CSection_00c5eca0() { InitializeCriticalSectionStub(m_section); } };
struct CWoundBase_00c5eca0 { virtual void v00() { } };
struct CWound_00c5eca0 : CWoundBase_00c5eca0 {
    CHandle_00c5eca0 m_4; CHandleEx_00c5eca0 m_c;
    long m_14, m_18, m_1c, m_20;
    unsigned char _24[8];
    CEvent_00c5eca0 m_2c; CMutex_00c5eca0 m_34;
    unsigned char _3c[4];
    CSection_00c5eca0 m_40;
    virtual void v00() { }
    CWound_00c5eca0() { m_1c = 0; m_14 = 0; m_18 = 0; m_20 = 0; }
};
int main()
{
    static unsigned char buf[sizeof(CWound_00c5eca0)];
    std::memset(buf, 0xCD, sizeof(buf));
    CWound_00c5eca0* w = new (buf) CWound_00c5eca0();
    bool ok = w->m_4.m_hHandle == (void*)-1 && w->m_c.m_hHandle == (void*)-1 && w->m_2c.m_hHandle == 0
        && w->m_34.m_hHandle == 0 && g_sectionInits == 1 && g_section == w->m_40.m_section
        && w->m_14 == 0 && w->m_18 == 0 && w->m_1c == 0 && w->m_20 == 0;
    if (!ok) { std::printf("00c5ed90_TEST FAIL\n"); return 1; }
    std::printf("00c5ed90_TEST PASS\n");
    return 0;
}
