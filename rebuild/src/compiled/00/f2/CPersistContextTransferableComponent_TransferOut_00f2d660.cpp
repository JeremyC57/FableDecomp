// CPersistContext_TransferableComponent<EFeatAttackType>::TransferOut(CPersistContext& context, CCharString& out) -- write the value as a long
//   through CPersistTraitsSLONG::TransferOut
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@W4EFeatAttackType@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F2D660, 30 bytes, thiscall `ret 8`: push ecx ; mov eax,[ecx+4] ; mov ecx,[eax] ; lea edx,[esp] ;
//   mov [esp],ecx ; mov ecx,[esp+8] ; push edx ; mov edx,[esp+10h] ; call 0x009E2560 ; pop ecx ; ret 8 (the value copied into a long
//   local whose address is pushed; CPersistTraitsSLONG::TransferOut is __fastcall: context ecx, out edx).
// Names (NF201, 2026-11-20): FableWin.pdb ?TransferOut@?$CPersistContext_TransferableComponent@W4EFeatAttackType@@@@EAEXAAVCPersistContext@@
//   AAVCCharString@@@Z (the TransferOut / TransferIn instance pair); slot 0 of the RTTI-less component vtable 0x012FB898 whose slot 1 is the
//   landed TransferIn 0x00F2D640 (NF136, same T); vtable stored by CPersistContext::Transfer<EFeatAttackType> 0x00F2D4D0. Callee Ego_r.pdb /
//   FableWin.pdb ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z = 0x009E2560. Was the generated stand-in
//   CTransferableLong_009e2560::TransferOut over the free TransferOut_009e2560.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "engine/CDefClassBase.h"
#include "engine/CFeatDef.h"

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    long value = *Object;
    CPersistTraitsSLONG::TransferOut(context, out, value);
}

template void CPersistContext_TransferableComponent<EFeatAttackType>::TransferOut(CPersistContext& context, CCharString& out);
