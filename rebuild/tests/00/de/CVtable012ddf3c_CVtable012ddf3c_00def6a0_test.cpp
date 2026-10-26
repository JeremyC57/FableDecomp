// Behaviour test for 00def6a0: body copied from src/00def6a0.cpp (CVtable_012ddf3c::CVtable_012ddf3c), the CBase and CScriptThing copy constructors stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCtor = 0, g_copyCtor = 0;
namespace N_0099a2d0 { struct CBase {
    CBase() { ++g_baseCtor; }
    virtual void v00() {}
}; } using N_0099a2d0::CBase;
struct CScriptThingRef {
    void* m_pObject;
    void* m_pCount;
    CScriptThingRef() : m_pObject(0), m_pCount(0) { }
};
class CScriptThing : public CBase { public:
    CScriptThingRef m_ref;
    virtual void v00() {}
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) { ++g_copyCtor; m_ref = other.m_ref; }
};
struct CScript {
    unsigned char _pad[0x40];
    void* m_pContext;
};
struct CVtable_012c3224_Root {
    virtual void v00() {}
    void* m_pContext;
    CVtable_012c3224_Root(void* context) : m_pContext(context) { }
};
struct CVtable_012c3224 : CVtable_012c3224_Root {
    virtual void v00() {}
    CScriptThing m_thing;
    CVtable_012c3224(CScript* owner, const CScriptThing& thing) : CVtable_012c3224_Root(owner->m_pContext), m_thing(thing) { }
};
template<class T> struct CArrayAllocProxy {
    T* p;
    CArrayAllocProxy(T* v) : p(v) { }
};
struct CStateWords {
    long*                  _M_start;
    long*                  _M_finish;
    CArrayAllocProxy<long> _M_end_of_storage;
    CStateWords() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CVtable_012ddf3c : CVtable_012c3224 {
    virtual void v00() {}
    CScript* m_pOwner;
    void* m_pArgument;
    CStateWords m_state;
    CScriptThing m_target;
    CVtable_012ddf3c(CScript* owner, void* argument, const CScriptThing& thing);
};
CVtable_012ddf3c::CVtable_012ddf3c(CScript* owner, void* argument, const CScriptThing& thing)
    : CVtable_012c3224(owner, thing), m_pOwner(owner)
{
    m_pArgument = argument;
}
int main()
{
    CScript script; script.m_pContext = (void*)0x1111;
    CScriptThing thing; thing.m_ref.m_pObject = (void*)0x2222; thing.m_ref.m_pCount = (void*)0x3333;
    int arg = 0;
    unsigned char raw[sizeof(CVtable_012ddf3c)]; memset(raw, 0xAA, sizeof(raw));
    CVtable_012ddf3c* p = new (raw) CVtable_012ddf3c(&script, &arg, thing);
    CHECK(p->m_pContext == (void*)0x1111);
    CHECK(g_copyCtor == 1); CHECK(p->m_thing.m_ref.m_pObject == (void*)0x2222); CHECK(p->m_thing.m_ref.m_pCount == (void*)0x3333);
    CHECK(p->m_pOwner == &script); CHECK(p->m_pArgument == &arg);
    CHECK(p->m_state._M_start == 0); CHECK(p->m_state._M_finish == 0); CHECK(p->m_state._M_end_of_storage.p == 0);
    CHECK(g_baseCtor >= 2); CHECK(p->m_target.m_ref.m_pObject == 0); CHECK(p->m_target.m_ref.m_pCount == 0);
    if (fails == 0) printf("00def6a0_TEST PASS\n"); else printf("00def6a0_TEST FAIL\n");
    return fails;
}