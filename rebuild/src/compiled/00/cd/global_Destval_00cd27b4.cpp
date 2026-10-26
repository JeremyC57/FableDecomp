#include "rebuild_abi.h"
// Leaf: ??1CPair@

// _Dest_val<std::allocator<...>,...> @ 0x00cd27b4
// Destructor forwarder: destroys the embedded member at +0x4, then
// tail-jumps to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x4]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
class CScriptGameResourceObjectScriptedThingBase {
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
};
struct FableDestMember_00cd27b4;
struct FableDestHost_00cd27b4;

extern "C" void FABLE_FASTCALL FableBaseDtor_00cd27b4(FableDestHost_00cd27b4* self);

extern "C" void FABLE_FASTCALL FableDestVal_00cd27b4(FableDestHost_00cd27b4* self)
{
    ((CScriptGameResourceObjectScriptedThingBase*)((FableDestMember_00cd27b4*)((char*)self + 0x4)))->CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase();
    FableBaseDtor_00cd27b4(self);
}