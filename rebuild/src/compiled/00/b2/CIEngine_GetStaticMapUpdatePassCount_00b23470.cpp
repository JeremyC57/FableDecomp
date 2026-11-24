// Leaf: ?GetStaticMapUpdatePassCount@CIEngine@@UAEJXZ
// CIEngine::GetStaticMapUpdatePassCount @ 0x00b23470
// Retail: 33 c0 c3  ->  xor eax,eax ; ret
// Virtual this-only accessor returning long 0. Modeled as __fastcall so the
// object pointer arrives in ECX (byte-identical to __fastcall for a this-only
// method). VC7.1 /O2 emits "xor eax,eax; ret" for "return 0;".
// Switch route (flatten pilot --switch): the class model is engine/CIEngine.h under FABLE_RETAIL_CIENGINE; preamble copied from the landed precedent CIEngine_AddCustomMorphTexture_00b23460.cpp.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"

long CIEngine::GetStaticMapUpdatePassCount()
{
    CIEngine* thisptr = this;
    (void)thisptr;
    return 0;
}
