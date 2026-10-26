#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
#include <string.h>
#include <new>
class CThingBaseDef { public: virtual void v00() { } long m_base; CThingBaseDef() : m_base(1) { } };
struct SBuildingRecord {
    long m_value; float m_levelA; float m_levelB; bool m_flagA; bool m_flagB;
    SBuildingRecord() : m_value(0), m_levelA(0.0f), m_levelB(0.0f), m_flagA(false), m_flagB(false) { }
};
class CThingBuildingDef : public CThingBaseDef { public:
    long            m_uninit[2];
    SBuildingRecord m_record;
    virtual void v00() { }
    CThingBuildingDef() { }
};
int main()
{
    unsigned char raw[sizeof(CThingBuildingDef)]; memset(raw, 0xcd, sizeof(raw));
    CThingBuildingDef* d = new (raw) CThingBuildingDef;
    if (d->m_base != 1 || d->m_record.m_value != 0 || d->m_record.m_levelA != 0.0f || d->m_record.m_levelB != 0.0f || d->m_record.m_flagA || d->m_record.m_flagB) { std::printf("00454b5f_TEST FAIL\n"); return 1; }
    std::printf("00454b5f_TEST PASS\n");
    return 0;
}