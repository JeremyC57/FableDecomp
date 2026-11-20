// CPersistContext_TransferableComponent<EHeroTrainingStatus>::TransferIn(CPersistContext& context, CStringParser& parser) -- parse the value as a long through CPersistTraitsSLONG::TransferIn and store it through Object
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent@W4EHeroTrainingStatus@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F93E, 32 bytes, thiscall `ret 8`: Object (+4) into esi before the call; the long is homed in the parser's dead
//   argument slot; CPersistTraitsSLONG::TransferIn 0x009E2590 is __fastcall (context ecx, parser edx, &value pushed).
// NF136 (2026-11-20, q5199 / q5220): was the free __fastcall stand-in CPersistContext_TransferableComponent_TransferIn (a local non-template
//   CPersistContext_TransferableComponent model) calling the free PC_Transfer(CPersistContext*, int, int*) declaration of 0x009E2590.
//   Identity: slot 1 of the RTTI-less component vtable 0x0124F2F4 (slot 0 = TransferOut 0x0057F95E; vtable stored at 0x0057F243 / 0x005803CE / 0x005803F8); FableWin.pdb
//   ?TransferIn@?$CPersistContext_TransferableComponent@W4EHeroTrainingStatus@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z; the vtable dword is the only reference to 0x0057F93E (exe scan: no landed row calls it).
//   Model: engine/CPersistContext_TransferableComponent.h ({vptr, T* Object +4}; private virtuals) and the generic inline CPersistTraits<T>::TransferIn
//   of engine/CPersistTraits.h (Ego_r.pdb / FableWin.pdb ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z = 0x009E2590).
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "engine/EHeroTrainingStatus.h"
#pragma optimize("y", off)   // size-optimised TU: retail keeps the EBP frame
#pragma optimize("s", on)

template <class T>
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    T* object = Object;                                  // read before the call (retail loads [ecx+4] first)
    long value;
    CPersistTraitsSLONG::TransferIn(context, parser, value);
    *object = (T)value;
}

template void CPersistContext_TransferableComponent<EHeroTrainingStatus>::TransferIn(CPersistContext& context, CStringParser& parser);
