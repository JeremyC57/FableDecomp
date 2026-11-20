// CPersistContext_TransferableComponent<EScriptingStateGroups>::TransferIn(CPersistContext& context, CStringParser& parser) -- parse the value as a long through CPersistTraitsSLONG::TransferIn and store it through Object
// Leaf: ?TransferIn@?$CPersistContext_TransferableComponent@W4EScriptingStateGroups@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AD7B0, 32 bytes, thiscall `ret 8`: Object (+4) into esi before the call; the long is homed in the parser's dead
//   argument slot; CPersistTraitsSLONG::TransferIn 0x009E2590 is __fastcall (context ecx, parser edx, &value pushed).
// NF136 (2026-11-20, q5199 / q5220): was a local CPersistContext_TransferableComponent<T> model (public virtuals: leaf UAEX; both PDBs have private EAEX) whose local
//   CPersistTraits<T> called the free TransferInLong(CPersistContext&, CStringParser&, long*).
//   Identity: slot 1 of the RTTI-less component vtable 0x0126D3AC (slot 0 = TransferOut 0x007AD7D0; vtable stored at 0x007AD747 / 0x007AD77F); FableWin.pdb
//   ?TransferIn@?$CPersistContext_TransferableComponent@W4EScriptingStateGroups@@@@EAEXAAVCPersistContext@@AAVCStringParser@@@Z; the vtable dword is the only reference to 0x007AD7B0 (exe scan: no landed row calls it).
//   Model: engine/CPersistContext_TransferableComponent.h ({vptr, T* Object +4}; private virtuals) and the generic inline CPersistTraits<T>::TransferIn
//   of engine/CPersistTraits.h (Ego_r.pdb / FableWin.pdb ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z = 0x009E2590).
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "engine/EScriptingStateGroups.h"

template <class T>
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    T* object = Object;                                  // read before the call (retail loads [ecx+4] first)
    long value;
    CPersistTraitsSLONG::TransferIn(context, parser, value);
    *object = (T)value;
}

template void CPersistContext_TransferableComponent<EScriptingStateGroups>::TransferIn(CPersistContext& context, CStringParser& parser);
