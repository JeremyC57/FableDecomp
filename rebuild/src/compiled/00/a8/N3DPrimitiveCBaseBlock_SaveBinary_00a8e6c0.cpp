// N3DPrimitive::CBaseBlock::SaveBinary(CDataOutputStream&) -- two dwords, a bool, a byte and a bool through the stream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8E6C0, oracle length 71 bytes.
// Callees: CDataOutputStream::WriteULONG @ 0x00993EA0 (x2), WriteEBOOL @ 0x00993E30 (x2), WriteUBYTE @ 0x00993E50 (bytes below keep the old labels).
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; push edi ; mov edi,[esp+0Ch] ; push eax ; mov ecx,edi ; call WriteULong ;
//                mov ecx,[esi+4] ; push ecx ; mov ecx,edi ; call WriteULong ; xor edx,edx ; mov dl,[esi+8] ; mov ecx,edi ; push edx ;
//                call WriteBool ; xor eax,eax ; mov al,[esi+0Ah] ; mov ecx,edi ; push eax ; call WriteByte ; xor ecx,ecx ;
//                mov cl,[esi+9] ; push ecx ; mov ecx,edi ; call WriteBool ; pop edi ; pop esi ; ret 4
// Name fix (naming agent NF21, 2026-11-11): Ego_r.pdb 0x00BD0F70 / FableWin.pdb 0x032FECF0
//   ?SaveBinary@CBaseBlock@N3DPrimitive@@QAEXAAVCDataOutputStream@@@Z; the CDataOutputStream_Calls stand-in (WriteULong /
//   WriteBool / WriteByte) is the shared retail CDataOutputStream (Ego_r.pdb WriteULONG / WriteEBOOL / WriteUBYTE), the block
//   the shared engine/N3DPrimitive_CStaticBlock.h (Ego_r.pdb CBaseBlock: PrimitiveCount +0, StartIndex +4, IsStrip +8,
//   DegenerateTriangles +9, ChangeFlags +0xa). SaveBinary / WriteUBYTE are header opt-ins (FABLE_RETAIL_CBASEBLOCK_SAVEBINARY,
//   FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITEUBYTE).
#include "rebuild_abi.h"
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITEUBYTE
#include "engine/CDataOutputStream.h"
#define FABLE_RETAIL_CBASEBLOCK_SAVEBINARY
#include "engine/N3DPrimitive_CStaticBlock.h"

namespace N3DPrimitive {
void CBaseBlock::SaveBinary(CDataOutputStream& stream)
{
    stream.WriteULONG(PrimitiveCount);
    stream.WriteULONG(StartIndex);
    stream.WriteEBOOL(IsStrip);
    stream.WriteUBYTE(ChangeFlags);
    stream.WriteEBOOL(DegenerateTriangles);
}
}
