// Behaviour test for 00f29d00: body copied from src/00f29d00.cpp (CCutsceneDef ctor), the CResource base ctor stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_resourceCtor = 0;
#pragma pack(push, 1)
struct CResource {
    virtual void v00() {}
    unsigned char _body[0x18];
    CResource() { ++g_resourceCtor; memset(_body, 0x77, sizeof(_body)); }
};
struct CDefinitionManager;
struct __declspec(novtable) CDefClassBase : CResource {
    CDefinitionManager* PDefManager;
    unsigned long GlobalIndex;
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
    CDefClassBase() : CResource(), PDefManager(0), Setup(0), Template(0), DefaultValsApplied(0) { }
};
struct CCharString;
template <class T> struct CAllocProxy { T* p; CAllocProxy(T* v) : p(v) { } };
struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CAllocProxy<CCharString> _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CDefIndex { long Index; CDefIndex() : Index(-1) { } };
struct __declspec(novtable) CCutsceneDefBase : CDefClassBase {
    CDefIndex     m_index;
    long          m_value29;
    long          m_value2d;
    long          m_value31;
    unsigned char m_byte35;
    unsigned char m_flag36;
    unsigned char _pad37[5];
    CCutsceneDefBase() : m_value29(0), m_value2d(0), m_value31(0), m_flag36(0) { }
};
struct CCutsceneDef : CCutsceneDefBase {
    CCharStringVector Macro, SkipCond, Lights, LightScene, SetupCond, Sound, Answer0, Answer1;
    virtual void v00() {}
    CCutsceneDef();
};
#pragma pack(pop)
CCutsceneDef::CCutsceneDef()
{
}
int main()
{
    unsigned char raw[sizeof(CCutsceneDef)]; memset(raw, 0xAA, sizeof(raw));
    CCutsceneDef* def = new (raw) CCutsceneDef();
    CHECK(sizeof(CCutsceneDef) == 0x9c);
    CHECK(g_resourceCtor == 1 && def->_body[0] == 0x77);
    CHECK(def->PDefManager == 0 && def->GlobalIndex == 0xAAAAAAAA);
    CHECK(def->Setup == 0 && def->Template == 0 && def->DefaultValsApplied == 0);
    CHECK((*((unsigned char*)def + 0x24) & 0xF8) == 0xA8);           // the other five bits of the flag byte are kept
    CHECK(def->m_index.Index == -1 && def->m_value29 == 0 && def->m_value2d == 0 && def->m_value31 == 0);
    CHECK(def->m_byte35 == 0xAA && def->m_flag36 == 0);
    CCharStringVector* v = &def->Macro;
    for (int i = 0; i < 8; ++i) CHECK(v[i]._M_start == 0 && v[i]._M_finish == 0 && v[i]._M_end_of_storage.p == 0);
    if (fails == 0) printf("00f29d00_TEST PASS\n"); else printf("00f29d00_TEST FAIL\n");
    return fails;
}