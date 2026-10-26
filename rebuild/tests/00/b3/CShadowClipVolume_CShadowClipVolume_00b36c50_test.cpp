// Behaviour test for CShadowClipVolume::CShadowClipVolume(const CShadowClipVolume&) @ 0x00B36C50 (implicit member-wise copy)
#include <cstdio>
#include <new>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CPlane { public: C3DVector Normal; float Distance; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class CShadowClipVolume { public:
    CPlane            ClipPlanes[8];
    long              ClipPlaneCount;
    C3DBoundingSphere BoundingSphere;
    bool              ClipToSphere;
};
int main()
{
    CShadowClipVolume a;
    int i;
    for (i = 0; i < 8; ++i) {
        a.ClipPlanes[i].Normal.X = (float)i; a.ClipPlanes[i].Normal.Y = (float)(i * 2);
        a.ClipPlanes[i].Normal.Z = (float)(i * 3); a.ClipPlanes[i].Distance = (float)(-i);
    }
    a.ClipPlaneCount = 6;
    a.BoundingSphere.Center.X = 1.5f; a.BoundingSphere.Center.Y = 2.5f; a.BoundingSphere.Center.Z = 3.5f;
    a.BoundingSphere.Radius = 9.0f;
    a.ClipToSphere = true;
    static unsigned char storage[sizeof(CShadowClipVolume)];
    CShadowClipVolume* b = new (storage) CShadowClipVolume(a);
    for (i = 0; i < 8; ++i) {
        if (b->ClipPlanes[i].Normal.X != (float)i || b->ClipPlanes[i].Normal.Y != (float)(i * 2) ||
            b->ClipPlanes[i].Normal.Z != (float)(i * 3) || b->ClipPlanes[i].Distance != (float)(-i)) {
            std::printf("00b36c50_TEST FAIL\n"); return 1;
        }
    }
    if (b->ClipPlaneCount != 6 || b->BoundingSphere.Center.Y != 2.5f || b->BoundingSphere.Radius != 9.0f || !b->ClipToSphere) {
        std::printf("00b36c50_TEST FAIL\n"); return 1;
    }
    std::printf("00b36c50_TEST PASS\n");
    return 0;
}