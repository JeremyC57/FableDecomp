// Test for 0x00AC3B80 C3DClothPrimitive::~C3DClothPrimitive: the ParticleProgram counted pointer drops its reference (DeleteFunc +
// info freed at zero) and is cleared, then ~CBaseClassNonCopyable.
#include <cstdio>
static int g_deleted = 0, g_baseDtors = 0;
class CCPPointerInfo { public:
    unsigned int RefCount; void (*DeleteFunc)(void*); void* Data;
};
static void DeleteIt(void*) { ++g_deleted; }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CBaseClassNonCopyable : public CBaseClass { public: virtual ~CBaseClassNonCopyable() { ++g_baseDtors; } };
class CParticleProgram;
class C3DClothPrimitive : public CBaseClassNonCopyable { public:
    virtual ~C3DClothPrimitive();
    CCountedPointer<CParticleProgram> ParticleProgram;
    unsigned int PrimitiveIndex, MaterialIndex;
};
C3DClothPrimitive::~C3DClothPrimitive() { }
int main()
{
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteIt; info->Data = 0;
    { C3DClothPrimitive a; a.ParticleProgram.Data = 0; a.ParticleProgram.Info = info; }
    if (g_deleted != 0 || info->RefCount != 1 || g_baseDtors != 1) { std::printf("00ac3b80_TEST FAIL\n"); return 1; }
    { C3DClothPrimitive b; b.ParticleProgram.Data = 0; b.ParticleProgram.Info = info; }
    if (g_deleted != 1 || g_baseDtors != 2) { std::printf("00ac3b80_TEST FAIL\n"); return 1; }
    { C3DClothPrimitive c; c.ParticleProgram.Data = 0; c.ParticleProgram.Info = 0; }
    if (g_deleted != 1 || g_baseDtors != 3) { std::printf("00ac3b80_TEST FAIL\n"); return 1; }
    std::printf("00ac3b80_TEST PASS\n");
    return 0;
}