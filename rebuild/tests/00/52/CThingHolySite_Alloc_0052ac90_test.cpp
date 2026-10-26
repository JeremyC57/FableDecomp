// Behaviour test for 0052ac90: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)


static int g_physicalCtorType = -1;
static bool g_refuse = false;
static bool g_newFlag = true;
static unsigned int g_newSize = 0;
class CClass {
public:
    virtual ~CClass() { }
    virtual int Id() const { return 0; }
};
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }         // stub for 0x00A01B10
    virtual ~CBaseIntelligentPointer() { }
};
struct CThingPointer : CBaseIntelligentPointer {
    CThingPointer() { }
};
class CThing : public CClass {
public:
    static void* operator new(size_t size, bool flag) throw()   // stub for 0x004C7380: null when refused
    {
        g_newSize = (unsigned int)size;
        g_newFlag = flag;
        if (g_refuse) return 0;
        void* p = std::malloc(size);
        std::memset(p, 0xCD, size);
        return p;
    }
    static void operator delete(void* p, bool) { std::free(p); }
    static void operator delete(void* p) { std::free(p); }
};
class CThingPhysical : public CThing {
public:
    long Type;
    CThingPhysical(long type) : Type(type) { g_physicalCtorType = (int)type; }   // stub for 0x005296B0
};
class CThingGameObject : public CThingPhysical {
public:
    CThingPointer PThingStandingOn;
    unsigned char m_gameObjectFlags;
    CThingGameObject(long type) : CThingPhysical(type) { }
};

class CThingHolySiteDef;
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CThingHolySite : public CThingGameObject {
public:
    CDefPointer<const CThingHolySiteDef> Def;
    unsigned char PlayerStart;
    virtual int Id() const { return 7; }
    CThingHolySite() : CThingGameObject(7) { }
    static CClass* Alloc() { return new (false) CThingHolySite; }
};
static void RunTest()
{
    CClass* c = CThingHolySite::Alloc();
    CThingHolySite* h = (CThingHolySite*)c;
    CHECK(c != 0 && c->Id() == 7 && g_physicalCtorType == 7 && !g_newFlag && g_newSize == sizeof(CThingHolySite));
    CHECK(h->Def.Object == 0 && h->PThingStandingOn.m_pItem == 0 && h->Type == 7);
    delete h;
    g_refuse = true;
    g_physicalCtorType = -1;
    CHECK(CThingHolySite::Alloc() == 0 && g_physicalCtorType == -1);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0052ac90_TEST FAIL\n");
        return 1;
    }
    std::printf("0052ac90_TEST PASS\n");
    return 0;
}