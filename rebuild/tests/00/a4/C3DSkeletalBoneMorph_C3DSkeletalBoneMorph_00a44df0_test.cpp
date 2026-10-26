// Behaviour test for 0x00A44DF0: C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(name, scaling) (copies, then caches the name CRC)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length)
    {
        const unsigned char* p = (const unsigned char*)data;
        unsigned long h = seed ^ 2166136261u;
        for (unsigned long i = 0; i < length; ++i)
            h = (h ^ p[i]) * 16777619u;
        return h & 0xFFFFFFFFu;
    }
};
class CCharStringData { public:
    const char* Data;
    long        Length;
    long        RefCount;
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(CCharStringData* d) : PStringData(d) { if (d) ++d->RefCount; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { if (PStringData) ++PStringData->RefCount; }
    ~CCharString() { if (PStringData) --PStringData->RefCount; }
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->Data; }
    long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
    unsigned long GetCRC() const { return CCRC::Calc(0, c_str(), Length()); }
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;
    unsigned long BoneNameCRC;
    C3DVector     Scaling;
    C3DSkeletalBoneMorph(const CCharString& boneName, const C3DVector& scaling);
};
C3DSkeletalBoneMorph::C3DSkeletalBoneMorph(const CCharString& boneName, const C3DVector& scaling)
    : BoneName(boneName), Scaling(scaling)
{
    BoneNameCRC = boneName.GetCRC();
}

int main()
{
    CCharStringData data;
    data.Data = "Bip01 Head";
    data.Length = 10;
    data.RefCount = 0;
    {
        CCharString name(&data);
        C3DSkeletalBoneMorph m(name, C3DVector(1.5f, 2.0f, -3.0f));
        Check(m.BoneName.PStringData == &data && data.RefCount == 2);
        Check(m.BoneNameCRC == CCRC::Calc(0, "Bip01 Head", 10) && m.BoneNameCRC != CCRC::Calc(0, "", 0));
        Check(Near(m.Scaling.X, 1.5f) && Near(m.Scaling.Y, 2.0f) && Near(m.Scaling.Z, -3.0f));
        CCharString empty((CCharStringData*)0);
        C3DSkeletalBoneMorph e(empty, C3DVector(0, 0, 0));
        Check(e.BoneName.PStringData == 0 && e.BoneNameCRC == CCRC::Calc(0, "", 0));
    }
    Check(data.RefCount == 0);
    if (g_fail) {
        printf("00a44df0_TEST FAIL\n");
        return 1;
    }
    printf("00a44df0_TEST PASS\n");
    return 0;
}