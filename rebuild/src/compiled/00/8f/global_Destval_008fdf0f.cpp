#include "rebuild_abi.h"
// Leaf: @FableDestVal_008fdf0f@4
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).

// _Dest_val<std::allocator<...>,...> @ 0x008fdf0f
// Destructor forwarder: destroys the embedded member at +0x14, then tail-jumps
// to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();   // 0x008FD288
};
struct FableDestMember_008fdf0f;
struct FableDestHost_008fdf0f
{
    char pad[0x14];
    FableDestMember_008fdf0f* member_at_14;
};


extern "C" void FABLE_FASTCALL FableDestVal_008fdf0f(FableDestHost_008fdf0f* self)
{
    ((CBaseIntelligentPointer*)((FableDestMember_008fdf0f*)&self->member_at_14))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CAIStateGroupBase*)(self))->CAIStateGroupBase::~CAIStateGroupBase();
}