// test for 00431e40 CFrontEndDef scalar deleting destructor
#include <stdio.h>
#include <new>
class CCharString {
public:
    void* PStringData;
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                           // 0x00414EA0 for CCharString
};
}
class CParentDefClassBase {
public:
    unsigned char _body[0x35];                           // +0x04 .. +0x39 (CResource, CDefClassBase, InstantiationName, SubDefInfoMap)
    CParentDefClassBase();
    virtual ~CParentDefClassBase();                      // 0x00430300
};
class CFrontEndDef : public CParentDefClassBase {
public:
    std::vector<CCharString> vAttractModeMovie;          // +0x3c
    long ErrorMessageBackgroundGraphic;                  // +0x48
    long ButtonABigGraphic;                              // +0x4c
    long ButtonBBigGraphic;                              // +0x50
    virtual ~CFrontEndDef() {}
};
void ConstructCFrontEndDef(void* p)                      // emission helper, not a retail function: the vtable (and its ??_G) is emitted
{
    new (p) CFrontEndDef;
}

static int g_order, g_vec, g_base; static void* g_vecthis;
namespace std { template <class T> vector<T>::~vector() { g_vec = ++g_order; g_vecthis = this; } }
CParentDefClassBase::CParentDefClassBase() {}
CParentDefClassBase::~CParentDefClassBase() { g_base = ++g_order; }
int main()
{
    static double buf[0x20];
    ConstructCFrontEndDef(buf);
    CFrontEndDef* p = (CFrontEndDef*)buf;
    int ok = (char*)&p->vAttractModeMovie - (char*)p == 0x3c;
    p->~CFrontEndDef();
    ok = ok && g_vec == 1 && g_base == 2 && g_vecthis == (char*)buf + 0x3c;
    CParentDefClassBase* h = new CFrontEndDef;
    delete h;
    ok = ok && g_vec == 3 && g_base == 4;
    printf(ok ? "00431e40_TEST PASS\n" : "00431e40_TEST FAIL\n");
    return 0;
}