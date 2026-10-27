// behaviour test for 0x007E72A0 CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase() (model copy of the source; the base ctor/dtor are local stubs)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __declspec(x)
#endif
static int g_base = 0;
#include <new>
class CBaseClass {                                       // Ego_r.pdb / RTTI: the polymorphic engine root
public:
    CBaseClass() { ++g_base; }
    virtual ~CBaseClass() { }
};
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {                  // Ego_r.pdb, 8 bytes
protected:
    CBaseObject() : IntelligentPointer(0) { }
    virtual ~CBaseObject() { }
    CBaseObjectPointer* IntelligentPointer;              // +0x04
};
class __declspec(novtable) CScriptGameResourceObjectBase : public CBaseObject {   // RTTI base; its inline ctor's vtable store is dead
protected:
    CScriptGameResourceObjectBase() { }
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject {
public:
    CScriptGameResourceObjectScriptedThingBase();
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;
};
CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase() { }
CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase() { }
int main()
{
    unsigned char raw[sizeof(CScriptGameResourceObjectScriptedThingBase)];
    std::memset(raw, 0xCD, sizeof(raw));
    CScriptGameResourceObjectScriptedThingBase* p = new (raw) CScriptGameResourceObjectScriptedThingBase;
    int ok = g_base == 1 && p->PImp.Data == 0 && p->PImp.Info == 0 && (void*)p == (void*)raw;
    if (!ok) { std::printf("007e72a0_TEST FAIL\n"); return 1; }
    std::printf("007e72a0_TEST PASS\n");
    return 0;
}