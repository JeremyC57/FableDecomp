// CPersistContext_TransferableComponent<NScript::CV_AssassinAttacksScript::AA_LocationIdentifier>::TransferOut(CPersistContext& context, CCharString& out) -- write the enum as a long through CPersistTraitsSLONG::TransferOut
// Leaf: ?TransferOut@?$CPersistContext_TransferableComponent@W4AA_LocationIdentifier@CV_AssassinAttacksScript@NScript@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E38F60, 30 bytes, thiscall `ret 8`: the value is widened into a `push ecx` local and passed by const reference to the
//   __fastcall CPersistTraitsSLONG::TransferOut 0x009E2560 (context ecx, text edx, &value pushed).
// NF162 (2026-11-20, q11752): was the generated stand-in CTransferableLong_009e2560::TransferOut calling the free TransferOut_009e2560(CPersistContext*, long, long*)
//   (old file unattributed_TransferOutCTransferableLong009e2560_00e38f60.cpp). Identity: slot 0 of the RTTI-less component vtable 0x012E37B4 whose slot 1 is
//   the landed TransferIn 0x00E38F40 (NF136, same T); the vtable is stored by Transfer<AA_LocationIdentifier> 0x00E38DD0; FableWin.pdb
//   ?TransferOut@?$CPersistContext_TransferableComponent@W4AA_LocationIdentifier@CV_AssassinAttacksScript@NScript@@@@EAEXAAVCPersistContext@@AAVCCharString@@@Z (the TransferOut / TransferIn instance pair); the vtable dword is the only reference to 0x00E38F60 (exe scan: no landed row calls it).
//   Model: engine/CPersistContext_TransferableComponent.h ({vptr, T* Object +4}; private virtuals) and engine/CPersistTraits.h
//   (FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT: ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z = 0x009E2560); the enum
//   from its script class header (engine/CV_AssassinAttacksScript.h). Body as the landed twin 0x00E1E0F0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_AssassinAttacksScript.h"

template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    long value = *Object;
    CPersistTraitsSLONG::TransferOut(context, out, value);
}

template void CPersistContext_TransferableComponent<NScript::CV_AssassinAttacksScript::AA_LocationIdentifier>::TransferOut(CPersistContext& context, CCharString& out);
