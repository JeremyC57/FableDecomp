#pragma optimize("s",on)
#include "rebuild_abi.h"

// COpinionDeedReactionDef::`scalar deleting destructor' @ 0x0047200d
// module: CEngineSubPrimitiveText  (thiscall, void*, 2 params)
//
// Standard MSVC scalar deleting destructor:
//   push esi; mov esi,ecx            ; this = ecx
//   lea ecx,[esi+0x3c]; call <dtor>  ; destruct sub-object/member at this+0x3c
//   mov ecx,esi;       call <dtor>   ; run this object's own destructor
//   test byte[esp+8],1; je skip      ; second param (flags) & 1 ?
//   push esi; call operator delete; pop ecx
// skip:
//   mov eax,esi; pop esi; ret 4      ; return this
//
// The +0x3c member destructor is __fastcall (ecx = this+0x3c); modeled as a
// free __fastcall(objptr). The main destructor takes ecx = this.

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
struct CEngineSubPrimitiveText;
struct COpinionDeedReactionDef_Member;

// sub-object destructor at +0x3c (thiscall)
extern void FABLE_FASTCALL COpinionDeedReactionDef_Member_dtor(
        COpinionDeedReactionDef_Member* thisptr);

// this object's own (non-deleting) destructor (thiscall)


void* FABLE_FASTCALL COpinionDeedReactionDef_scalar_deleting_dtor(
        CEngineSubPrimitiveText* thisptr,
        int /*edx*/,
        unsigned int flags)
{
    COpinionDeedReactionDef_Member_dtor(
        (COpinionDeedReactionDef_Member*)((fable_u8*)thisptr + 0x3c));
    ((CParentDefClassBase*)(thisptr))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(thisptr);
    return thisptr;
}