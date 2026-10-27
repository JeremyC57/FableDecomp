// behaviour test for 0x006E7B40 CScriptThing::CScriptThing() (model copy of the source; the base ctor/dtor are local stubs)
#include <cstdio>
#include <cstring>
static int g_base = 0;
#include <new>
class CBaseClass {                                       // Ego_r.pdb / RTTI: the polymorphic engine root
public:
    CBaseClass() { ++g_base; }
    virtual ~CBaseClass() { }
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};

class CScriptThing : public CBaseClass {
public:
    CScriptThing();
    virtual ~CScriptThing();
    CCountedPointer<CScriptThing> PImp;
};
CScriptThing::CScriptThing() { }
CScriptThing::~CScriptThing() { }
int main()
{
    unsigned char raw[sizeof(CScriptThing)];
    std::memset(raw, 0xCD, sizeof(raw));
    CScriptThing* p = new (raw) CScriptThing;
    int ok = g_base == 1 && p->PImp.Data == 0 && p->PImp.Info == 0 && (void*)p == (void*)raw;
    if (!ok) { std::printf("006e7b40_TEST FAIL\n"); return 1; }
    std::printf("006e7b40_TEST PASS\n");
    return 0;
}