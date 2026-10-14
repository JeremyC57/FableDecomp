#include <cstdio>
#include <new>
#include <cstring>
struct CBase { virtual void v00() {} CBase() {} };
struct CEventRecord { unsigned char _body[0x2c]; CEventRecord() { std::memset(_body, 0, sizeof(_body)); } };
struct CMaintainedEvent : CBase { CEventRecord m_record; virtual void v00() {} CMaintainedEvent(); };
CMaintainedEvent::CMaintainedEvent() : CBase(), m_record() { }
int main()
{
    unsigned char raw[sizeof(CMaintainedEvent)]; std::memset(raw, 0xEE, sizeof(raw));
    CMaintainedEvent* e = new (raw) CMaintainedEvent;
    for (int i = 0; i < 0x2c; ++i) if (e->m_record._body[i] != 0) { std::printf("009e45b0_TEST FAIL\n"); return 1; }
    std::printf("009e45b0_TEST PASS\n");
    return 0;
}