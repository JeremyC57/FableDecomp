#include "engine/CCameraCollisionDef.h"
#include <cstdio>
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};


static int g_called = 0;
extern "C" void base_copy_called() { g_called = 1; }


void __fastcall CCameraCollisionDef_Copy(CCameraCollisionDef* self, void* edx, const CDefClassBase* src);

CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    void* self = (void*)this; (void)self;
    void* edx = 0; (void)edx;
    const CDefClassBase* src = (const CDefClassBase*)&other_; (void)src;
    base_copy_called();
    return *this;
}

int main() {
    CCameraCollisionDef a, b;
    b.CameraCollisionMesh = 0xDEADBEEF;
    a.CameraCollisionMesh = 0;
    CCameraCollisionDef_Copy(&a, 0, (const CDefClassBase*)&b);
    if (a.CameraCollisionMesh == 0xDEADBEEF && g_called == 1) {
        printf("004e187e_TEST PASS\n");
    } else {
        printf("FAIL a=%08lx called=%d\n", a.CameraCollisionMesh, g_called);
    }
    return 0;
}