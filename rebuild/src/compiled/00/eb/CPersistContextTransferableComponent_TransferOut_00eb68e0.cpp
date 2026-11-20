// CPersistContext_TransferableComponent<NScript::CV_MurderTwistFinalScript::EAssassinState>::TransferOut(CPersistContext& context, CCharString& out) -- write the enum as a long through CPersistTraitsSLONG::TransferOut
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@W4EAssassinState@CV_MurderTwistFinalScript@NScript@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB68E0, 30 bytes, thiscall `ret 8`: the value is widened into a `push ecx` local and passed by const reference to the
//   __fastcall CPersistTraitsSLONG::TransferOut 0x009E2560 (context ecx, text edx, &value pushed).
// NF162 (2026-11-20, q11754): was the generated stand-in CTransferableLong_009e2560::TransferOut calling the free TransferOut_009e2560(CPersistContext*, long, long*)
//   (old file unattributed_TransferOutCTransferableLong009e2560_00eb68e0.cpp). Identity: slot 0 of the RTTI-less component vtable 0x012F047C whose slot 1 is
//   the landed TransferIn 0x00EB68C0 (NF136, same T); the vtable is stored by Transfer<EAssassinState> 0x00EB6750; FableWin.pdb
//   ?TransferOut@?$CPersistContext_TransferableComponent@W4EAssassinState@CV_MurderTwistFinalScript@NScript@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z (the TransferOut / TransferIn instance pair); the vtable dword is the only reference to 0x00EB68E0 (exe scan: no landed row calls it).
//   Model: engine/CPersistContext_TransferableComponent.h ({vptr, T* Object +4}; private virtuals) and engine/CPersistTraits.h
//   (FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT: ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z = 0x009E2560); the enum
//   from its script class header (engine/CV_MurderTwistFinalScript.h). Body as the landed twin 0x00E1E0F0.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "engine/CV_MurderTwistFinalScript.h"

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    long value = *Object;
    CPersistTraitsSLONG::TransferOut(context, out, value);
}

template void CPersistContext_TransferableComponent<NScript::CV_MurderTwistFinalScript::EAssassinState>::TransferOut(CPersistContext& context, CCharString& out);
