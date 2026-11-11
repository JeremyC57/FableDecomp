// CPersistContext::TransferMap<float, EDamageAttribute>(map<EDamageAttribute, float>& map) -- unnamed binary-only map transfer: CRC check, then the binary map reader (Mode 2) or writer (Mode 3)
// Leaf: ??$TransferMap@MW4EDamageAttribute@@@CPersistContext@@QAEXAAV?$map@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00461894, 53 bytes, `ret 4`; ecx = the context, one pushed map address; no frame.
// Identity (V = float, K = EDamageAttribute): CArmourDef::Transfer +0x48 / +0x54 (map<EDamageAttribute,float>).
//   Ego_r.pdb / FableWin.pdb name the named form ??$TransferMap@MW4EDamageAttribute@@@CPersistContext@@QAEXPBDAAV?$map@... for this map, and
//   both stream helpers this row calls: GFSerialiseMapBinaryOut<float,EDamageAttribute,less> 0x004618C9 (mode 3, the save stream) and
//   GFSerialiseMapBinaryIn<float,EDamageAttribute,less> 0x0046190A (mode 2, the load stream), __fastcall (stream ecx, map edx). The one-argument
//   parameter list is the established descriptive spelling of the unnamed retail-only transfer family (engine/CPersistContext.h
//   `TransferMap(map&)`, the sibling of the landed `Transfer(T&)` rows 0x00431020 / 0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E), then the Mode switch (`dec;dec;je / dec;jne`).
// Replaces a pad-model `CEngineLightingManager::UpdateShadowScene` stand-in (fake profiler callee for CheckCRC, free helper names).
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"

enum EDamageAttribute { };
template <class V, class K, class C> void FABLE_FASTCALL GFSerialiseMapBinaryIn(CMemoryDataInputStream& is, std::map<K, V, C, std::allocator<std::pair<const K, V> > >& m);
template <class V, class K, class C> void FABLE_FASTCALL GFSerialiseMapBinaryOut(CDataOutputStream& os, const std::map<K, V, C, std::allocator<std::pair<const K, V> > >& m);

template <> void CPersistContext::TransferMap<float, EDamageAttribute>(std::map<EDamageAttribute, float, std::less<EDamageAttribute>, std::allocator<std::pair<const EDamageAttribute, float> > >& map)
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
