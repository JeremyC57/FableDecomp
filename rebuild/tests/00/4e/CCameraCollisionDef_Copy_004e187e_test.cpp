// NF335 (NF324 Q2): the row now defines the PDB member CCameraCollisionDef::Copy (?Copy@CCameraCollisionDef@@UAEXPBVCDefClassBase@@@Z),
// declared by the shared header's retail model (FABLE_RETAIL_CCAMERACOLLISIONDEF_VIRTUAL_MODEL on FABLE_RETAIL_DEF_CLASS_BASE, the
// row's own switches). The test calls it qualified through references to that model bound to zero-initialised static storage: the
// model is never constructed, so the test emits no vftable and no copy of the row's symbol.
#define FABLE_RETAIL_CCAMERACOLLISIONDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCameraCollisionDef.h"
#include "engine/CDefClassBase.h"
#include <cstdio>


static int g_called = 0;
extern "C" void base_copy_called() { g_called = 1; }


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    void* self = (void*)this; (void)self;
    void* edx = 0; (void)edx;
    const CDefClassBase* src = (const CDefClassBase*)&other_; (void)src;
    base_copy_called();
    return *this;
}

static unsigned long g_aStorage[(sizeof(CCameraCollisionDef) + sizeof(unsigned long) - 1) / sizeof(unsigned long)];
static unsigned long g_bStorage[(sizeof(CCameraCollisionDef) + sizeof(unsigned long) - 1) / sizeof(unsigned long)];

int main() {
    CCameraCollisionDef& a = *(CCameraCollisionDef*)g_aStorage;
    CCameraCollisionDef& b = *(CCameraCollisionDef*)g_bStorage;
    b.CameraCollisionMesh = 0xDEADBEEF;
    a.CameraCollisionMesh = 0;
    a.CCameraCollisionDef::Copy((const CDefClassBase*)&b);
    if (a.CameraCollisionMesh == 0xDEADBEEF && g_called == 1) {
        printf("004e187e_TEST PASS\n");
    } else {
        printf("FAIL a=%08lx called=%d\n", a.CameraCollisionMesh, g_called);
    }
    return 0;
}
