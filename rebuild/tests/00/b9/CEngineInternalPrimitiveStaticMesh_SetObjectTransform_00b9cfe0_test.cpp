#include <stdio.h>

class CMatrix3x4 { public: float m[12]; };

class CEngineInternalPrimitiveStaticMesh { public:
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall SetObjectTransform(CEngineInternalPrimitiveStaticMesh* mesh, int /*edx*/, const CMatrix3x4& mat)
{
    mesh->transform = mat;
    return true;
}

int main()
{
    CEngineInternalPrimitiveStaticMesh obj;
    CMatrix3x4 src;
    for (int i = 0; i < 12; ++i) src.m[i] = (float)(i * 3 + 1);

    bool r = SetObjectTransform(&obj, 0, src);

    bool ok = r;
    for (int i = 0; i < 12; ++i)
        if (obj.transform.m[i] != (float)(i * 3 + 1)) ok = false;

    if (ok) printf("00b9cfe0_TEST PASS\n");
    else    printf("FAIL\n");
    return 0;
}