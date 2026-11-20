// CPersistContext_TransferableComponent<EHeroCentreDoorType>::TransferIn(CPersistContext& context, CStringParser& parser) -- parse the value as a long through CPersistTraitsSLONG::TransferIn and store it through Object
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent@W4EHeroCentreDoorType@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077FDA0, 32 bytes, thiscall `ret 8`: Object (+4) into esi before the call; the long is homed in the parser's dead
//   argument slot; CPersistTraitsSLONG::TransferIn 0x009E2590 is __fastcall (context ecx, parser edx, &value pushed).
// NF136 (2026-11-20, q5199 / q5220): was the generated stand-in CTransferableLong_009e2590::TransferIn calling the free TransferIn_009e2590(CPersistContext*, long, long*).
//   Identity: slot 1 of the RTTI-less component vtable 0x0126B038 (slot 0 = TransferOut 0x0077FDC0; vtable stored at 0x0077FC97 / 0x0077FE4F); FableWin.pdb
//   ?TransferIn@?$CPersistContext_TransferableComponent@W4EHeroCentreDoorType@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z; the vtable dword is the only reference to 0x0077FDA0 (exe scan: no landed row calls it).
//   Model: engine/CPersistContext_TransferableComponent.h ({vptr, T* Object +4}; private virtuals) and the generic inline CPersistTraits<T>::TransferIn
//   of engine/CPersistTraits.h (Ego_r.pdb / FableWin.pdb ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z = 0x009E2590).
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
enum EHeroCentreDoorType { };   // global enum (both PDBs: W4EHeroCentreDoorType@@); no shared header declares it; enumerators not needed here

template <class T>
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    T* object = Object;                                  // read before the call (retail loads [ecx+4] first)
    long value;
    CPersistTraitsSLONG::TransferIn(context, parser, value);
    *object = (T)value;
}

template void CPersistContext_TransferableComponent<EHeroCentreDoorType>::TransferIn(CPersistContext& context, CStringParser& parser);
