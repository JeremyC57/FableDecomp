// Reconstruction of 00c4b040 _Dest_val_pair_CCharString_CSymbolInfo
// Leaf: ?_Dest_val_pair_CCharString_CSymbolInfo@@YIXPAX0@Z
// push esi; mov esi,ecx; call A(esi); mov ecx,esi; call B(esi); mov ecx,esi; pop esi; jmp A
// = A(this); B(this); return A(this);  (last is tail -> jmp)
// Callees (NF44, round 6): A = PKCBufferedReader::ReturnAllBuffers() 0x00C4B010 (descriptive, libpk/PKCBufferedReader.h opt-in
// FABLE_RETAIL_PKCBUFFEREDREADER_RETURNALLBUFFERS; the landed row's name since NF42), B = PKCBufferedReader::Refill() 0x00C4A3B0 (the
// landed row's spelling); were the local stand-ins helperA / helperB. `self` is the reader those members run on (ecx kept in esi).
#define FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_RETURNALLBUFFERS
#include "libpk/PKCBufferedReader.h"

void __fastcall _Dest_val_pair_CCharString_CSymbolInfo(void* self, void* /*edx*/)
{
    PKCBufferedReader* reader = static_cast<PKCBufferedReader*>(self);
    reader->ReturnAllBuffers();
    reader->Refill();
    reader->ReturnAllBuffers();
}
