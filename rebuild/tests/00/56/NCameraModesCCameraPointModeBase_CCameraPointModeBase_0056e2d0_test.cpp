#include <cstdio>
// NCameraModes::CCameraPointModeBase::~CCameraPointModeBase(): own vtable, then the pointer member, then the base
static int g_seq, g_member, g_base;
static void* g_member_at = 0;
class CBaseObjectPointer;
class __declspec(novtable) CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
private:
    CBaseObjectPointer* PData;
};
CBaseIntelligentPointer::~CBaseIntelligentPointer() { g_member = ++g_seq; g_member_at = this; }
namespace NCameraModes {
class __declspec(novtable) CBase {
public:
    virtual ~CBase();
private:
    unsigned char _pad_0x04[0xa0];
};
CBase::~CBase() { g_base = ++g_seq; }
class CCameraPointModeBase : public CBase {
public:
    static void Destroy(CCameraPointModeBase* p) { p->CCameraPointModeBase::~CCameraPointModeBase(); }
protected:
    virtual ~CCameraPointModeBase();
    CBaseIntelligentPointer PCameraPoint;
};
}
int main(){
    void* buf[0x40] = {0};
    NCameraModes::CCameraPointModeBase::Destroy((NCameraModes::CCameraPointModeBase*)buf);
    bool ok = buf[0] != 0 && g_member == 1 && g_base == 2 && g_member_at == (void*)((char*)buf + 0xa4);
    if(ok) printf("DTOR_CHAIN_OK\n");
    else   printf("DTOR_CHAIN_FAIL m=%d b=%d\n", g_member, g_base);
    return ok?0:1;
}
