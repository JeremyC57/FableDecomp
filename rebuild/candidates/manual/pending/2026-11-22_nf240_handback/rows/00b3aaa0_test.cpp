// Test for 0x00B3AAA0 CEngineInternalPrimitiveBase::WasRendered on the shared engine/CEngineInternalPrimitiveBase.h (NF240): links the
// row object. The primitive is a raw buffer (LastFrameRendered at +0x36); the private static CurrentRenderFrame (0x0143B8D8) is
// defined here. WasRendered is protected, so a never-constructed derived probe makes a qualified (non-virtual) call: no vtable.
// Cases: equal frame (true), a different frame (false), neighbouring bytes do not leak in (only the 16-bit word at +0x36 is
// compared), and SetAsRendered (the header's public inline) makes it true.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_SCENEGRID_VISIBILITY_PASS27
#include "stlport/fable_map.h"
#include "engine/CEngineInternalPrimitiveBase.h"

unsigned short CEngineInternalPrimitiveBase::CurrentRenderFrame = 0x1234;

struct WasRenderedProbe : CEngineInternalPrimitiveBase {
    static bool Call(const CEngineInternalPrimitiveBase* p)
    { return static_cast<const WasRenderedProbe*>(p)->CEngineInternalPrimitiveBase::WasRendered(); }
};

int main()
{
    static unsigned char buf[0x48];
    CEngineInternalPrimitiveBase* p = reinterpret_cast<CEngineInternalPrimitiveBase*>(buf);
    buf[0x36] = 0x34; buf[0x37] = 0x12;
    if (!WasRenderedProbe::Call(p)) { printf("00b3aaa0_TEST FAIL 1\n"); return 1; }
    buf[0x36] = 0x35;
    if (WasRenderedProbe::Call(p)) { printf("00b3aaa0_TEST FAIL 2\n"); return 1; }
    memset(buf, 0xff, sizeof(buf)); buf[0x36] = 0x34; buf[0x37] = 0x12;
    if (!WasRenderedProbe::Call(p)) { printf("00b3aaa0_TEST FAIL 3\n"); return 1; }
    buf[0x36] = 0; buf[0x37] = 0;
    p->SetAsRendered();
    if (!WasRenderedProbe::Call(p) || buf[0x36] != 0x34 || buf[0x37] != 0x12) { printf("00b3aaa0_TEST FAIL 4\n"); return 1; }
    printf("00b3aaa0_TEST PASS\n");
    return 0;
}
