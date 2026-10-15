// Behaviour test for 0x007E7490 (CScriptGameResourceObjectScriptedThingBase::GetScriptThing): with an
// implementation the call is forwarded to its virtual; without one an empty CScriptThing (null PImp)
// is returned.
#include <cstdio>

struct CCPPointerInfo {
    long m_count;
};

static int g_baseCtors = 0;

struct CBaseClass {
    virtual void v00() { }
    virtual ~CBaseClass() { }
    CBaseClass() { ++g_baseCtors; }
};

struct CScriptThingImp;
struct CCountedPointer_CScriptThing {
    CScriptThingImp* Data;
    CCPPointerInfo*  Info;
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};

struct CScriptThing : CBaseClass {
    CCountedPointer_CScriptThing PImp;
    virtual void v00() { }
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBaseClass() { PImp.Data = other.PImp.Data; PImp.Info = other.PImp.Info; }
};

static CCPPointerInfo g_info;

struct CScriptedThingBaseVirtuals {
    virtual ~CScriptedThingBaseVirtuals() { }
    virtual CScriptThing GetScriptThing() const
    {
        CScriptThing t;
        t.PImp.Data = (CScriptThingImp*)&g_info;
        t.PImp.Info = &g_info;
        return t;
    }
};

struct CScriptGameResourceObjectScriptedThingBase {
    void* __vftable;
    unsigned char _pad_0x04[4];
    CScriptedThingBaseVirtuals* PImp_Data;
    CCPPointerInfo* PImp_Info;
    CScriptThing GetScriptThing() const;
};

CScriptThing CScriptGameResourceObjectScriptedThingBase::GetScriptThing() const
{
    const CScriptedThingBaseVirtuals* imp = PImp_Data;
    if (imp == 0)
        return CScriptThing();
    return imp->GetScriptThing();
}

int main()
{
    CScriptedThingBaseVirtuals impl;
    CScriptGameResourceObjectScriptedThingBase base;
    base.__vftable = 0; base.PImp_Info = 0;
    base.PImp_Data = &impl;
    {
        CScriptThing t = base.GetScriptThing();
        if (t.PImp.Info != &g_info || t.PImp.Data != (CScriptThingImp*)&g_info) {
            std::printf("007e7490_TEST FAIL\n");
            return 1;
        }
    }
    base.PImp_Data = 0;
    int before = g_baseCtors;
    {
        CScriptThing t = base.GetScriptThing();
        if (t.PImp.Info != 0 || t.PImp.Data != 0 || g_baseCtors <= before) {
            std::printf("007e7490_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("007e7490_TEST PASS\n");
    return 0;
}