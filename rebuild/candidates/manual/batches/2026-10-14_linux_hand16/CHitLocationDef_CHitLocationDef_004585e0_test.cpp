#include <cstdio>
static int g_baseCtors = 0;
struct CParentDefClassBase {
    virtual void v00() {}
    CParentDefClassBase()
    {
        ++g_baseCtors;
        // poison everything the derived constructor is expected to zero afterwards
        unsigned char* p = reinterpret_cast<unsigned char*>(this);
        for (unsigned i = 4; i < 0x44; ++i) p[i] = 0xff;
    }
};
struct CPhysicalPrimitiveInit { long m_type; long m_param; CPhysicalPrimitiveInit() : m_type(0), m_param(0) {} };
struct CHitLocationDef : CParentDefClassBase {
    unsigned char _body[0x38];
    CPhysicalPrimitiveInit PhysicalPrimitive;
    virtual void v00() {}
    CHitLocationDef() : CParentDefClassBase(), PhysicalPrimitive() {}
};
int main()
{
    CHitLocationDef* d = new CHitLocationDef();
    if (g_baseCtors != 1 || d->PhysicalPrimitive.m_type != 0 || d->PhysicalPrimitive.m_param != 0 || d->_body[0] != 0xff) { std::printf("004585e0_TEST FAIL\n"); return 1; }
    if ((unsigned char*)&d->PhysicalPrimitive - (unsigned char*)d != 0x3c) { std::printf("004585e0_TEST FAIL\n"); return 1; }
    delete d;
    std::printf("004585e0_TEST PASS\n");
    return 0;
}
