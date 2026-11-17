// CPersistContext::TransferVector<bool>(std::vector<bool>&) -- the one-argument release persist transfer of a bool vector: CheckCRC(""), then GFSerialiseVectorBinaryIn<bool> on the load stream in MODE_LOAD_BINARY, else GFSerialiseVectorBinaryOut<bool> on the save stream
// Leaf: ??$TransferVector@_N@CPersistContext@@QAEXAAV?$vector@_NV?$allocator@_N@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00467597, 46 bytes, size-optimised TU (`dec eax ; dec eax ; je`).
// Retail bytes:  push esi ; push offset "" ; mov esi,ecx ; call CPersistContext::CheckCRC ; mov eax,[esi+18h] ; mov edx,[value] ; dec eax ;
//   dec eax ; je L ; mov ecx,[esi+28h] ; call GFSerialiseVectorBinaryOut ; ... L: mov ecx,[esi+24h] ; call GFSerialiseVectorBinaryIn
// Identity (NF56, round 7, queue q10456): the one-argument member template TransferVector<T> as TLC's release persist macros call it
//   (engine/CPersistContext.h FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY: CheckCRC with the empty key, MODE_LOAD_BINARY -> In,
//   else Out), T = bool: its callees are the bool instances ??$GFSerialiseVectorBinaryIn@_N@@... 0x004675F6 / ...Out@_N@@... 0x004675C5
//   (Ego_r.pdb 0x0046B40C / 0x0046AF88 + FableWin.pdb 0x019C6E22 / 0x019C6D32 spell both; the named twin ??$TransferVector@_N@CPersistContext@@
//   QAEXPBD... is Ego_r 0x0046B4CB / FableWin 0x01990D50). Was the generated stand-in CPersistContext::TransferBinary_00467597 (no Leaf
//   line) over a local CPersistContext model and C2DVector / CRGBColour-labelled serialiser stand-ins.
#pragma optimize("s",on)
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CCHARSTRING
#include "stlport/fable_containers.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"

template void CPersistContext::TransferVector<bool>(std::vector<bool>& value);
