// CEngine::CEngine() -- the engine constructor: CIEngine / CDeviceResetCallback bases, the camera and preload state, the video options
// defaults, the 4 KiB-granular frame memory pool and the empty arrays
// Leaf: ??0CEngine@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B260B0, 280 bytes; vtables 0x012A0F3C (CIEngine part) / 0x012A0F30 (CDeviceResetCallback part).
// NF249 (q12692 step 3): on engine/CEngine.h's retail branch with the typed-member mode FABLE_RETAIL_CENGINE_TYPED_MEMBERS and the
//   override set FABLE_RETAIL_CENGINE_OVERRIDES; slot 1 is the inherited ?Validate@CInitBaseClass@@UBEXXZ (CIEngine.h INHERITED_VALIDATE).
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_CTOR
#define FABLE_RETAIL_CIENGINE_INLINE_DTOR
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#define FABLE_RETAIL_CENGINE_TYPED_MEMBERS
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CEngine.h"

CEngine::CEngine() : CurrentCamera(0), FrameMemoryPool(0, 4096)
{
    IsInitialised = false;
}
