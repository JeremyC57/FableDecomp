// behaviour test for 0x00A95FE0 C3DClothPrimitive::C3DClothPrimitive() (model copy of the source; the base ctor/dtor are local stubs)
#include <cstdio>
#include <cstring>
static int g_base = 0;
#include <new>
class CBaseClass {                                       // Ego_r.pdb / RTTI: the polymorphic engine root
public:
    CBaseClass() { ++g_base; }
    virtual ~CBaseClass() { }
};
class CBaseClassNonCopyable : public CBaseClass {        // RTTI base of the class below
protected:
    CBaseClassNonCopyable() { }
    virtual ~CBaseClassNonCopyable() { }
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // Ego_r.pdb: Data, Info
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) { }
};
class CParticleProgram;

class C3DClothPrimitive : public CBaseClassNonCopyable {
public:
    C3DClothPrimitive();
    virtual ~C3DClothPrimitive();
    CCountedPointer<CParticleProgram> ParticleProgram;
    unsigned long PrimitiveIndex;                       // +0x0C (not initialised by the default ctor)
    unsigned long MaterialIndex;                        // +0x10
};
C3DClothPrimitive::C3DClothPrimitive() { }
C3DClothPrimitive::~C3DClothPrimitive() { }
int main()
{
    unsigned char raw[sizeof(C3DClothPrimitive)];
    std::memset(raw, 0xCD, sizeof(raw));
    C3DClothPrimitive* p = new (raw) C3DClothPrimitive;
    int ok = g_base == 1 && p->ParticleProgram.Data == 0 && p->ParticleProgram.Info == 0 && (void*)p == (void*)raw;
    if (!ok) { std::printf("00a95fe0_TEST FAIL\n"); return 1; }
    std::printf("00a95fe0_TEST PASS\n");
    return 0;
}