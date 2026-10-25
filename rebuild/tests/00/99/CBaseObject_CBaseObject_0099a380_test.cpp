// behaviour test for 0x0099A380 CBaseObject::CBaseObject(), linked against the built constructor: the back-link is
// cleared, the vtable installed (virtual destructor dispatch reaches CBaseObject's), this returned
#include <cstdio>
#include <cstring>
#include <new>
static int g_dtors = 0;
int main();
class CBaseObjectPointer;
class CBaseObject {
    friend int ::main();
protected:
    CBaseObject();
    virtual ~CBaseObject();
public:
    CBaseObjectPointer* IntelligentPointer;
};
CBaseObject::~CBaseObject() { ++g_dtors; }
int main()
{
    if (sizeof(CBaseObject) != 8) { std::printf("0099a380_TEST FAIL size\n"); return 1; }
    unsigned char buf[sizeof(CBaseObject)];
    std::memset(buf, 0xCD, sizeof(buf));
    CBaseObject* p = new (buf) CBaseObject();
    bool ok = (void*)p == (void*)buf && p->IntelligentPointer == 0 && *(unsigned long*)buf != 0xCDCDCDCDu;
    p->~CBaseObject();
    ok = ok && g_dtors == 1;
    if (!ok) { std::printf("0099a380_TEST FAIL\n"); return 1; }
    std::printf("0099a380_TEST PASS\n");
    return 0;
}
