// CEngine::~CEngine() -- Uninitialise, then the members in reverse order (the three pointer arrays, the frame memory pool, the def
// pointers, the preload cameras) and the inline ~CIEngine
// Leaf: ??1CEngine@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B261D0, 236 bytes; tail `jmp ~CInitBaseClass` 0x0099A410.
// NF249 (q12692 step 3): on engine/CEngine.h's retail branch with FABLE_RETAIL_CENGINE_TYPED_MEMBERS / _OVERRIDES (the members are typed,
//   so their destructors run in reverse member order), CIEngine.h INLINE_DTOR (retail expands ~CIEngine here and in ??_GCIEngine 0x00B23770).
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

CEngine::~CEngine()
{
    Uninitialise();
}
