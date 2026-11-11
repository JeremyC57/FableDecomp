// CPersistContext::TransferMap<bool, EOpinionReactionType>(map<EOpinionReactionType, bool>& map) -- unnamed binary-only map transfer: CRC check, then the binary map reader (Mode 2) or writer (Mode 3)
// Leaf: ??$TransferMap@_NW4EOpinionReactionType@@@CPersistContext@@QAEXAAV?$map@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045DEE9, 53 bytes, `ret 4`; ecx = the context, one pushed map address; no frame.
// Identity (V = bool, K = EOpinionReactionType): COpinionReactionMaskDef +0x3C = Ego_r ReactionEnabled; COpinionSourceDef +0x50 = Ego_r ReactionFlag (map<EOpinionReactionType,bool>).
//   Ego_r.pdb / FableWin.pdb name the named form ??$TransferMap@_NW4EOpinionReactionType@@@CPersistContext@@QAEXPBDAAV?$map@... for this map, and
//   both stream helpers this row calls: GFSerialiseMapBinaryOut<bool,EOpinionReactionType,less> 0x0045DF1E (mode 3, the save stream) and
//   GFSerialiseMapBinaryIn<bool,EOpinionReactionType,less> 0x0045DF5E (mode 2, the load stream), __fastcall (stream ecx, map edx). The one-argument
//   parameter list is the established descriptive spelling of the unnamed retail-only transfer family (engine/CPersistContext.h
//   `TransferMap(map&)`, the sibling of the landed `Transfer(T&)` rows 0x00431020 / 0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E), then the Mode switch (`dec;dec;je / dec;jne`).
// Replaces a pad-model `CEngineLightingManager::UpdateShadowScene` stand-in (fake profiler callee for CheckCRC, free helper names).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"

enum EOpinionReactionType { };
template <class V, class K, class C> void FABLE_FASTCALL GFSerialiseMapBinaryIn(CMemoryDataInputStream& is, std::map<K, V, C, std::allocator<std::pair<const K, V> > >& m);
template <class V, class K, class C> void FABLE_FASTCALL GFSerialiseMapBinaryOut(CDataOutputStream& os, const std::map<K, V, C, std::allocator<std::pair<const K, V> > >& m);

template <> void CPersistContext::TransferMap<bool, EOpinionReactionType>(std::map<EOpinionReactionType, bool, std::less<EOpinionReactionType>, std::allocator<std::pair<const EOpinionReactionType, bool> > >& map)
{
    CheckCRC("");
    switch (Mode) {
    case MODE_LOAD_BINARY:
        GFSerialiseMapBinaryIn(*PLoadStream, map);
        break;
    case MODE_SAVE_BINARY:
        GFSerialiseMapBinaryOut(*PSaveStream, map);
        break;
    }
}
