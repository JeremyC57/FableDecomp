#pragma optimize("t", on)
// Leaf: ?CEngineSubPrimitiveText_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Naming wave 28 (family esb): the base destructor is ??1CEntityScriptBase@@UAE@XZ 0x00F35B40 (chunk 696: stores vtable 0x012C3224, the
//   engine/CEntityScriptBase.h class; Ego_r.pdb + FableWin.pdb), called qualified. Was the free
//   __fastcall ?AutoTinyCompositeDestructor_00f35b40@@YIXPAX@Z stand-in.
// Name fix (agentNF54): the +28 member's destructor 0x00412130 is STLport ~vector<CWideString> (Ego_r + FableWin
//   ??1?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAE@XZ; landed row 0x00412130), reached through the shared
//   stlport/fable_containers.h + fable_string.h; was the free __fastcall AutoTinyMemberDestructor_00412130 stand-in.
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CEntityScriptBase { public: virtual ~CEntityScriptBase(); };   // 0x00F35B40 (vtable 0x012C3224)
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<std::vector<CWideString>*>(static_cast<unsigned char*>(self) + 28)->~vector();
    ((CEntityScriptBase*)(self))->CEntityScriptBase::~CEntityScriptBase();
    if (flags & 1)
        operator delete(self);
    return self;
}