#pragma optimize("t", on)
// Leaf: ?CEngineSubPrimitiveText_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF48 (round 7): the +0x60 member is an STLport 4.6.2 std::vector<CScriptThing> (shared stlport/fable_containers.h, included first so
// the overlay opt-in applies; engine/CScriptThing.h for the element); its destructor is ~vector<CScriptThing> 0x008AC970, called out of
// line (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE) where a free __fastcall stand-in named it before.
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "stlport/fable_containers.h"
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<std::vector<CScriptThing>*>(static_cast<unsigned char*>(self) + 96)->std::vector<CScriptThing>::~vector();
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}
