// Behaviour test for 0x00905330 (CScriptGameResourceObjectScriptedThingBase copy constructor): the base is copy-constructed,
// the counted pointer copied and its shared count incremented (a null count block is left alone).
#include <cstdio>

struct CCPPointerInfo {
    long m_count;
};

struct CScriptGameResourceObjectScriptedThingBase;

struct CCountedPointer_T {
    CScriptGameResourceObjectScriptedThingBase* Data;
    CCPPointerInfo* Info;
    CCountedPointer_T() : Data(0), Info(0) { }
    CCountedPointer_T(const CCountedPointer_T& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};

static int g_baseCopies = 0;

struct CBaseObject {
    virtual void v00() { }
    virtual ~CBaseObject() { }
    unsigned char _body[4];
    CBaseObject() { for (int i = 0; i < 4; ++i) _body[i] = 0; }
    CBaseObject(const CBaseObject& other) { ++g_baseCopies; for (int i = 0; i < 4; ++i) _body[i] = 0; }
};

struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {
    CCountedPointer_T PImp;
    virtual void v00() { }
    CScriptGameResourceObjectScriptedThingBase() { }
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other);
};

CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other)
    : CBaseObject(other), PImp(other.PImp)
{
}

int main()
{
    CCPPointerInfo info;
    info.m_count = 1;
    CScriptGameResourceObjectScriptedThingBase src;
    src.PImp.Data = (CScriptGameResourceObjectScriptedThingBase*)&info;
    src.PImp.Info = &info;
    CScriptGameResourceObjectScriptedThingBase copy(src);
    if (g_baseCopies != 1 || copy.PImp.Data != src.PImp.Data || copy.PImp.Info != &info || info.m_count != 2) {
        std::printf("00905330_TEST FAIL\n");
        return 1;
    }
    CScriptGameResourceObjectScriptedThingBase empty;
    CScriptGameResourceObjectScriptedThingBase copy2(empty);
    if (g_baseCopies != 2 || copy2.PImp.Data != 0 || copy2.PImp.Info != 0 || info.m_count != 2) {
        std::printf("00905330_TEST FAIL\n");
        return 1;
    }
    std::printf("00905330_TEST PASS\n");
    return 0;
}