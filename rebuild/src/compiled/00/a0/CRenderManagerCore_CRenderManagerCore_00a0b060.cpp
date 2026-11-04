// CRenderManagerCore::~CRenderManagerCore() -- core render manager dtor: uninitialise the system font while it is valid, then ~SystemFont and ~CRenderStateManager
// Leaf: ??1CRenderManagerCore@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0B060.
// Identity: Ego_r.pdb ??1CRenderManagerCore@@UAE@XZ; SystemFont is a CSystemFont (overlay FABLE_RETAIL_CRENDERMANAGERCORE_SYSTEMFONT).
// Callees: CSystemFont::Uninitialise 0x00A6C120, ~CSystemFont, ~CInitBaseClass.
// Shape: `if (SystemFont.IsValid()) SystemFont.Uninitialise();`.
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_SYSTEMFONT
#include "engine/CRenderManagerCore.h"

CRenderManagerCore::~CRenderManagerCore()
{
    if (SystemFont.IsValid())
        SystemFont.Uninitialise();
}