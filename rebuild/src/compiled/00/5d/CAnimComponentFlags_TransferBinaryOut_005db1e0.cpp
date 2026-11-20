// CAnimComponentFlags::TransferBinaryOut(CDataOutputStream& stream) const -- write the component's flag word as a signed long
// Leaf: ?TransferBinaryOut@CAnimComponentFlags@@UBEXAAVCDataOutputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB1E0, 16 bytes: mov eax,[ecx+8] ; mov ecx,[esp+4] ; push eax ;
//   call 0x00993EB0 ; ret 4.
// Name (NF197, 2026-11-20): Ego_r.pdb 0x0063AC70 ?TransferBinaryOut@CAnimComponentFlags@@UBEXAAVCDataOutputStream@@@Z (public
//   virtual const, the stream by reference: `ret 4`, ecx = this, the reference reloaded from [esp+4]); the catalog already names
//   CAnimComponentFlags::TransferBinaryOut. Ported from the stand-in S / local CDataOutputStream::Write(void*) onto the shared
//   engine/CAnimComponentFlags.h (Flags +0x08) and engine/CDataOutputStream.h (?WriteSLONG@CDataOutputStream@@QAEXJ@Z, 0x00993EB0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ANIMCOMPONENT
#include "fable_string.h"
#include "engine/CDataOutputStream.h"
#include "engine/CAnimComponentFlags.h"

void CAnimComponentFlags::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteSLONG(Flags);
}
