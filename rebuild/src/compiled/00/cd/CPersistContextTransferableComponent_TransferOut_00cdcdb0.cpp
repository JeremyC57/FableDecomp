// CPersistContext_TransferableComponent<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>::TransferOut(CPersistContext&, CCharString&) -- write the enum as a long through CPersistTraitsSLONG::TransferOut
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@W4ArcheryOpenState@CQ_SunnyvaleMasterData@NScript@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDCDB0, 30 bytes, thiscall `ret 8`:
//   push ecx ; mov eax,[ecx+0x4] ; mov ecx,[eax] ; lea edx,[esp] ; mov [esp],ecx ; mov ecx,[esp+8] ; push edx ; mov edx,[esp+10h] ;
//   call 0x009E2560 ; pop ecx ; ret 8 -- the value behind Object (+4) is copied into a `push ecx` local and passed by const reference
//   to the __fastcall CPersistTraitsSLONG::TransferOut 0x009E2560 (context ecx, out edx).
// Name-fix NF163 (q11748, found by NF136 with q5199): FableWin.pdb 0x03B589B0
//   ?TransferOut@?$CPersistContext_TransferableComponent@W4ArcheryOpenState@CQ_SunnyvaleMasterData@NScript@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
//   (the TransferIn / TransferOut pair of this T: TransferIn 0x03B58930 = the landed 0x00CDCD90); slot 0 of the RTTI-less component vtable
//   0x012C2ECC, whose slot 1 is the landed TransferIn 0x00CDCD90 and which only CPersistContext::Transfer<ArcheryOpenState> 0x00CDCC20
//   stores. Callee: Ego_r.pdb / FableWin.pdb ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z 0x009E2560
//   (engine/CPersistTraits.h FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT). Was the generated stand-in
//   CTransferableLong_009e2560::TransferOut calling the free TransferOut_009e2560(CPersistContext*, long, long*).
// Model: engine/CPersistContext_TransferableComponent.h ({vptr, T* Object +4}; private virtuals), the shape of the landed
//   TransferIn 0x00CDCD90 and TransferOut 0x00E1E0F0.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY
#define FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "engine/CQ_SunnyvaleMasterData.h"

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    long value = *Object;
    CPersistTraitsSLONG::TransferOut(context, out, value);
}

template void CPersistContext_TransferableComponent<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>::TransferOut(CPersistContext& context, CCharString& out);
